#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__
#include <string>

enum Locale {
    ua,
    en,
    ru,
    undefined
};

class Language
{
    Language() = delete;
//    static std::map<std::string, std::string> words;

public:
    static const std::string &get(Locale locale, const std::string &text);
    static const std::string &get_ua(const std::string &text);
    static const std::string &get_en(const std::string &text);
    static const std::string &get_ru(const std::string &text);
};

#endif // __LANGUAGE_H__
