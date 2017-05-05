#include "DataSetting.h"

USING_NS_CC;

bool DataSetting::UserData_t::playingBackgroundMusic = false;
bool DataSetting::UserData_t::playingEffectSound = false;

int DataSetting::GameData_t::currentLevel = 1;

std::string DataSetting::UserData_t::playerName = "Player";
std::string DataSetting::UserData_t::opponentName = "Bot";
std::string DataSetting::UserData_t::NetworkIp = "127.0.0.1";
std::string DataSetting::UserData_t::NetworkPort = "2324";

/* tmp */
std::string DataSetting::UserData_t::tmpMyNetworkIp = "127.0.0.1";
std::string DataSetting::UserData_t::tmpMyNetworkPort = "2327";

Color3B DataSetting::UserData_t::playerColor = Color3B::BLUE;
Color3B DataSetting::UserData_t::opponentColor = Color3B::RED;

GameMode DataSetting::GameData_t::mode = GameMode::Classic;

GameType DataSetting::UserData_t::type = GameType::PlayerVSBot;

// not working on android
// const std::string filesetting  = FileUtils::getInstance()->sharedFileUtils()->getWritablePath() + "/setting_neon_snake.ini";

inline const std::string& getPathSetting() {
    static const std::string path = FileUtils::getInstance()->sharedFileUtils()->getWritablePath() + "/setting_neon_snake.ini";
    return path;
}

std::string color3BToString(const Color3B &color) {
    std::string result;
    result += StringUtils::toString((unsigned int)color.r) + ' ';
    result += StringUtils::toString((unsigned int)color.g) + ' ';
    result += StringUtils::toString((unsigned int)color.b) + '\n';
    return result;
}

Color3B stringToColor3B(const std::string &str) {

    auto rgb = DataSetting::stringToVector(str, ' ');

    GLubyte r = Value(rgb[0]).asByte();
    GLubyte g = Value(rgb[1]).asByte();
    GLubyte b = Value(rgb[2]).asByte();

    return Color3B(r, g, b);
}

void DataSetting::save()
{
    FileUtils::getInstance()->writeStringToFile(DataSetting::toString(), getPathSetting());
}

void DataSetting::load()
{
    Data data = FileUtils::getInstance()->getDataFromFile(getPathSetting());

    if (data.isNull()) {
        DataSetting::reset();
        save();
    }

    else DataSetting::set(stringToVector((const char*)data.getBytes(), '\n'));

    log("print load data: %s", toString().c_str());

}

std::vector<std::string> DataSetting::stringToVector(const std::string &source, char separator)
{
    std::vector<std::string> result;

    std::string tmpStr;

    for (const char &i : source) {

        if (i == separator) {

            if (!tmpStr.empty())
                result.push_back(tmpStr);

            tmpStr.clear();
        }

        else tmpStr += i;

    }

    if (!tmpStr.empty())
        result.push_back(tmpStr);

    return result;
}

void DataSetting::reset() {

    UserData_t::opponentColor = Color3B::RED;
    UserData_t::playerColor = Color3B::BLUE;
    UserData_t::playerName = "Player";
    UserData_t::opponentName = "Bot";
    UserData_t::NetworkIp = "127.0.0.1";
    UserData_t::NetworkPort = "2324";
    UserData_t::playingBackgroundMusic =
    UserData_t::playingEffectSound =
    false;

    GameData_t::mode = GameMode::Classic;
    UserData_t::type = GameType::PlayerVSBot;

    GameData_t::currentLevel = 1;

    save();
}

void DataSetting::set(const std::vector<std::string> &source) {

    if (source.size() < 11/*must be size vector*/) {
        log("SOUNCE SIZE: %d", source.size());
        reset();
        return;
    }

    UserData_t::opponentColor = stringToColor3B(source[0]);
    UserData_t::playerColor = stringToColor3B(source[1]);

    UserData_t::opponentName = source[2].empty()? "Player" : source[2];
    UserData_t::playerName = source[3].empty()? "Bot" : source[3];
    UserData_t::NetworkIp = source[4];
    UserData_t::NetworkPort = source[5];
    UserData_t::playingBackgroundMusic = (source[6][0] == '1' ? true : false);
    UserData_t::playingEffectSound     = (source[7][0] == '1' ? true : false);

    GameData_t::mode = (GameMode)Value(source[8]).asInt();
    UserData_t::type = (GameType)Value(source[9]).asInt();

    GameData_t::currentLevel = Value(source[10]).asInt();
}

std::string DataSetting::toString() {

    std::string result;

    result += color3BToString(UserData_t::opponentColor);

    result += color3BToString(UserData_t::playerColor);

    result += UserData_t::opponentName + '\n';

    result += UserData_t::playerName + '\n';

    result += UserData_t::NetworkIp + '\n';

    result += UserData_t::NetworkPort + '\n';

    if (UserData_t::playingBackgroundMusic) result += "1\n";
    else result += "0\n";

    if (UserData_t::playingEffectSound) result += "1\n";
    else result += "0\n";

    result += StringUtils::toString((int)GameData_t::mode) + '\n';
    result += StringUtils::toString((int)UserData_t::type) + '\n';

    result += StringUtils::toString(GameData_t::currentLevel) + '\n';

    return result + '\0';

}
