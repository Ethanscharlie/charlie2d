#pragma once
#include "Camera.h"
#include "Entity.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Serializer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

struct Font {
  Font(std::string _filepath, int _size) : filepath(_filepath), size(_size) {
    font = TTF_OpenFont(_filepath.c_str(), _size);
  }

  void changeFont(std::string fontFile, int size) {
    size = size;
    filepath = fontFile;
    font = TTF_OpenFont(fontFile.c_str(), size);
  }

  std::string filepath = "";
  int size = 0;
  TTF_Font *font = nullptr; // TTF_OpenFont(mfontFile.c_str(), fontSize);
};

/**
 * \brief A simple UI text Component (good with UISliceRenderer)
 * Don't forget to use a UICanvas
 */
class Text : public Component {
public:
  Text() {
    propertyRegister = {GET_PROP(text), GET_PROP(text_color), GET_PROP(font),
                        GET_PROP(renderInWorld)};

    typeIsRendering = true;
  };

  void start() override;

  void update(float deltaTime) override;

  /**
   * \brief Sets the font and text size,
   * This MUST be called for text to work
   * \param fontFile A filepath to a ttf font
   * \param size Fontsize
   */
  void changeFont(std::string fontFile, int size);
  /**
   * \brief Renders the text (is called in updateloop don't worry about this)
   */
  void renderTextInRect(SDL_Renderer *renderer, const std::string &text,
                        const SDL_Rect &rect);

  Box textBox;

  std::string text = "Sample Text";

  /**
   * \brief Sets the text color
   */
  SDL_Color text_color = {255, 255, 0};
  // int original_font_size = 20;
  // int fontSize;
  // std::string mfontFile = "";
  Font font = {"", 20};
  // SDL_Color textColor;
  bool renderInWorld = false;
};
REGISTER_COMPONENT_TYPE(Text);
