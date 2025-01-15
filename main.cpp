// main.cpp
#include "RedHorseCore.h"

int32_t wmain() {
    RedHorseCore::init();

    std::wstring userProfile;
    DWORD size = GetEnvironmentVariable(L"USERPROFILE", NULL, 0);
    if (size > 0) {
        userProfile.resize(size - 1);
        GetEnvironmentVariable(L"USERPROFILE", &userProfile[0], size);
    }

    std::wstring baseDir = userProfile + L"\\Desktop\\screenshot\\";

    for (int32_t i = 0; i < 2001; ++i) {
        std::wstring filename = baseDir + RedHorseCore::getCurrentTimestamp() + L".bmp";
        std::wcout << L"shooting " << filename << std::endl;
        RedHorseCore::ShootScreen(filename, NULL);
        Sleep(1000);

        // 判断是否可以发送至阿里云OSS
        if (RedHorseCore::HasAtLeast200BmpFiles(baseDir)) {
            // SEND LOGIC HERE

            RedHorseCore::cleanFolder(baseDir); //清空文件夹

        }
    }
    return 0;
}