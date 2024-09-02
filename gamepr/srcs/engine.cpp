#include "../includes/engine.h"


Engine::Engine()
{
}

void Engine::preInit()
{
    //m_window.create(sf::VideoMode(display_width, display_height), "Game", sf::Style::Titlebar | sf::Style::Close );
    m_window.create(sf::VideoMode(display_width, display_height), "Game");
    m_window.setVerticalSyncEnabled(true);
}

void Engine::init()
{
    Init();
    m_startTime = std::chrono::high_resolution_clock::now();
}

void Engine::tick(float deltaTime)
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if ((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)) {
            bool IsRealesed = false;
            if (event.type == sf::Event::MouseButtonReleased) IsRealesed = true;
            MouseButton button = MouseButton::OTHER;
            if (event.mouseButton.button == sf::Mouse::Left ) button = MouseButton::LEFT; else  if (event.mouseButton.button == sf::Mouse::Right) button = MouseButton::RIGHT;
            onMouseButtonClick(button, IsRealesed);
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                onKeyPressed(KeyButton::LEFT);
            }
            else if (event.key.code == sf::Keyboard::D|| event.key.code == sf::Keyboard::Right) {
                onKeyPressed(KeyButton::RIGHT);
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                onKeyReleased(KeyButton::LEFT);
            }
            else if (event.key.code == sf::Keyboard::D|| event.key.code == sf::Keyboard::Right) {
                onKeyReleased(KeyButton::RIGHT);
            }
        }

        onMouseMove(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
    }


    m_window.clear();

    tick();
    drawScene();
    m_window.display();
}

void Engine::run()
{
    preInit();
    init();

    sf::Clock clock;
    while (m_window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        tick(deltaTime);
    }
}

unsigned int Engine::getTickCount()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(now - m_startTime);
    return static_cast<unsigned int>(elapsed.count());
}

Sprite* Engine::createSprite(const std::string& texturePath) {
    //   std::cout << texturePath << "  "; 
    return new Sprite(texturePath, &m_window);
}

void Engine::getScreenSize(int& w, int& h) {
    w = m_window.getSize().x; h = m_window.getSize().y;
}
