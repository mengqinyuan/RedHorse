// RedHorseCore.h
#pragma once

#include <windows.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <chrono>

namespace RedHorseCore {
    std::wstring getCurrentTimestamp();
    void ShootScreen(const std::wstring& filename, HWND hWnd);
    void init();
    bool HasAtLeast200BmpFiles(const std::wstring& folderPath);
}