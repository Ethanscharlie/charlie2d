#include "UICanvas.h"

UICanvas::UICanvas() : UIElement({0,0,0,0})
{   
    text = "";
}


UICanvas::~UICanvas()
{

}

void UICanvas::startBase()
{
    UIElement::startBase();
    loadTexture("");
    setCentered();
   // ignore_screen_resize = true;
}

void UICanvas::update(float deltaTime) 
{
    float scaler = (float) SceneManager::windowWidth / SceneManager::originalWidth / SceneManager::screen_change_scale;
    size.x = SceneManager::originalWidth  * scaler; 
    size.y = SceneManager::originalHeight * scaler;
}

