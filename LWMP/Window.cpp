#include "Functions.h"
#include "Window.h"

enum WindowState
{
    UNINITIALIZED,
    VISIBLE,
    HIDDEN
};

void** const DISPLAY_SWITCH = (void**)ASLR(0xFD7358);
int* const FEFEE8 = (int*)ASLR(0xFEFEE8);
int* const WAIT_FOR_INPUT = (int*)ASLR(0xFEFEEC);
int* const STATE = (int*)ASLR(0xFEFEF0);
void** const GENERAL_WINDOW = (void**)ASLR(0xFEFEF8);
char* const IS_GLOBAL = (char*)ASLR(0xFEFEFC);
char* const FEFEFD = (char*)ASLR(0xFEFEFD);
int* const SELECTED_BUTTON = (int*)ASLR(0xFEFF00);
float* const ELAPSED_TIME = (float*)ASLR(0xFEFF04);

void Window::appear(const wchar_t* text)
{
    appear(text, nullptr, nullptr);
}

void Window::appear(const wchar_t* text, float time)
{
    appear(text, nullptr, &time);
}

void Window::appear(const wchar_t* text, Buttons buttons)
{
    appear(text, &buttons, nullptr);
}

void Window::appear(const wchar_t* text, Buttons* buttons, const float* time)
{
    struct
    {
        float field0;
        float field4;
        int field8;
        int field10;
        int field14;
        int field18;
        int field1C;
        int field20;
        int field24;
    } sParam;

    memset(&sParam, 0, sizeof(sParam));

    sParam.field0 = -320.0f;
    sParam.field4 = -320.0f;
    sParam.field1C = 0x104;
    sParam.field20 = 3;
    sParam.field24 = buttons ? (*buttons == Buttons::OK ? 1 : *buttons == Buttons::YES_NO ? 3 : 0) : 0;

    *GENERAL_WINDOW = generalWindowCtor(gameObjectNew(0x15C), &sParam);
    setText(text);

    gameDocumentAddObject(*GAME_DOCUMENT, *GENERAL_WINDOW);

    *FEFEFD = 0;
    *IS_GLOBAL = true;
    *WAIT_FOR_INPUT = !time;
    *SELECTED_BUTTON = 0;
    *FEFEE8 = 1;
    *ELAPSED_TIME = 1.0f - (time ? *time : 1.0f);
    *STATE = VISIBLE;
}

Window::YesNoResult Window::getYesNoResult()
{
    return !*GENERAL_WINDOW || *STATE == 1
               ? YesNoResult::NONE
               : generalWindowIsYesButtonPressed()
               ? YesNoResult::YES
               : generalWindowIsNoButtonPressed()
               ? YesNoResult::NO
               : YesNoResult::NONE;
}

bool Window::isVisible()
{
    return *GENERAL_WINDOW && *STATE == 1;
}

void Window::setText(const wchar_t* text)
{
    if (!*GENERAL_WINDOW)
        return;

    struct
    {
        const char* key;
        const wchar_t* text;
        const int gap8;
        const int gapC;
    } converseData = {"LWMP", text, 0, 0};

    generalWindowSetText(*GENERAL_WINDOW, &converseData);
}

void Window::disappear()
{
    if (!*GENERAL_WINDOW)
        return;

    generalWindowDisappear();
}
