#include "cocos2d.h"
#include "Language.h"

#include <map>
using namespace std;

const string &Language::get(Locale locale, const string &text)
{
    switch (locale) {
    case Locale::ua: return get_ua(text);
    case Locale::en: return get_en(text);
    case Locale::ru: return get_ru(text);
    default: cocos2d::log("undefined locale. term."); std::terminate();
    }
}

const string &Language::get_ua(const string &text)
{
    static map<string, string> dictionary {
        //MenuScene
        make_pair("Classic",           "Класична"),
        make_pair("Survival",          "Виживання"),
        make_pair("Local",             "Локальна"),
        make_pair("Exit",              "Вихід"),
        make_pair("Player",            "Гравець"),
        make_pair("Color Player",      "Колір гравця"),
        make_pair("Opponent",          "Опонент"),
        make_pair("Color Opponent",    "Колір опонента"),
        make_pair("Back",              "Назад"),
        make_pair("Next",              "Далі"),
        make_pair("Select Type Game",  "Виберіть тип гри"),
        make_pair("Local Game",        "Локальна гра"),
        make_pair("Player vs. Player", "Два гравця"),
        make_pair("Player vs. Bot",    "Гравець проти бота"),
        make_pair("Only Player",       "Тільки гравець"),
        make_pair("Only Bot",          "Тільки бот"),
        make_pair("Start",             "Старт"),
        //GameView
        make_pair("Music",             "Музика"),
        make_pair("Effect",            "Ефекти")
    };

    return dictionary[text];
}

const string &Language::get_en(const string &text)
{
    return text;
}

const string &Language::get_ru(const string &text)
{
    static map<string, string> dictionary {
        //MenuScene
        make_pair("Classic",           "Классическая"),
        make_pair("Survival",          "Выживание"),
        make_pair("Local",             "Локальная"),
        make_pair("Exit",              "Выход"),
        make_pair("Player",            "Игрок"),
        make_pair("Color Player",      "Цвет игрока"),
        make_pair("Opponent",          "Оппонент"),
        make_pair("Color Opponent",    "Цвет оппонента"),
        make_pair("Back",              "Назад"),
        make_pair("Next",              "Далее"),
        make_pair("Select Type Game",  "Выберите тип игры"),
        make_pair("Local Game",        "Локальная игра"),
        make_pair("Player vs. Player", "Два игрока"),
        make_pair("Player vs. Bot",    "Игрок против бота"),
        make_pair("Only Player",       "Только игрок"),
        make_pair("Only Bot",          "Только бот"),
        make_pair("Start",             "Старт"),
        //GameView
        make_pair("Music",             "Музыка"),
        make_pair("Effect",            "Эффекты")
    };

    return dictionary[text];
}
