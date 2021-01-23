/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_UPDATER_H
#define _IO_JACKHAY_SWAMP_UPDATER_H

#include <string>

/*
 * Checks for new content
 * Note: not currently set up to pull code changes,
 * just map content changes. Therefore, only
 * minor versions can be pulled
 */
namespace updater {

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
                    const std::string& base_path_parent);

  /**
   * Pull the initial cfg for the app
   * @param  base_path        path to resource folder
   * @param  base_path_parent the parent of the resource folder
   * @return                  whether setup succeeded
   */
  bool pull_initial(const std::string& base_path,
                    const std::string& base_path_parent);
}

#endif /*_IO_JACKHAY_SWAMP_UPDATER_H*/
