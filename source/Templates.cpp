#include "Templates.h"
#include "imguiUtils.h"

Entity *Templates::createUIPanel(Entity *canvas, Box setBox, std::string text,
                                 std::string tag) {
  Entity *entity = GameManager::createEntity(tag);
  entity->setParent(canvas);
  entityBox *box = entity->add<entityBox>();
  box->anchor = 4;
  box->setSize(setBox.size);
  box->setLocalWithCenter(setBox.position);
  entity->add<SimpleImGuiPanel>();
  entity->add<Text>()->changeFont("img/fonts/prstart.ttf", 30);
  entity->get<Text>()->text = text;
  return entity;
}
