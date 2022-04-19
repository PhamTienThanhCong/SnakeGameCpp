#include <iostream>
#include <SDL.h>
using namespace std;

class Snake{
private:
    int x;
    int y;
    int size_snake;
public:
    Snake(){
        x = 0;
        y = 0;
        size_snake = 3;
    }
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    void getKey(char key){
        if (key == 'u'){
            y -= 50;
        }else if (key == 'd'){
            y += 50;
        }else if (key == 'l'){
            x -= 50;
        }else if (key == 'r'){
            x += 50;
        }
    }
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Game snake";

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

     // vẽ hình chữ nhật
     SDL_Rect filledRect;
     filledRect.h = 50;
     filledRect.w = 50;

//     waitUntilKeyPressed();
     SDL_Event e;
     Snake mySnake;
     char key = 'r';

     while (true) {
        if ( SDL_WaitEvent(&e) != 0 && e.type == SDL_KEYDOWN){
            if (e.key.keysym.sym == SDLK_ESCAPE){
                quitSDL(window, renderer);
                return 0;
            }else if(e.key.keysym.sym == SDLK_UP){
                key = 'u';
            }else if(e.key.keysym.sym == SDLK_DOWN){
                key = 'd';
            }else if(e.key.keysym.sym == SDLK_LEFT){
                key = 'l';
            }else if(e.key.keysym.sym == SDLK_RIGHT){
                key = 'r';
            }
        }else if(e.type == SDL_QUIT){
            quitSDL(window, renderer);
            return 0;
        }
        mySnake.getKey(key);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        filledRect.x = mySnake.getX();
        filledRect.y = mySnake.getY();

        SDL_RenderDrawRect(renderer, &filledRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(200);
     }

//     quitSDL(window, renderer);
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
     //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
     //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
     if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

     //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
     SDL_RENDERER_PRESENTVSYNC);
     //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
     //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

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
