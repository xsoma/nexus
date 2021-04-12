#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>

namespace Utils {
    void AttachConsole();

    void DetachConsole();

    bool ConsolePrint(const char* fmt, ...);
}