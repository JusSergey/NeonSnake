#include "NetworkSettingLayer.h"
#include "Data/DataSetting.h"
#include "Constant.h"
#include "Jus.h"

USING_NS_CC;

#define offset (Vec2(0, Jus::getHeight()))
#define rectX1 (visibleSize.width * 0.25)
#define rectX2 (visibleSize.width * 0.75)

#define fsize 24

// on "init" you need to initialize your instance
bool NetworkSettingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if(!Layer::init())
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    log("init fields");
    initTextFields();
    log("init navig");
    initNavigation();
    log("init labels");
    initLabels();
    log("init dnode");
    initDrawNode();


    return true;
}

void NetworkSettingLayer::setCallbackNext(const std::function<void(Ref *)> &call)
{
    itemNext->setCallback([=](Ref *ref){
//        UserData::NetworkIp = fieldIp->getString();
//        UserData::NetworkPort = fieldPort->getString();
//        UserData::tmpMyNetworkPort = fieldMyPort->getString();
        UserData::NetworkIp = fieldIpToServer->getString();
        UserData::NetworkPort = fieldServerPort->getString();
        UserData::tmpMyNetworkPort = fieldClientPort->getString();
        DataSetting::save();
        call(ref);
    });
}

void NetworkSettingLayer::setCallbackBack(const std::function<void(Ref *)> &call)
{
    itemBack->setCallback([=](Ref *ref){
        UserData::NetworkIp = fieldIpToServer->getString();
        UserData::NetworkPort = fieldServerPort->getString();
        UserData::tmpMyNetworkPort = fieldClientPort->getString();
        DataSetting::save();
        call(ref);
    });
}

void NetworkSettingLayer::setCallbackStartServer(const std::function<void (Ref *)> &call)
{
    if (ItemServerStart)
        ItemServerStart->setCallback([=](Ref* ref){
            UserData::NetworkIp = fieldIpToServer->getString();
            UserData::NetworkPort = fieldServerPort->getString();
            UserData::tmpMyNetworkPort = fieldClientPort->getString();
            DataSetting::save();
            call(ref);
        });
}

Label *NetworkSettingLayer::createLabel(const std::string &text, float fontSize)
{
    return Label::createWithSystemFont(text, "monospace", fontSize);
}

void NetworkSettingLayer::initLabels()
{
    auto l = Label::createWithSystemFont("Server", "monospace", 42);
    l->setPosition(Jus::getDisplayPoint(0.5, 0.9) + offset);
    addChild(l, 2);
//    labelIp = createLabel("IP", fsize);
//    labelPort = createLabel("PORT", fsize);

//    labelIp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    labelPort->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

//    labelIp  ->setPosition(rectX1 + 16, Jus::getDisplayPoint(0, 1.55).y);
//    labelPort->setPosition(rectX1 + 16, Jus::getDisplayPoint(0, 1.45).y);

//    addChild(labelIp,   1);
//    addChild(labelPort, 1);
}

void NetworkSettingLayer::initTextFields()
{
    fieldClientPort = ui::TextField::create("client port", "monospace", 34);
    fieldIpToServer = ui::TextField::create("ip to server","monospace", 34);
    fieldServerPort = ui::TextField::create("server port", "monospace", 34);

    fieldIpToServer->setString("127.0.0.1");
    fieldServerPort->setString("2121");

    fieldServerPort->setPosition(Jus::getDisplayPoint(0.5, 0.7) + offset);
    fieldIpToServer->setPosition(Jus::getDisplayPoint(0.5, 0.4) + offset);
    fieldClientPort->setPosition(Jus::getDisplayPoint(0.5, 0.2) + offset);

    addChild(fieldServerPort, 2);
    addChild(fieldIpToServer, 2);
    addChild(fieldClientPort, 2);

//    fieldIp = ui::TextField::create("ip address", "monospace", fsize);
//    fieldPort = ui::TextField::create("port address", "monospace", fsize);
//    fieldMyPort = ui::TextField::create("my port", "monospace", fsize);

//    fieldIp->setString(UserData::NetworkIp);
//    fieldPort->setString(UserData::NetworkPort);

//    fieldIp  ->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    fieldPort->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    fieldMyPort->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

//    fieldIp  ->setPosition({labelPort->getPositionX() + labelPort->getContentSize().width * 1.5f, labelIp->getPositionY()});
//    fieldPort->setPosition({labelPort->getPositionX() + labelPort->getContentSize().width * 1.5f, labelPort->getPositionY()});
//    fieldMyPort->setPosition({fieldIp->getPositionX(), fieldPort->getPositionY() - fieldMyPort->getContentSize().height*2});

//    addChild(fieldIp, 2);
//    addChild(fieldPort, 2);
//    addChild(fieldMyPort, 2);

}

void NetworkSettingLayer::initDrawNode()
{
    auto node = DrawNode::create(5);
    node->setPosition(offset);
    addChild(node);

    node->drawSolidRect(Vec2(rectX1, 0),
                        Vec2(rectX2, visibleSize.height),
                        Color4F(0, 0, 0, 0.7));

    node->drawSolidRect(Vec2(rectX1, visibleSize.height * 0.25),
                        Vec2(rectX2, visibleSize.height * 0.75),
                        Color4F(0, 0, 0, 0.4));

//    float procentX = visibleSize.width / 100;
//    float procentY = visibleSize.height/ 100;

//    float off = labelPort->getContentSize().width;

//    // draw edit line port
//    node->drawSolidRect(Vec2(rectX1 + off, labelPort->getPositionY()),
//                        Vec2(rectX2 - procentX*2, labelPort->getContentSize().height),
//                        Color4F(0.2, 0, 0, 0.7));

}

void NetworkSettingLayer::initNavigation()
{
    Label* lback = Label::createWithTTF("<-Back", "fonts/Bicubik.ttf", 36);
    lback->setAdditionalKerning(3);

    Label* lnext = Label::createWithTTF("Next", "fonts/Bicubik.ttf", 36);
    lnext->setAdditionalKerning(3);

    Label* lSServer = Label::createWithTTF("start serv.", "fonts/Bicubik.ttf", 36);

    itemBack = MenuItemLabel::create(lback);
    itemNext = MenuItemLabel::create(lnext);
    ItemServerStart = MenuItemLabel::create(lSServer);

    itemBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemNext->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    ItemServerStart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    itemBack->setPosition(offset + Vec2( 32, 16) - Jus::getCenter());
    itemNext->setPosition(offset + Vec2(-32, 16) - Jus::getCenter() + Vec2(Jus::getWidth(), 0));
    ItemServerStart->setPosition(offset);

    auto menu = Menu::create(itemBack, itemNext, ItemServerStart, nullptr);

    addChild(menu, LLayer);
}
