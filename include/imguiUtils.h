#include <algorithm>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Charlie2D.h"

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

class SimpleImGuiPanel : public Component {
public:
  void start() override {
    entity->useLayer = true;
    entity->layer = 80;

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("img/fonts/prstart.ttf", 40);
  }

  void update(float deltaTime) override {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(GameManager::window);

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(entity->get<entityBox>()->getPosition().x +
                                       GameManager::gameWindowSize.x / 2,
                                   entity->get<entityBox>()->getPosition().y +
                                       GameManager::gameWindowSize.y / 2),
                            ImGuiCond_None);
    ImGui::SetNextWindowSize({entity->get<entityBox>()->getSize().x,
                              entity->get<entityBox>()->getSize().y},
                             ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          frameColor); // replace with your color

    ImGui::Begin(std::to_string(entity->iid).c_str(), nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    TextCentered(text);

    ImGui::End();
    ImGui::PopStyleColor();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }

  void setColor(std::array<Uint8, 3> color, float alpha = 1) {
    frameColor.x = color[0] / 255.0f;
    frameColor.y = color[1] / 255.0f;
    frameColor.z = color[2] / 255.0f;
    frameColor.w = alpha;
  }

  std::string text = "";
  ImVec4 frameColor = ImVec4(1, 1, 1, 1);
};

