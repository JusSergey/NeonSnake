#include "GameClient.h"
#include "Data/DataSetting.h"
#include <iostream>

USING_NS_CC;

#define npos std::string::npos

GameClient::GameClient(const std::string &ip, const std::string &port) :
    TcpClient(ip, (u_short)Value(port).asInt()),
    callbackRead([](ExperimentalSendData::Dat){})
{
}

void GameClient::read()
{

    if (statusThread)
        callbackRead(ExperimentalSendData::Dat(buffer));

//    PlayerData pdata;

//    log("readmsg: %s", buffer);

//    for (std::string iterator : DataSetting::stringToVector(buffer, '\n')) {

//        auto strArray = DataSetting::stringToVector(iterator, ' ');

//        if (strArray[0][0] == TypeData::Snake){
//            if (strArray[0].substr(1, npos) != UserData::playerName)
//                pdata.opponentPos = PlayerData::stringToVec2(strArray[1] + ' ' + strArray[2]);

//        }

//        else if (strArray[0][0] == TypeData::Eat)
//            pdata.eatPos = PlayerData::stringToVec2(strArray[1] + ' ' + strArray[2]);

//    }

//    if (statusThread)
//        callbackRead(pdata);

}

void GameClient::setCallbackRead(const std::function<void (ExperimentalSendData::Dat)> &value)
{
    callbackRead = value;
}
