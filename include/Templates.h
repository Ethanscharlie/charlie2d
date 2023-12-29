#pragma once

#include "Animation.h"
#include "Camera.h"
#include "Component.h"
#include "Entity.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Math.h"
#include "ResourceManager.h"
#include "Scene.h"

// Components
#include "CircleCollider.h"
#include "Collider.h"
#include "EntityBox.h"
#include "JumpMan.h"
#include "Sprite.h"
#include "physicsBody.h"

#include "ldtk.h"

// #include "Tile.h"

#include "Button.h"
#include "Text.h"
#include "UICanvas.h"
#include "UISliceRenderer.h"

namespace Templates {

Entity *createUIPanel(Entity *canvas, Box setBox, std::string text,
                      std::string tag = "UIPanel");
} // namespace Templates
