#pragma once

enum class WindowButtons
{
    OK,
    YES_NO
};

enum class YesNoResult
{
    NONE,
    YES,
    NO
};

class Window
{
public:
    static void appear(const wchar_t* text);
    static void appear(const wchar_t* text, float time);
    static void appear(const wchar_t* text, WindowButtons buttons);
    static void appear(const wchar_t* text, WindowButtons* buttons, const float* time);
    static void disappear();

    static YesNoResult getYesNoResult();
    static bool isVisible();

    static void setText(const wchar_t* text);
};
