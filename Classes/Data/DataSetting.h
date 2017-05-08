#ifndef __DATASETTING_H__
#define __DATASETTING_H__

#include "cocos2d.h"
#include "Constant.h"
#include "Language.h"

class DataSetting {

    DataSetting() = delete;
   ~DataSetting() = delete;

public:
    static void save();
    static void load();

    static std::vector<std::string> stringToVector(const std::string &source, char separator);

    struct UserData_t {

        private:
            UserData_t() = delete;
           ~UserData_t() = delete;

        public:
            static bool playingBackgroundMusic;
            static bool playingEffectSound;
            static std::string playerName;
            static std::string opponentName;
            static std::string NetworkPort;
            static std::string NetworkIp;
            static cocos2d::Color3B playerColor;
            static cocos2d::Color3B opponentColor;
            static GameType type;
            static Locale locale;

            /* tmp var's. not save. */
            static std::string tmpMyNetworkIp;
            static std::string tmpMyNetworkPort;
    };

    struct GameData_t {

        private:
            GameData_t() = delete;
           ~GameData_t() = delete;

        public:
            static GameMode mode;
            static int currentLevel;

    };

    static void reset();
    static void set(const std::vector<std::string> &source);

    static std::string toString();

};


typedef DataSetting::UserData_t UserData;
typedef DataSetting::GameData_t GameData;

#endif // __DATASETTING_H__
