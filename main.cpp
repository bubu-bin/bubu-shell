#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>

int main(int argc, char *argv[])
{
    std::string input;

    while (true) {
        std::cout << "bubu-shell ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;
        
        pid_t pid = fork();
        
        if (pid == 0) {
            std::vector<std::string> args;
            std::stringstream ss(input);
            std::string token;

            while (getline(ss, token, ' ')) {
                args.push_back(token);
            }
            
            const char* path = args.front().c_str();
            std::vector<char*> argv;
            for (const std::string& arg : args) {
                argv.push_back(strdup(arg.c_str()));
            }
            argv.push_back(nullptr);
            
            std::cout << execvp(path, argv.data());
        } else {
            int status;
            wait(&status);
        }
    }

    std::cout << "exitting..." << std::endl;
}
