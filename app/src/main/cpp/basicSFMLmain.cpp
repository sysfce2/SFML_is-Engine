#include "isEngine/core/GameEngine.h"

namespace is
{
bool GameEngine::basicSFMLmain()
{
#if defined(__ANDROID__)
    m_window.create(sf::VideoMode::getDesktopMode(), "");
#else
    #if !defined(IS_ENGINE_HTML_5)
    m_window.create(sf::VideoMode(is::GameConfig::WINDOW_WIDTH,
                                  is::GameConfig::WINDOW_HEIGHT),
                                  is::GameConfig::GAME_NAME,
                                  is::getWindowStyle());

    // load application icon
    sf::Image iconTex;
    if (!iconTex.loadFromFile(is::GameConfig::GUI_DIR + "icon.png")) return false;
    m_window.setIcon(32, 32, iconTex.getPixelsPtr());
    #else
    m_window = sf::RenderWindow(is::GameConfig::WINDOW_WIDTH, is::GameConfig::WINDOW_HEIGHT, is::GameConfig::GAME_NAME);
    #endif // defined
#endif // defined

    sf::View m_view(m_window.getDefaultView());

    #if !defined(IS_ENGINE_HTML_5) // using the SFML library
    sf::Music music;
    if (!music.openFromFile(is::GameConfig::MUSIC_DIR + "game_music.ogg")) return false;
    music.play();
    sf::Texture texture;
    if (!texture.loadFromFile(is::GameConfig::GUI_DIR + "icon.png")) return false;
    #else                          // using the SMK library
    sf::SoundBuffer soundBuffer(is::GameConfig::MUSIC_DIR + "game_music.ogg");
    sf::Music music(soundBuffer);
    sf::Texture texture(is::GameConfig::GUI_DIR + "icon.png");
    #endif

    sf::Sprite image(texture);
    image.setPosition(m_view.getCenter().x / 2.f, m_view.getCenter().y / 2.f);
    image.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    bool focus = true; // Doesn't work when you're on the web version
    #if !defined(IS_ENGINE_HTML_5)
    while (m_window.isOpen())
    #else
    m_window.ExecuteMainLoop([&]
    #endif // defined
    {
        sf::Vector2i mousePosition(-1, -1); // Allows to get mouse or touch position
                                            // A negative value means that no position has been recorded

        #if !defined(IS_ENGINE_HTML_5) // using the SFML library
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    m_window.close();
                break;

                case sf::Event::Resized:
                    m_view.setSize(event.size.width, event.size.height);
                    m_view.setCenter(event.size.width / 2, event.size.height / 2);
                    m_window.setView(m_view);
                break;

                #if defined(__ANDROID__)
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0)
                    {
                        mousePosition.x = event.touch.x;
                        mousePosition.y = event.touch.y;
                        is::vibrate(100);
                    }
                break;
                #else
                case sf::Event::MouseButtonPressed:
                    mousePosition.x = sf::Mouse::getPosition(m_window).x;
                    mousePosition.y = sf::Mouse::getPosition(m_window).y;
                break;
                #endif // defined

                case sf::Event::LostFocus:
                focus = false;                    //don't draw, if the window is not shown
                is::showLog("LOST FOCUS!");
                break;

                case sf::Event::GainedFocus:
                focus = true;                    //draw if the window is shown
                is::showLog("GAINED FOCUS!");
                break;

                default: break;
            }
        }
        #else                          // using the SMK library
        m_window.PoolEvents(); // Allows to update events

        // With the SMK library, musics can only be played if they are in the render loop
        if (is::checkSFMLSndState(music, is::SFMLSndStatus::Stopped)) music.Play();

        // Get position on cursor released.
        if (m_window.input().IsCursorReleased())
        {
            mousePosition = sf::Mouse::getPosition(m_window);

            // Vibrate on cursor released
            // Works if and only if the platform supports the vibrator (Smart phone)
            is::vibrate(100);
        }
        #endif // defined

        if (mousePosition.x != -1 && mousePosition.y != -1) image.setPosition(mousePosition.x, mousePosition.y);
        if (focus)
        {
            is::clear(m_window, sf::Color::Blue);
            is::draw(m_window, image);
            is::display(m_window);
        }
    }
    #if defined(IS_ENGINE_HTML_5)
    );
    #endif // defined

    return true;
}
}
