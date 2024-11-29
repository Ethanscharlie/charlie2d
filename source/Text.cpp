#include "Text.hpp"

void Text::start() { entity->useLayer = true; }

void Text::update(float deltaTime) {
  if (text.length() <= 0)
    return;
  // if (fontFile == "") return;
  if (font.size == 0)
    return;

  SDL_Rect spriteRect = {0, 0, 0, 0};

  float width_scale =
      (float)GameManager::currentWindowSize.x / GameManager::gameWindowSize.x;
  float height_scale =
      (float)GameManager::currentWindowSize.y / GameManager::gameWindowSize.y;

  SDL_Rect renderRect = getRenderBox(entity);

  // spriteRect.w = entity->box.size.x;
  // spriteRect.h = entity->box.size.y;

  renderTextInRect(GameManager::renderer, text, renderRect);
}

// void onScreenChange()  { changeFont(mfontFile, fontSize); }

/**
 * \brief Sets the font and text size,
 * This MUST be called for text to work
 * \param fontFile A filepath to a ttf font
 * \param size Fontsize
 */
void Text::changeFont(std::string fontFile, int size) {
  // fontSize = size;
  // mfontFile = fontFile;
  // font = TTF_OpenFont(fontFile.c_str(), fontSize);
  font.changeFont(fontFile, size);
}

/**
 * \brief Renders the text (is called in updateloop don't worry about this)
 */
void Text::renderTextInRect(SDL_Renderer *renderer, const std::string &text,
                            const SDL_Rect &rect) {
  // Create a surface from the text
  TTF_SetFontWrappedAlign(font.font, TTF_WRAPPED_ALIGN_CENTER);
  SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(
      font.font, text.c_str(), text_color, rect.w);

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
