#include "font_manager.h"

// 字体配置
const QString FontManager::FONT_FAMILY = "Microsoft YaHei";
const int FontManager::TITLE_FONT_SIZE = 24;
const int FontManager::BUTTON_FONT_SIZE = 14;
const int FontManager::BODY_FONT_SIZE = 12;
const int FontManager::STATUS_BAR_FONT_SIZE = 10;

QFont FontManager::getFont(FontType type)
{
    switch (type) {
    case Title:
        return QFont(FONT_FAMILY, TITLE_FONT_SIZE, QFont::Bold);
    case Button:
        return QFont(FONT_FAMILY, BUTTON_FONT_SIZE, QFont::Medium);
    case Body:
        return QFont(FONT_FAMILY, BODY_FONT_SIZE, QFont::Normal);
    case StatusBar:
        return QFont(FONT_FAMILY, STATUS_BAR_FONT_SIZE, QFont::Normal);
    default:
        return QFont(FONT_FAMILY, BODY_FONT_SIZE, QFont::Normal);
    }
}

QString FontManager::getFontFamily()
{
    return FONT_FAMILY;
}
