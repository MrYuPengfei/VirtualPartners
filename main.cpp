#include "PetWindow.h"
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
     
        // 获取当前工作目录
        std::filesystem::path current_path = std::filesystem::current_path();

        // 打印当前工作目录
        std::cout << "Current working directory: " << current_path << std::endl;

    QApplication app(argc, argv);

    PetWindow petWindow;
    petWindow.show();

    return app.exec();
}
