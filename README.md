# RedHorse
中文文档请查看: README_zh.md

## Project Overview
RedHorse is a C++ application designed to capture screenshots and save them to a specified folder. It uses the Windows API to take screenshots and saves them as BMP files. When the number of images reaches or exceeds 200, they are uploaded to Alibaba Cloud OSS storage and the folder is cleared.

## Features
- **Screenshot Functionality**: Captures the entire screen or a specified window and saves the screenshot as a BMP file.
- **Folder Initialization**: Automatically creates a screenshot folder.
- **File Count Check**: Verifies if the number of BMP files in the specified folder is greater than or equal to 200.
- **Upload to Alibaba Cloud OSS**: Uses the OSS SDK to upload images.

## Usage

### Prerequisites
Purchase Alibaba Cloud OSS storage and create a Bucket list.
Compile the OSS C++ SDK according to the official documentation and configure it in Visual Studio.

### Compilation
Open Visual Studio.
Open the RedHorse project.
Select the appropriate configuration (e.g., Debug or Release) and platform (e.g., x64).
Compile the project.

### Execution
Run the compiled executable file RedHorse.exe.

### Dependencies
Windows SDK: For calling Windows API functions.
MSVC++: For utilizing features in Visual Studio 2022.
OSS C++ SDK: For uploading files.

### Contribution
Contributions and suggestions for improvement are welcome. Both Chinese and English are accepted.

### License
This project does not use a license and allows for unrestricted distribution or modification.

### Contact
Author: Meng Qinyuan
GitHub: https://github.com/mengqinyuan
Dev.to: https://dev.to/mengqinyuan