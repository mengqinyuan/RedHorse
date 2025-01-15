// RedHorseCore.cpp
#include "RedHorseCore.h"
#include <codecvt>
#include <locale>
#include <filesystem>

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

    void init() {
        std::wstring userProfile;
        DWORD size = GetEnvironmentVariable(L"USERPROFILE", NULL, 0);
        if (size > 0) {
            userProfile.resize(size - 1);
            GetEnvironmentVariable(L"USERPROFILE", &userProfile[0], size);
        }

        std::wstring screenshotPath = userProfile + L"\\Desktop\\screenshot";
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
}