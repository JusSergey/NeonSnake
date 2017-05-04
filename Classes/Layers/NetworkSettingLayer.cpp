#include "NetworkSettingLayer.h"
#include "Data/DataSetting.h"
#include "Constant.h"
#include "Jus.h"

USING_NS_CC;

#define offset (Vec2(Jus::getWidth()*3, 0))
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

    localIpAddress = "No local address";

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    initLabels();
    initNavigation();
    initTitle();
    initDrawNode();
    initTextFields();
    initLabelLocalInfo();

    return true;
}

void NetworkSettingLayer::setCallbackStart(const std::function<void(Ref *)> &call)
{
    itemStart->setCallback([=](Ref *ref){
        UserData::NetworkIp = fieldIpToServer->getString();
        UserData::NetworkPort = fieldServerPort->getString();
        DataSetting::save();
        call(ref);
    });
}

void NetworkSettingLayer::setCallbackBack(const std::function<void(Ref *)> &call)
{
    itemBack->setCallback([=](Ref *ref){
        UserData::NetworkIp = fieldIpToServer->getString();
        UserData::NetworkPort = fieldServerPort->getString();
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
            DataSetting::save();
            call(ref);
            StartServer = !StartServer;
            ItemServerStart->setString("server: " + std::string(StartServer ? "Yes" : "No"));
        });
}

Label *NetworkSettingLayer::createLabel(const std::string &text, float fontSize)
{
    return Jus::createLabelSystem(text, "monospace", fontSize);
}

void NetworkSettingLayer::initLabelLocalInfo()
{
    std::string filename = "localinfo.log";
    std::string path = FileUtils::getInstance()->getWritablePath();

    std::string commandIfconfig = "ifconfig | grep \"inet addr:192.168.\" > ";
    system((commandIfconfig + path + filename).c_str());

    Data data = FileUtils::getInstance()->getDataFromFile(path + filename);

    if(!data.isNull()){

        std::string info = (const char *)data.getBytes();
        auto posBegin = info.find(':');
        auto posEnd   = std::string(info.c_str() + posBegin).find(' ');

        if (posBegin != std::string::npos && posEnd != std::string::npos) {
            localIpAddress = info.substr(posBegin+1, posEnd);
        }
    }

    std::string commandRm = "rm ";
    system((commandRm + path + filename).c_str());

    Label *labelLocalIpAddserr = Label::createWithSystemFont((std::string("Your local address\n") + localIpAddress).c_str(), "monospace", fsize);
    labelLocalIpAddserr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelLocalIpAddserr->setPosition(offset + Jus::getDisplayPoint(0.5, 0.1));
    addChild(labelLocalIpAddserr, 2);

}

void NetworkSettingLayer::initTitle()
{
    auto l = Jus::createLabelSystem("LAN", "monospace", 42);
    l->setPosition(Jus::getDisplayPoint(0.5, 0.9) + offset);
    addChild(l, 2);

}

void NetworkSettingLayer::initLabels()
{
    labelIp   = Jus::createLabelSystem("IP: ", "monospace", 42);
    labelPort = Jus::createLabelSystem("PORT: ", "monospace", 42);

    labelIp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    labelPort->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

    labelIp->setPosition(Vec2(visibleSize.width/2.5, visibleSize.height/2 + labelIp->getContentSize().height) + offset);
    labelPort->setPosition(Vec2(visibleSize.width/2.5, visibleSize.height/2 - labelPort->getContentSize().height) + offset);

    log("InitLabels.");

    addChild(labelIp, 2);
    addChild(labelPort, 2);
}

void NetworkSettingLayer::initTextFields()
{
    fieldIpToServer = ui::TextField::create("ip to server","monospace", 34 / _director->getContentScaleFactor());
    fieldServerPort = ui::TextField::create("server port", "monospace", 34 / _director->getContentScaleFactor());

    fieldIpToServer->setString(UserData::NetworkIp);
    fieldServerPort->setString(UserData::NetworkPort);

    fieldIpToServer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    fieldServerPort->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    fieldServerPort->setPosition(labelPort->getPosition());
    fieldIpToServer->setPosition(labelIp->getPosition());

    addChild(fieldServerPort, 2);
    addChild(fieldIpToServer, 2);

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

}

void NetworkSettingLayer::initNavigation()
{
    Label* lback = Jus::createLabelTTF("<-Back", "fonts/Bicubik.ttf", 36);
    lback->setAdditionalKerning(3);

    Label* lnext = Jus::createLabelTTF("Start", "fonts/Bicubik.ttf", 36);
    lnext->setAdditionalKerning(3);

    Label* lSServer = Jus::createLabelTTF("server: No", "fonts/Bicubik.ttf", 36);

    itemBack = MenuItemLabel::create(lback);
    itemStart = MenuItemLabel::create(lnext);
    ItemServerStart = MenuItemLabel::create(lSServer);

    itemBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemStart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    ItemServerStart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    itemBack->setPosition(offset + Vec2( 32, 16) - Jus::getCenter());
    itemStart->setPosition(offset + Vec2(-32, 16) - Jus::getCenter() + Vec2(Jus::getWidth(), 0));
    ItemServerStart->setPosition(labelPort->getPosition() - Vec2(0, labelIp->getPositionY() - labelPort->getPositionY()) - visibleSize/2);

    auto menu = Menu::create(itemBack, itemStart, ItemServerStart, nullptr);

    addChild(menu, LLayer);
}
