# RedHorse

## 项目简介

RedHorse 是一个用于截图并保存到指定文件夹的C++应用程序。它使用Windows API进行截图，并将截图保存为BMP文件，当图片数量大于等于200，发送至阿里云的OSS存储并清空文件夹。

## 功能

1. **截图功能**：可以捕获整个屏幕或指定窗口的截图，并保存为BMP文件。
2. **文件夹初始化**：自动创建截图文件夹。
3. **文件数量检查**：检查指定文件夹下的BMP文件数量是否大于等于200。
4. **发送至阿里云OSS**: 使用OSS SDK上传图片。

## 使用方法

### 先决条件
请购买阿里云的OSS存储服务，创建好Bucket列表。
按照官方文档编译OSS C++ SDK, 并在Visual Studio 中配置。[https://blog.csdn.net/weixin_44152895/article/details/109313250](参考)

### 编译

1. 打开Visual Studio。
2. 打开 `RedHorse` 项目。
3. 选择适当的配置（例如 `Debug` 或 `Release`）和平台（例如 `x64`）。
4. 编译项目。

### 运行
运行编译后的可执行文件 `RedHorse.exe`。

## 依赖

- **Windows SDK**：用于调用Windows API。
- **MSVC++**：用于使用在Visual Studio 2022中的功能。
- **OSS C++ SDK**：用于上传文件。

## 贡献

欢迎贡献代码和提出改进建议。中文或英文都是被接受的。

## 许可

本项目不使用许可证，允许随便传播或修改。

## 联系

- **作者**：meng qinyuan
- **GitHub**：https://github.com/mengqinyuan
- **Dev.to**: https://dev.to/mengqinyuan