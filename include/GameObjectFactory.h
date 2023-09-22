//#pragma once
//#include "GameObject.h"
//
//class GameObjectFactory
//{
//    //jusing GameObjectCreator = std::function<GameObject*()>;
//    std::map<std::string, std::type_index> creators;
//
//public:
//    template <typename T>
//    void Register(const std::string& name)
//    {
//        creators[name] = typeid(T);
//    }
//
//    std::type_index Create(const std::string& name)
//    {
//        auto it = creators.find(name);
//        if (it != creators.end())
//        {
//            std::type_index creator = it->second;
//            return creator;
//        }
//        //return nullptr;
//    }
//};
