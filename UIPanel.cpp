#include "UIPanel.h"

UIPanel::UIPanel(const SDL_Rect& rect, const std::string& imageIn) : UIElement(rect)
{   
    image = imageIn;
}

void UIPanel::startBase()
{
    UIElement::startBase();
    loadTexture(image);
}

void UIPanel::update(float deltaTime)
{
    
}

UIPanel::~UIPanel()
{
    // SDL_DestroyTexture(textTexture);
    // SDL_FreeSurface(textSurface);
    // TTF_CloseFont(font);
}
