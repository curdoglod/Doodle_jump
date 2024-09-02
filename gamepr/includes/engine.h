#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include "sprite.h"
#include "chrono"

enum class MouseButton {
    LEFT,
    RIGHT,
    OTHER
};

enum class KeyButton {
    LEFT,
    RIGHT,
    OTHER
};

class Engine {
public:
    Engine();

    void preInit();

    virtual void Init() = 0;

    void init();

    void tick(float deltaTime);

    virtual void tick() = 0;

    void run();

    unsigned int getTickCount();

    virtual void drawScene() = 0;

    virtual void onMouseMove(int x, int y) = 0;

    virtual void onMouseButtonClick(MouseButton button, bool isReleased) = 0;

    virtual void onKeyPressed(KeyButton key) = 0;

    virtual void onKeyReleased(KeyButton key) = 0;

    Sprite* createSprite(const std::string& texturePath);

    void getScreenSize(int& w, int& h);
 
    std::pair<int,int> getScreenSize(){
        return std::make_pair(m_window.getSize().x, m_window.getSize().y);
    }
            

    void setScreenSize(const int& w, const int& h) {
        display_width = w;
        display_height = h; 
    };


private:
    sf::RenderWindow m_window;
    std::string nameWindow;
    int display_width = 1280;
    int display_height = 720;
    std::pair<int, int> screensize;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

#endif // ENGINE_H
