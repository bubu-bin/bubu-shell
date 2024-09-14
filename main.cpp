#include <iostream>

int main(int argc, char *argv[])
{
    std::string input;

    while (true) {
        std::cout << "bubu-shell ";
        std::getline(std::cin, input);

        if (input == "exit") 
            break;
    }

    std::cout << "exitting..." << std::endl;
}
