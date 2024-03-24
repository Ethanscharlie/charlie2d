#include "imguiUtils.h"
#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"
#include "GameManager.h"
#include "Math.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "Text.h"
#include "Vector2f.h"
#include <string>


void TextCentered(std::string text) {
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

  // Calculate the indentations that centers the text, horizontal and vertical
  float horiz_indent = (windowSize.x - textSize.x) * 0.5f;
  float vert_indent = (windowSize.y - textSize.y) * 0.5f;

  // If the text is too long to be drawn on a line, `horiz_indent` can
  // become too small or even negative, so we set a minimum indentation
  float min_indent = 20.0f;
  if (horiz_indent <= min_indent) {
    horiz_indent = min_indent;
  }

  // This prevents vertical indentation from becoming negative
  if (vert_indent < 0) {
    vert_indent = 0;
  }

  // Set the cursor position and draw the text
  ImGui::SetCursorPosX(horiz_indent);
  ImGui::SetCursorPosY(vert_indent);
  ImGui::TextUnformatted(text.c_str());
}

void SimpleImGuiPanel::start() {
  entity->useLayer = true;
  entity->layer = 80;
  typeIsRendering = true;
}

void SimpleImGuiPanel::update(float deltaTime) {
  SDL_RenderSetLogicalSize(GameManager::renderer,
                           GameManager::currentWindowSize.x,
                           GameManager::currentWindowSize.y);

  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();

  ImGui::NewFrame();
  Vector2f framePosition =
      getWindowPosition(entity->get<entityBox>()->getPosition() +
                        GameManager::gameWindowSize / 2);

  ImGui::SetNextWindowPos(ImVec2(framePosition.x, framePosition.y),
                          ImGuiCond_None);
  ImGui::SetNextWindowSize({entity->get<entityBox>()->getSize().x * 1,
                            entity->get<entityBox>()->getSize().y * 1},
                           ImGuiCond_Always);

  ImGui::PushStyleColor(ImGuiCol_WindowBg,
                        frameColor); // replace with your color

  ImGui::Begin(std::to_string(entity->iid).c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  // TextCentered(text);

  // addToPanel();

  ImGui::End();
  ImGui::PopStyleColor();

  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

  // if (entity->checkComponent<Text>()) {
  //   entity->get<Text>()->update(deltaTime);
  // }

  SDL_Rect rect = getLogicalRect();

  SDL_RenderSetLogicalSize(GameManager::renderer, GameManager::gameWindowSize.x,
                           GameManager::gameWindowSize.y);
}

void SimpleImGuiPanel::setColor(std::array<Uint8, 3> color, float alpha) {
  frameColor.x = color[0] / 255.0f;
  frameColor.y = color[1] / 255.0f;
  frameColor.z = color[2] / 255.0f;
  frameColor.w = alpha;
}

void SimpleImGuiPanel::changeFont(std::string fontFile, int size) {}
