#include "PlayerData.h"
#include "Data/DataSetting.h"

USING_NS_CC;

void PlayerData::operator =(const std::string &buff)
{
    auto strings = DataSetting::stringToVector(buff, ' ');
    opponentPos.x = Value(strings[0]).asFloat();
    opponentPos.y = Value(strings[1]).asFloat();
}

void PlayerData::operator =(const char *buff)
{
    this->operator =(std::string(buff));
}

PlayerData::PlayerData(const std::string &source)
{
    *this = source;
}

PlayerData::PlayerData(const char *data)
{
    *this = std::string(data);
}

std::string PlayerData::toString() const
{
    std::string result;
    result += StringUtils::toString(opponentPos.x) + ' ';
    result += StringUtils::toString(opponentPos.y);
    return result;
}

Vec2 PlayerData::stringToVec2(const std::string &str)
{
    auto vertx = DataSetting::stringToVector(str, ' ');
    return Vec2(Value(vertx[0]).asFloat(), Value(vertx[1]).asFloat());
}
