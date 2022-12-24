#include "include/print.h"
#include "include/thrower.h"

#include "include/sdl++.h"
#include "include/posVector-RC.h"

#include <thread>
#include <array>
#include <string>
using namespace std::literals;

#include "day12.h"



void drawTexture( Grid<RGBA>  &pixels, SDL_Texture *texture)
{
    static  int pos{};

    getPixels(pixels);

///

    int     pitch{};
    void   *destination{};

    if (SDL_LockTexture(texture, nullptr, &destination, &pitch) != 0) 
    {
        throw_runtime_error("SDL_LockTexture : "s + SDL_GetError());
    }

    memcpy(destination, pixels.data.data(), pitch * pixels.height);

    SDL_UnlockTexture(texture);
}



void windowThread(int width, int height)
try
{
    SDL         sdl;
    SDLWindow   window{"Day12", width*8,height*8};
    SDLRenderer renderer{window};
    SDLTexture  texture{renderer,width,height};
    Grid<RGBA>  pixels{width,height};

    SDL_RenderSetLogicalSize(renderer,width, height);

    bool done = false; 
    while( !done)
    { 
        SDL_Event e{};
        
        while( SDL_PollEvent( &e ) )
        { 
            if( e.type == SDL_QUIT ) 
            {
                done = true;
            }
        }

        drawTexture      (pixels, texture);
        
        SDL_RenderClear  (renderer);
        SDL_RenderCopy   (renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer );      // synced with vsync
    }


}
catch(std::exception const &e)
{
    print("{}",e.what());
}



std::thread createWindow(int width, int height)
{
    return std::thread{windowThread,width, height};
}
