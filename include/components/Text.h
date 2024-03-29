#pragma once
#include "Camera.h"
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * \brief A simple UI text Component (good with UISliceRenderer)
 * Don't forget to use a UICanvas
 */
class Text : public Component {
public:
  Text() : Component("Text"){};

  void start() override {
    text = "";
    fontSize = original_font_size;
    changeFont(mfontFile, original_font_size);
    entity->layer = 70;
    entity->useLayer = true;
    typeIsRendering = true;
  }

  void update(float deltaTime) override {
    if (text.length() <= 0)
      return;
    // if (fontFile == "") return;
    if (fontSize == 0)
      return;

    SDL_Rect spriteRect = {0, 0, 0, 0};

    float width_scale =
        (float)GameManager::currentWindowSize.x / GameManager::gameWindowSize.x;
    float height_scale =
        (float)GameManager::currentWindowSize.y / GameManager::gameWindowSize.y;

    Vector2f renderPos = entity->require<entityBox>()->getPosition() +
                         GameManager::gameWindowSize / 2;
    spriteRect.x = renderPos.x; //+ GameManager::camera.getCenter().x;
    spriteRect.y = renderPos.y; //+ GameManager::camera.getCenter().y;

    spriteRect.w = entity->require<entityBox>()->getSize().x;
    spriteRect.h = entity->require<entityBox>()->getSize().y;

    renderTextInRect(GameManager::renderer, text, spriteRect);
  }

  void onScreenChange() override { changeFont(mfontFile, fontSize); }

  /**
   * \brief Sets the font and text size,
   * This MUST be called for text to work
   * \param fontFile A filepath to a ttf font
   * \param size Fontsize
   */
  void changeFont(std::string fontFile, int size) {
    fontSize = size;
    mfontFile = fontFile;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
  }

  /**
   * \brief Renders the text (is called in updateloop don't worry about this)
   */
  void renderTextInRect(SDL_Renderer *renderer, const std::string &text,
                        const SDL_Rect &rect) {
    // Create a surface from the text
    TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);
    SDL_Surface *textSurface =
        TTF_RenderText_Blended_Wrapped(font, text.c_str(), text_color, rect.w);

    if (textSurface == nullptr) {
      std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;
      return;
    }

    // Create a texture from the surface
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textTexture == nullptr) {
      std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
      SDL_FreeSurface(textSurface);
      return;
    }

    // Calculate the destination rectangle to center the text
    SDL_Rect dstRect;
    dstRect.w = textSurface->w;
    dstRect.h = textSurface->h;
    dstRect.x = rect.x + (rect.w - textSurface->w) / 2;
    dstRect.y = rect.y + (rect.h - textSurface->h) / 2;

    // Render the texture to the renderer
    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

    // Clean up resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
  }

  Box textBox;

  std::string text;

  /**
   * \brief Sets the text color
   */
  SDL_Color text_color = {0, 0, 0};
  int original_font_size = 20;
  int fontSize;
  std::string mfontFile = "";
  TTF_Font *font = TTF_OpenFont(mfontFile.c_str(), fontSize);
  SDL_Color textColor;
};
