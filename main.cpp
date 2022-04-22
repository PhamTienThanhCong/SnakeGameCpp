#include <iostream>
#include <deque>
#include <SDL.h>
using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const string WINDOW_TITLE = "Game snake";
const int jump = 25;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();

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
void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
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
        SDL_Delay(0);

     }
}

class food{
public:

    int randomX(){
        return (rand() % (SCREEN_WIDTH/25))*25;
    }
    int randomY(){
        return (rand() % (SCREEN_HEIGHT/25))*25;
    }
    void newPoint(){
        x = randomX();
        y = randomY();
    }
    int x = randomX();
    int y = randomY();
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
};


class snake{
public:
    int x;
    int y;
    int x_before;
    int y_before;
    int sizeSnake = 2;
    deque<int> bodyX;
    deque<int> bodyY;
    snake(){
        x = 250;
        y = 250;
        x_before = x;
        y_before = y;
        bodyX.push_front(200);
        bodyX.push_front(175);

        bodyY.push_front(250);
        bodyY.push_front(250);
    }
    int getX(){
        return x;
    }
    int bodyUpdate(int type){
        bodyX.push_front(x_before);
        bodyY.push_front(y_before);
        if (type == 0){
            bodyX.pop_back();
            bodyY.pop_back();
        }
//        jump;
    }
    int getY(){
        return y;
    }
    void getKey(char key){
        x_before = x;
        y_before = y;
        if (key == 'u'){
            y -= jump;
        }else if (key == 'd'){
            y += jump;
        }else if (key == 'l'){
            x -= jump;
        }else if (key == 'r'){
            x += jump;
        }
    }
    bool checkDie(){
        if (x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0){
            return false;
        }
        for (int i = 0 ; i < sizeSnake ; i++){
            if ( x == bodyX.at(i) && y == bodyY.at(i) ){
                return false;
            }
        }
        return true;
    }
    bool eatFood(food f){
        if (x == f.getX() && y == f.getY()){
            sizeSnake++;
            bodyUpdate(1);
            return true;
        }
        bodyUpdate(0);
        return false;
    }

};

int main(int argc, char* argv[]){

     SDL_Window* window;
     SDL_Renderer* renderer;
     initSDL(window, renderer);
     SDL_RenderClear(renderer);
     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
     SDL_Event e;
     snake mySnake;
     food myFood;
     char key = 'r';


     SDL_Rect headSnake;
     headSnake.h = 25;
     headSnake.w = 25;

     SDL_Rect foodofSnake;
     foodofSnake.h = 25;
     foodofSnake.w = 25;

     bool checkEventKey = true;
     while (checkEventKey) {
        while ( SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP && key != 'd') {key = 'u';}
                if (e.key.keysym.sym == SDLK_DOWN && key != 'u') {key = 'd';}
                if (e.key.keysym.sym == SDLK_LEFT && key != 'r') {key = 'l';}
                if (e.key.keysym.sym == SDLK_RIGHT && key != 'l') {key = 'r';}
            }
            if(e.type == SDL_QUIT) checkEventKey = false;
        }
        mySnake.getKey(key);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        headSnake.x = mySnake.getX();
        headSnake.y = mySnake.getY();
        SDL_RenderFillRect(renderer, &headSnake);

        if (mySnake.checkDie() == false) return 0;

        if (mySnake.eatFood(myFood) == true) {
            myFood.newPoint();

        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        foodofSnake.x = myFood.getX();
        foodofSnake.y = myFood.getY();
        SDL_RenderFillRect(renderer, &foodofSnake);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0 ; i < mySnake.sizeSnake ; i++){
            headSnake.x = mySnake.bodyX.at(i);
            headSnake.y = mySnake.bodyY.at(i);
            SDL_RenderFillRect(renderer, &headSnake);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(150);
    }

    quitSDL(window, renderer);
    return 0;
}
