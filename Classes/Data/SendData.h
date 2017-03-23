#ifndef __SENDDATA_H__
#define __SENDDATA_H__

#include "cocos2d.h"

#include <string>

#include "DataSetting.h"

class ExperimentalSendData {

public:
    enum class TypeData : char {
        NAME_PLAYER = ' ',
        NAME_OPPONENT,
        POS_PLAYER,
        POS_OPPONENT,
        POS_EAT,
        POS_BONUS,
        IS_VISIBLE_BONUS,
        SCORE_PLAYER,
        SCORE_OPPONENT,
        CURRENT_TIME,
        DIRECT_TIME,
        TYPE_SHADER
    };

public:
    static ExperimentalSendData *getInst() {
        static auto *pointer = new ExperimentalSendData;
        return pointer;
    }

    typedef std::string string;

    class Dat : public std::map<TypeData, string> {

    public:
        Dat(){}
        Dat(const std::string &source) {
            auto container = std::move(DataSetting::stringToVector(source, '\n'));
            for (const std::string &i : container) {
                this->operator []((TypeData)((char)i[0])) = i.substr(1);
            }
        }

    public:
        void inline set(ExperimentalSendData::TypeData const &key, string const &value) {
            this->operator [](key) = value;
        }

        void inline set(ExperimentalSendData::TypeData const &key, cocos2d::Vec2 const &value) {
            this->operator [](key) = cocos2d::StringUtils::toString(value.x) + ' ' + cocos2d::StringUtils::toString(value.y);
        }

        template <typename Number>
        void inline set(ExperimentalSendData::TypeData const &key, const Number &value) {
            this->operator [](key) = cocos2d::StringUtils::toString(value);
        }

        static std::string charToStr(char ch);

        string toStr() const;

    } dat;

    string toStr() const;

    cocos2d::Vec2 toVec2(const string &value);


};



class SendData
{
private:
    SendData() = delete;
   ~SendData() = delete;

public:
    static std::string namePlayer;
    static std::string nameOpponent;
    static cocos2d::Vec2 posPlayer;
    static cocos2d::Vec2 posOpponent;
    static cocos2d::Vec2 posEat;
    static cocos2d::Vec2 posBonus;
    static bool visibleBonus;
    static int scorePlayer;
    static int scoreOpponent;
    static int currentTime;
    static int directTime;
    static int typeShader;

public:
    std::string toString();

};

#endif // __SENDDATA_H__
