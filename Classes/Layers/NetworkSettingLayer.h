#ifndef __NETWORKSETTINGLAYER_H__
#define __NETWORKSETTINGLAYER_H__

#include "cocos2d.h"
#include <functional>
#include "ui/CocosGUI.h"

class NetworkSettingLayer : public cocos2d::Layer
{

public:
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(NetworkSettingLayer);

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::MenuItemLabel *itemBack;
    cocos2d::MenuItemLabel *itemNext;

    // for client
    cocos2d::ui::TextField *fieldIpToServer;
    cocos2d::ui::TextField *fieldServerPort;

    cocos2d::MenuItemLabel *ItemServerStart;

    // labels
    cocos2d::Label* labelIp;
    cocos2d::Label* labelPort;

    std::string localIpAddress;

public:
    void setCallbackNext(const std::function<void(Ref*)> &call);
    void setCallbackBack(const std::function<void(Ref*)> &call);
    void setCallbackStartServer(const std::function<void(Ref*)> &call);

    cocos2d::Label *createLabel(const std::string &text, float fontSize);

private:
    void initLabelLocalInfo();
    void initTitle();
    void initLabels();
    void initTextFields();
    void initDrawNode();
    void initNavigation();

};

#endif // __NETWORKSETTINGLAYER_H__
