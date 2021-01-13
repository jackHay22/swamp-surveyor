/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "engine.h"
#include <pthread.h>
#include "logger.h"

namespace impl {
namespace engine {

  /**
   * Pthread handler for update cycle
   * @param cfg_v the manager
   */
  void* update_thread(void *manager_v) {
    //shared ptr to the manager
    std::shared_ptr<state::state_manager_t> manager =
      *((std::shared_ptr<state::state_manager_t>*) manager_v);

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
   * @param camera   the camera
   * @manager the gamestate manager
   * @return success or failure
   */
  bool start_renderer(SDL_Renderer& renderer,
                      SDL_Rect& camera,
                      std::shared_ptr<state::state_manager_t> manager) {

    //Event handler
		SDL_Event e;
    bool running = true;

    //run
    while (running) {
      //check events
      while (SDL_PollEvent(&e) != 0 ) {
        //check for a quit event
        if (e.type == SDL_QUIT) {
          running = false;
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

      //Update screen
      SDL_RenderPresent(&renderer);
    }

    return true;
  }
}}
