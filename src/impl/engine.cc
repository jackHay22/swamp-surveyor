/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "engine.h"
#include <iostream>
#include <ctime>
#include <pthread.h>
#include <chrono>
#include <thread>
#include "logger.h"

namespace impl {
namespace engine {

  //the duration in milliseconds to sleep between updates
  const int TICK_SLEEP = 50;
  const int FRAMES_PER_FPS = 10;
  const float MS_PER_SECOND = 1000.0;

  /**
   * Render debug info to the screen
   * @param renderer the renderer
   * @param fps      the frames per second value
   * @param font     the font for debug output
   */
  void render_debug_info(SDL_Renderer& renderer, int fps, TTF_Font *font) {
    SDL_Surface* debug_text;
    SDL_Color color = {255,255,255};

    //create text for debug
    debug_text = TTF_RenderText_Solid(font,std::to_string(fps).c_str(),color);

    if (!debug_text) {
      logger::log_err("failed to render debug text: " + std::string(TTF_GetError()));

    } else {
      //create a texture
      SDL_Texture* text_texture = SDL_CreateTextureFromSurface(&renderer,debug_text);

      //the position on screen
      SDL_Rect dest = {0,0,debug_text->w,debug_text->h};

      //render the text
      SDL_RenderCopy(&renderer,text_texture,&dest,&dest);

      //free
      SDL_DestroyTexture(text_texture);
      SDL_FreeSurface(debug_text);
    }
  }

  /**
   * Pthread handler for update cycle
   * @param cfg_v the manager
   */
  void* update_thread(void *manager_v) {
    //shared ptr to the manager
    std::shared_ptr<state::state_manager_t> manager =
      *((std::shared_ptr<state::state_manager_t>*) manager_v);

    //update state while the system is running
    while (manager->is_running()) {
      //update the game state
      manager->update();

      //sleep until the next tick
      std::this_thread::sleep_for(std::chrono::milliseconds(TICK_SLEEP));
    }

    pthread_exit(NULL);
  }

  /**
   * Start the update thread
   * @param manager the gamestate manager
   * @return whether the thread was started successfully
   */
  bool start_update_thread(std::shared_ptr<state::state_manager_t> manager) {
    pthread_t thread;

    //start the background thread
    if (pthread_create(&thread,
                       NULL,
                       update_thread,
                       (void*) &manager)) {
      logger::log_err("failed to start update thread");
      return false;
    }

    return true;
  }

  /**
   * Start the render loop
   * @param renderer the sdl renderer
   * @param manager the gamestate manager
   * @param debug whether debug enabled
   * @param debug_font_name the font to use
   * @return success or failure
   */
  bool start_renderer(SDL_Renderer& renderer,
                      std::shared_ptr<state::state_manager_t> manager,
                      const bool debug,
                      const std::string& debug_font_name) {

    //Event handler
		SDL_Event e;
    bool running = true;

    //track fps in debug mode
    int fps = 0;
    TTF_Font *debug_font;
    //update fps every 10 frames
    int frames = FRAMES_PER_FPS;

    //init debug font
    if (debug) {
      //load the debug font
      debug_font = TTF_OpenFont(debug_font_name.c_str(), 8);
      if (!debug_font) {
        logger::log_err("failed to load font: " + debug_font_name +
                        " (" + std::string(TTF_GetError()) + ")");
        return false;
      }
    }

    //run
    while (manager->is_running()) {
      //get the cycle start time
      std::clock_t start = std::clock();

      //check events
      while (SDL_PollEvent(&e) != 0 ) {
        //check for a quit event
        if (e.type == SDL_QUIT) {
          manager->set_running(false);
        } else {
          //handle event (player)
          manager->handle_event(e);
        }
      }

      //Clear screen
      SDL_SetRenderDrawColor(&renderer,0xFF,0xFF,0xFF,0xFF);
      SDL_RenderClear(&renderer);

      //render the current state
      manager->render(renderer);

      //render debug info
      if (debug) {
        render_debug_info(renderer,fps,debug_font);
      }

      //Update screen
      SDL_RenderPresent(&renderer);

      if (debug) {
        //update fps every 10 frames
        if (frames <= 0) {
          //calc the ms elapsed
          int ms = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
          if (ms > 0) {
            fps = int (MS_PER_SECOND / ms);
          }
          frames = FRAMES_PER_FPS;
        } else {
          frames--;
        }
      }
    }

    return true;
  }
}}
