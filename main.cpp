#include <iostream>
#include <SDL.h>
#include <unistd.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);

SDL_Surface* IMG_Load(const char* path);
int main(int argc, char* argv[])
{

     SDL_Window* window;
     SDL_Renderer* renderer;
     initSDL(window, renderer);

     SDL_RenderClear(renderer);
     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

     SDL_Rect filledRect;
     filledRect.x = 150;
     filledRect.y = 100;
     filledRect.h = 100;
     filledRect.w = 500;
     SDL_RenderDrawRect(renderer, &filledRect);



     SDL_RenderPresent(renderer);

     sleep(5);

     SDL_SetRenderDrawColor(renderer, 2, 4, 87, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
     waitUntilKeyPressed();
     quitSDL(window, renderer);
     return 0;

}


SDL_Surface* IMG_Load(const char* path){

}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
     if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

     window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
     if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
     SDL_RENDERER_PRESENTVSYNC);

     if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
     SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
     SDL_Event e;
     while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
     }
}
