#pragma once
#include "Charlie2D.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class LDTKEntity : public Component {
    public:
    LDTKEntity() {}

    void start() override {

    }

    void update(float deltaTime) override {
        
    }

    json entityJson;
};
