#include <iostream>
#include <SDL.h>
#include <cstdlib>
using namespace std;

const int buoc = 50;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Game snake";

class food{
private:
    int x;
    int y;
public:
    food(){
        x = ramdomPointX();
        y = ramdomPointY();
    }
    int ramdomPointX(){
        return (rand() % (SCREEN_WIDTH/50))*50;
    }
    int ramdomPointY(){
        return (rand() % (SCREEN_HEIGHT/50))*50;
    }
    void newPoint(){
        x = ramdomPointX();
        y = ramdomPointY();
    }
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
};

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
            y -= buoc;
        }else if (key == 'd'){
            y += buoc;
        }else if (key == 'l'){
            x -= buoc;
        }else if (key == 'r'){
            x += buoc;
        }
    }
    bool checkDie(){
        if (x >= SCREEN_WIDTH || x < 0 || y >=SCREEN_HEIGHT || y < 0){
            return false;
        }
        return true;
    }
    bool eat(food foodSnake){
        if (foodSnake.getX() == x && foodSnake.getY() == y){
            size_snake++;
            return true;
        }
        return false;
    }
};

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

     SDL_Rect fillFood;
     fillFood.h = 50;
     fillFood.w = 50;

//     waitUntilKeyPressed();
     Snake mySnake;
     food foodOfSnake;

     SDL_Event e;
     char key = 'r';

     while (true) {
        if ( SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym){
                    case SDLK_ESCAPE:
                        {
                            quitSDL(window, renderer);
                            return 0;
                        }
                    case SDLK_UP:
                        {
                            key = 'u';
                            break;
                        }
                    case SDLK_DOWN:
                        {
                            key = 'd';
                            break;
                        }
                    case SDLK_LEFT:
                        {
                            key = 'l';
                            break;
                        }
                    case SDLK_RIGHT:
                        {
                            key = 'r';
                            break;
                        }
                }
            }else if(e.type == SDL_QUIT){
                quitSDL(window, renderer);
                return 0;
            }
        }

        if ( mySnake.eat(foodOfSnake) ){
            foodOfSnake.newPoint();
        }
        if ( mySnake.checkDie() == false ){
            return 0;
        }

        SDL_Delay(200);
        mySnake.getKey(key);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        filledRect.x = mySnake.getX();
        filledRect.y = mySnake.getY();
        SDL_RenderFillRect(renderer, &filledRect);

        SDL_SetRenderDrawColor(renderer, 255, 253, 7, 5);
        fillFood.x = foodOfSnake.getX();
        fillFood.y = foodOfSnake.getY();

        SDL_RenderFillRect(renderer, &fillFood);

        SDL_RenderPresent(renderer);

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
