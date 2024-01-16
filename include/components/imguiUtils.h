#pragma once
#include <algorithm>
#include <string>

#include "SDL_render.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Charlie2D.h"

void TextCentered(std::string text);

class SimpleImGuiPanel : public Component {
public:
  void start() override;

  void update(float deltaTime) override;

  void setColor(std::array<Uint8, 3> color, float alpha = 1);
  void changeFont(std::string fontFile, int size);

  std::string text = "";
  ImVec4 frameColor = ImVec4(1, 1, 1, 1);
  SDL_Texture* renderingTexture;
};
