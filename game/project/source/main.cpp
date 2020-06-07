#define TE_OPENGL

#include"TE.h"

extern TornadoEngine engine;
extern Input inputManager;

class Game: public State{
private:

protected:
    void OnStart(){

    }

    void OnUpdate(float elapsedTime){
        if(inputManager.keyPressed(KeyCode::A))
            SetDrawColor(255, 0, 0, 255);
    }

    void OnDraw(){

    }

    ~Game(){

    }
};

int main(int argc, char *argv[]){
    engine.Init("Game", 1280, 720);

    Game *game = new Game();
    engine.Start(game);

    return 0;
}
