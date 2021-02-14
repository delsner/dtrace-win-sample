#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#ifdef _WIN32
#include "process.h"
#include "windows.h"
#else

#include <unistd.h>

#endif

/**
 * A simple program to open and read a file.
 */
int main() {
    std::cout << "Opening file from PID " <<
              #ifdef _WIN32
              GetCurrentProcessId()
              #else
              getpid() << " -> child of " << getppid()
              #endif
              << std::endl;
    std::string line;
    std::ifstream file;
    std::filesystem::path file_path = std::filesystem::current_path() /
                                      #ifdef _WIN32
                                      ".." / ".." / // This depends on the build process - in VS the executable is located in `./out/build/x86*/exec.exe`.
                                      #endif
                                      ".." / "sample.txt";
    std::cout << "Opening file at: " << file_path << std::endl;
    file.open(file_path);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    }
    return 0;
}
