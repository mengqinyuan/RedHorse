# RedHorse

## 项目简介

RedHorse 是一个用于截图并保存到指定文件夹的C++应用程序。它使用Windows API进行截图，并将截图保存为BMP文件，当图片数量大于等于200，发送至阿里云的OSS存储并清空文件夹。

## 功能

1. **截图功能**：可以捕获整个屏幕或指定窗口的截图，并保存为BMP文件。
2. **文件夹初始化**：自动创建截图文件夹。
3. **文件数量检查**：检查指定文件夹下的BMP文件数量是否大于等于200。
4. **发送至阿里云OSS**: （目前未实现）使用OSS SDK上传图片。

## 使用方法

### 先决条件
请购买阿里云的OSS存储服务，创建好Bucket列表。

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

## 贡献

欢迎贡献代码和提出改进建议。请遵循以下步骤：

1. 叉（Fork）本项目。
2. 创建一个新的分支（`git checkout -b feature/your-feature`）。
3. 提交你的更改（`git commit -am 'Add some feature'`）。
4. 推送分支到你的仓库（`git push origin feature/your-feature`）。
5. 创建一个新的拉取请求（Pull Request）。

## 许可

本项目采用MIT许可协议。详情请参阅 [LICENSE](LICENSE) 文件。

## 联系

- **作者**：meng qinyuan
- **GitHub**：https://github.com/mengqinyuan
