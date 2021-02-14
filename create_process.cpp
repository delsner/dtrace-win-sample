#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include "windows.h"
#else

#include <unistd.h>

#endif

/**
 * A simple program to create a new process.
 */
int main() {
    std::cout << "Creating subprocess from PID " <<
              #ifdef _WIN32
              GetCurrentProcessId()
              #else
              getpid()
              #endif
              << std::endl;
    std::filesystem::path file_path = std::filesystem::current_path() /
                                      #ifdef _WIN32
                                      "syscall_file_read.exe"
                                      #else
                                      "syscall_file_read"
#endif
    ;
    int return_code = system(file_path.string().c_str());
    std::cout << "Subprocess returned with status " << return_code << std::endl;
    return 0;
}
