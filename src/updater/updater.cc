/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "updater.h"
#include "../impl/logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <json/nlohmann_json.h>
#include <sys/stat.h>

namespace updater {

  typedef nlohmann::json json;

  using namespace impl;

  //the domain and path for metadata
  #define UPDATE_DOMAIN "jackhay.io"
  #define UPDATE_PATH "/swamp_surveyor/latest.json"
  #define MINOR_KEY "minor"
  #define MAJOR_KEY "major"
  #define BUNDLE_KEY "bundle"
  #define ZIP_EXT ".zip"
  #define FALLBACK "resources_fallback"
  #define LATEST "-latest"

  const std::string HTTPS = "https://";
  const std::string QUOTE = "\"";

  /**
   * Curl write callback
   */
  size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
  }

  /**
   * Curl file write callback
   */
  size_t fwrite_callback(void *ptr, size_t size, size_t nmemb, FILE *file_stream) {
    return fwrite(ptr, size, nmemb, file_stream);
  }

  /**
   * Make a get request
   * @param url      the path to request
   * @param response the response
   */
  void get_req(const std::string& url, std::string& response) {
    //init
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();

    //make the request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);

    //clean up
    curl_easy_cleanup(curl);
  }

  /**
   * Download a zip file
   * @param url  the url to request
   * @param path the path to write the file to
   * @param folder folder to unzip into
   * @return success/failure
   */
  bool zip_get_req(const std::string& url,
                   const std::string& path,
                   const std::string& folder) {
    FILE *dest_fptr;
    CURLcode cres;
    //init curl
    CURL* curl = curl_easy_init();

    if (curl) {
      //write to the path
      dest_fptr = fopen(path.c_str(),"wb");

      //make the request
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, dest_fptr);
      cres = curl_easy_perform(curl);

      //clean up
      curl_easy_cleanup(curl);

      //close the file and unzip
      if (!fclose(dest_fptr)) {
        const std::string zip_cmd = "unzip -qq -o " +
                                   QUOTE + path + QUOTE +
                                   " -d " + QUOTE + folder + QUOTE;
        logger::log_info("exec: " + zip_cmd);
        int res = system(zip_cmd.c_str());

        //remove the zip
        remove(path.c_str());

        return !res;
      }
    }
    return false;
  }

  /**
   * Check if a json object contains a particular number by some key
   * @param j   the json object
   * @param key the key to look for
   * @return whether a number with the given key was found
   */
  bool contains_number(const json j, const std::string& key) {
    return j.count(key) && j[key].is_number();
  }

  /**
   * Check if a json object contains a particular string by some key
   * @param  j   the json object
   * @param  key the key to look for
   * @return     whether a string with the given key was found
   */
  bool contains_string(const json j, const std::string& key) {
    return j.count(key) && j[key].is_string();
  }

  /**
   * Pull content updates
   * @param curr_version_major the downloaded major version
   * @param curr_version_minor the downloaded minor version
   * @param base_path          the location of the swamp surveyor rsrc folder
   * @param base_path_parent   path to rsrc folder parent
   * @return whether new content was pulled
   */
  bool pull_updates(int curr_version_major,
                    int curr_version_minor,
                    const std::string& base_path,
                    const std::string& base_path_parent) {
    //pull metadata
    try {
      std::string response;
      //get the update
      get_req(HTTPS + UPDATE_DOMAIN + UPDATE_PATH, response);

      //parse as json
      json latest = json::parse(response);

      //check for required fields
      if (contains_number(latest,MINOR_KEY) &&
          contains_number(latest,MAJOR_KEY) &&
          contains_string(latest,BUNDLE_KEY)) {

        //get the new version number
        int latest_minor_v = latest[MINOR_KEY].get<int>();
        int latest_major_v = latest[MAJOR_KEY].get<int>();
        //the url for the bundle
        const std::string bundle_loc = HTTPS + std::string(UPDATE_DOMAIN) +
                                       latest[BUNDLE_KEY].get<std::string>();
        const std::string version = std::to_string(latest_major_v) + "-" +
                                    std::to_string(latest_minor_v);

        //check for new version
        if (latest_minor_v > curr_version_minor) {
          //the path to download to
          const std::string temp_target = base_path + "latest" + version + ZIP_EXT;
          logger::log_info("pulling latest configuration version: " +
                           version + " from [" + bundle_loc + "] to [" +
                           temp_target + "]");

          //download the content
          if (zip_get_req(bundle_loc,temp_target,base_path_parent)) {
            const std::string fallback_target = base_path_parent + FALLBACK;

            //rename current as fallback
            if (rename(base_path.c_str(),fallback_target.c_str()) != 0) {
              logger::log_info("failed to rename current rsrc directory");
              return false;
            }

            const std::string extract_target = base_path_parent + "resources";
            //rename latest as current
            if (rename(extract_target.c_str(), base_path.c_str()) != 0) {
              //rename fallback
              rename(fallback_target.c_str(),base_path.c_str());
              return false;
            }
            return true;
          } else {
            logger::log_info("failed to download new zip archive, skipping");
          }
        }
      }

    } catch (...) {
      logger::log_info("failed to pull new content from jackhay.io, skipping");
    }
    return false;
  }

  /**
   * Pull the initial cfg for the app
   * @param  base_path        path to resource folder
   * @param  base_path_parent the parent of the resource folder
   * @return                  whether setup succeeded
   */
  bool pull_initial(const std::string& base_path,
                    const std::string& base_path_parent) {
    //pull metadata
    try {
      std::string response;
      //get the update
      get_req(HTTPS + UPDATE_DOMAIN + UPDATE_PATH, response);

      //parse as json
      json latest = json::parse(response);

      //check for required fields
      if (contains_string(latest,BUNDLE_KEY)) {
        const std::string bundle_loc = HTTPS + std::string(UPDATE_DOMAIN) +
                                       latest[BUNDLE_KEY].get<std::string>();

        //where to save the zip
        const std::string temp_target = base_path_parent + "initial" + ZIP_EXT;
        const std::string extract_target = base_path_parent + "resources";

        logger::log_info("mkdir " + base_path_parent);
        //attempt to make target dir
        if (mkdir(base_path_parent.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
          logger::log_info("unzipping " + temp_target);
          //unzip the resource folder
          if (zip_get_req(bundle_loc,temp_target,base_path_parent)) {
            logger::log_info("renaming " + extract_target + " to " + base_path);
            //rename to target
            return rename(extract_target.c_str(),base_path.c_str()) == 0;
          }
        }
      }
      return false;

    } catch (...) {
      return false;
    }
  }
}
