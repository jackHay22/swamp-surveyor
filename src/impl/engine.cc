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
#include "utils.h"

namespace impl {
namespace engine {

  //the duration in milliseconds to sleep between updates
  const int TICK_SLEEP = 50;
  const int FRAMES_PER_FPS = 10;
  const float MS_PER_SECOND = 1000.0;

  /**
   * Take the max value of two numbers
   * @param  a the first number
   * @param  b the second number
   * @return   the greater of a and b
   */
  inline int max(int a, int b) {
    if (a > b) {
      return a;
    }
    return b;
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
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

      //update the game state
      manager->update();

      //elapsed time
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

      //calculate the elapsed milliseconds
      int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

      //sleep until the next tick
      std::this_thread::sleep_for(std::chrono::milliseconds(max(TICK_SLEEP - elapsed_ms, 0)));
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
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

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

      //avoid rendering after stopping
      if (!manager->is_running()) {
        break;
      }

      //Clear screen
      SDL_SetRenderDrawColor(&renderer,0xFF,0xFF,0xFF,0xFF);
      SDL_RenderClear(&renderer);

      //render the current state
      manager->render(renderer,debug);

      //render debug info
      if (debug) {
        //render the fps
        utils::render_text(renderer,
                           std::to_string(fps),
                           0,0,*debug_font);

        //render debug info
        manager->render_debug_info(renderer,*debug_font);
      }

      //Update screen
      SDL_RenderPresent(&renderer);

      if (debug) {
        //update fps every 10 frames
        if (frames <= 0) {
          //elapsed time
          std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

          //calculate the elapsed milliseconds
          int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

          //calculate fps
          if (elapsed_ms > 0) {
            fps = int (MS_PER_SECOND / elapsed_ms);
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
