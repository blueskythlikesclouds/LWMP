#pragma once

class Window
{
public:
    enum class Buttons
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

    static void appear(const wchar_t* text);
    static void appear(const wchar_t* text, float time);
    static void appear(const wchar_t* text, Buttons buttons);
    static void appear(const wchar_t* text, Buttons* buttons, const float* time);
    static void disappear();

    static YesNoResult getYesNoResult();
    static bool isVisible();

    static void setText(const wchar_t* text);
};
