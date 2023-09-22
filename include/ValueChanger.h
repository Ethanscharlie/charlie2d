//#include "UIPanel.h"
//#include "GameObject.h"
//#include "SceneManager.h"
//
//class ArrowButton : public UIPanel
//{
//    public:
//    ArrowButton(int typeIn) : UIPanel({0, 0, 30, 30, "img/panel.png"}) {type = typeIn;}
//    onClick() override 
//    {
//        ValueChanger* valuechanger = dynamic_cast<ValueChanger*>(parent);
//        if (type == 0)
//        {
//            valuechanger->OnLeftButtonClick();
//        }
//    }
//    int type;
//};
//
//class ValueChanger : public GameObject
//{
//    public:
//    ValueChanger() {}
//    virtual void start() override 
//    {
//        leftButton = new ArrowButton(0)
//        valuePanel = new UIPanel({30, 30, 30, 30}, "img/panel.png");
//
//        scene->addToParent(leftButton, this);
//    }
//    virtual void update(float deltaTime) override {}
//
//    ArrowButton* leftButton;
//    UIPanel* valuePanel;
//    int value = 0;
//    //UIPanel* rightButton;
//
//    void OnLeftButtonClick()
//    {
//        value ++;
//        text = value;
//    }
//};
//
