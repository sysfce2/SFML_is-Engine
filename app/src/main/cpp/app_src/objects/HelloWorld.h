#pragma once
#include "../../isEngine/system/display/GameDisplay.h"

//////////////////////////////////////////////////////
/// \brief This class allows to scroll a "Hello world!"
/// on the screen and perform touch / click detection tests
//////////////////////////////////////////////////////
class HelloWorld : public is::MainObject
{
public:
    HelloWorld(float x, float y, is::GameDisplay *scene):
        is::MainObject(x ,y),
        m_scene(scene)
    {
        // Create the sprite of object and center it
        m_xOffset = 64.f; // set offset x to center the image
        m_yOffset = 65.f; // set offset y to center the image
        is::createSprite(scene->GRMgetTexture("hello_world"), m_sprParent, sf::Vector2f(m_x, m_y), sf::Vector2f(m_xOffset, m_yOffset));

        // Display depth of the object in the scene
        m_depth = -1;
    }

    void step(float const &DELTA_TIME)
    {
        // VALUE_CONVERSION : allows to avoid using large values to do operations
        // (basically it converts small values to large) and it also acts on the timing
        // of the program
        m_imageAngle += (is::VALUE_CONVERSION * 0.5f) * DELTA_TIME; // rotate the image

        // moved the object to the right
        m_x += (is::VALUE_CONVERSION * 2.f) * DELTA_TIME;

        // When you touch (on Android) or click on the sprite of the object it moves down
        if (m_scene->mouseCollision(m_sprParent) && m_scene->getGameSystem().isPressed())
            m_y += (is::VALUE_CONVERSION * 1.f) * DELTA_TIME;

        // If the object comes out to the right or at the top of the scene, bring it back
        // to its starting position
        // By default the size of the scene is equal to that of the window
        if (m_x > m_scene->getSceneWidth())  m_x = m_xStart - m_xOffset;
        if (m_y > m_scene->getSceneHeight()) m_y = m_yStart - m_yOffset;

        updateSprite(); // update the sprite with all object variables (x, y, angle, scale, ...)
    }

private:
    is::GameDisplay *m_scene;
};
