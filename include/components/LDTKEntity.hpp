#pragma once
#include <nlohmann/json.hpp>
// #include "Component.hpp"
//
using json = nlohmann::json;
//
// struct LDTKEntityValue {
//   std::string identifier;
//   std::string type;
//   json value;
// };
//
// /**
//  * \brief A simple Component container for LDTK entity json data
//  */
// class LDTKEntity : public Component {
// public:
//   LDTKEntity() {}
//
//   void start() override {}
//
//   void update(float deltaTime) override {}
//
//   void onDestroy() override;
//
//   /**
//    * \brief A nlohmann json file from ldtk for this entity
//    */
//   json entityJson;
// };
