#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <QFont>

class FontManager
{
private:
    FontManager() = default;
    ~FontManager() = default;

public:
    // 字体类型枚举
    enum FontType {
        Title,      // 标题字体
        Button,     // 按钮字体
        Body,       // 正文字体
        StatusBar   // 状态栏字体
    };

    // 获取字体实例
    static QFont getFont(FontType type);

    // 获取字体名称
    static QString getFontFamily();

private:
    // 字体配置
    static const QString FONT_FAMILY;
    static const int TITLE_FONT_SIZE;
    static const int BUTTON_FONT_SIZE;
    static const int BODY_FONT_SIZE;
    static const int STATUS_BAR_FONT_SIZE;
};

#endif // FONT_MANAGER_H
