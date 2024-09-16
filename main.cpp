#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>

extern "C" {
    int yyparse();
    void yyerror(const char* s);
    extern int yylex_destroy();
    extern void yy_scan_string(const char *); 
}

void yyerror(const char* s) {
    std::cerr << "Error: " << s << std::endl;
}

int main(int argc, char *argv[])
{
    std::string input;
    int commands_num = 2;
    const char *commands[2][10] = {
                                {"ls", "-l", NULL}, 
                                {"grep", "shell.l", NULL}
                            };

    while (true) {
        std::cout << "bubu-shell ";
        std::getline(std::cin, input);

        if (input == "exit") 
            break;

        // parse the user input
        yy_scan_string(input.c_str()); 
        yyparse();
        yylex_destroy();

        // store original fd's for reference
        int tmpin = dup(STDIN_FILENO);
        int tmpout = dup(STDOUT_FILENO);

        int fdin = dup(STDIN_FILENO);
        int fdout;
        pid_t pid;

        for (int i = 0; i < commands_num; i++) {
            // redirect input
            dup2(fdin, STDIN_FILENO);
            close(fdin);

            if (i == commands_num - 1) {
                // restore the default stdout
                fdout = dup(tmpout);
            } else {
                int fdpipe[2];
                pipe(fdpipe);
                fdin = fdpipe[0];
                fdout = fdpipe[1];
            }

            // write the current fdout into stdout
            dup2(fdout, STDOUT_FILENO);
            close(fdout);

            pid = fork();
            if (pid == 0) {
                execvp(commands[i][0], const_cast<char* const *>(commands[i]));
                _exit(1);
            }

            if (i == commands_num - 1) {
                close(fdin);
            }
        }

        dup2(tmpin, STDIN_FILENO);
        dup2(tmpout, STDOUT_FILENO);

        close(tmpin);
        close(tmpout);

        waitpid(pid, NULL, 0);
    }

    std::cout << "exitting..." << std::endl;
}
