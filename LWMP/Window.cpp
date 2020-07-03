#include "Window.h"

enum class WindowState : uint32_t
{
    UNINITIALIZED,
    VISIBLE,
    HIDDEN
};

void** const DISPLAY_SWITCH = (void**)ASLR(0xFD7358);
uint32_t* const FEFEE8 = (uint32_t*)ASLR(0xFEFEE8);
uint32_t* const WAIT_FOR_INPUT = (uint32_t*)ASLR(0xFEFEEC);
WindowState* const STATE = (WindowState*)ASLR(0xFEFEF0);
void** const GENERAL_WINDOW = (void**)ASLR(0xFEFEF8);
char* const IS_GLOBAL = (char*)ASLR(0xFEFEFC);
char* const FEFEFD = (char*)ASLR(0xFEFEFD);
uint32_t* const SELECTED_BUTTON = (uint32_t*)ASLR(0xFEFF00);
float* const ELAPSED_TIME = (float*)ASLR(0xFEFF04);

void Window::appear(const wchar_t* text)
{
    appear(text, nullptr, nullptr);
}

void Window::appear(const wchar_t* text, float time)
{
    appear(text, nullptr, &time);
}

void Window::appear(const wchar_t* text, WindowButtons buttons)
{
    appear(text, &buttons, nullptr);
}

void Window::appear(const wchar_t* text, WindowButtons* buttons, const float* time)
{
    struct
    {
        float field0;
        float field4;
        uint32_t field8;
        uint32_t field10;
        uint32_t field14;
        uint32_t field18;
        uint32_t field1C;
        uint32_t field20;
        uint32_t field24;
    } sParam;

    memset(&sParam, 0, sizeof(sParam));

    sParam.field0 = -320.0f;
    sParam.field4 = -320.0f;
    sParam.field1C = 0x104;
    sParam.field20 = 3;
    sParam.field24 = buttons ? (*buttons == WindowButtons::OK ? 1 : *buttons == WindowButtons::YES_NO ? 3 : 0) : 0;

    *GENERAL_WINDOW = generalWindowCtor(gameObjectNew(0x15C), &sParam);
    setText(text);

    gameDocumentAddObject(*GAME_DOCUMENT, *GENERAL_WINDOW);

    *FEFEFD = 0;
    *IS_GLOBAL = true;
    *WAIT_FOR_INPUT = !time;
    *SELECTED_BUTTON = 0;
    *FEFEE8 = 1;
    *ELAPSED_TIME = 1.0f - (time ? *time : 1.0f);
    *STATE = WindowState::VISIBLE;
}

YesNoResult Window::getYesNoResult()
{
    return !*GENERAL_WINDOW || *STATE == WindowState::VISIBLE
               ? YesNoResult::NONE
               : generalWindowIsYesButtonPressed()
               ? YesNoResult::YES
               : generalWindowIsNoButtonPressed()
               ? YesNoResult::NO
               : YesNoResult::NONE;
}

bool Window::isVisible()
{
    return *GENERAL_WINDOW && *STATE == WindowState::VISIBLE;
}

void Window::setText(const wchar_t* text)
{
    if (!*GENERAL_WINDOW)
        return;

    struct
    {
        const char* key;
        const wchar_t* text;
        const uint32_t gap8;
        const uint32_t gapC;
    } converseData = {"LWMP", text, 0, 0};

    generalWindowSetText(*GENERAL_WINDOW, &converseData);
}

void Window::disappear()
{
    if (!*GENERAL_WINDOW)
        return;

    generalWindowDisappear();
}
