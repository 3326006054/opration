#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGUMENTS 10

// 内部命令处理函数
void handleInternalCommand(char* command, char* args[]) {
    if (strcmp(command, "cd") == 0) {
        if (args[1] == NULL) {
            // 输出当前工作目录
            char cwd[MAX_COMMAND_LENGTH];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("当前目录: %s\n", cwd);
            }
            else {
                printf("shell> ");
            }
        }
        else {
            // 更改当前工作目录
            if (chdir(args[1]) != 0) {
                printf("未找到目录: %s\n", args[1]);
            }
            else {
                // 更新PWD环境变量
                if (setenv("PWD", args[1], 1) != 0) {
                    printf("shell> ");
                }
            }
        }
    }
    else if (strcmp(command, "environ") == 0) {
        // 列出所有环境变量
        extern char** environ;
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
        }
    }
    else if (strcmp(command, "echo") == 0) {
        // 显示内容并换行
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    else if (strcmp(command, "help") == 0) {
        // 输出帮助信息
        printf("Shell命令列表:\n");
        printf("cd <directory> - 更改当前工作目录\n");
        printf("environ - 列出所有环境变量\n");
        printf("echo <content> - 显示内容并换行\n");
        printf("help - 显示帮助信息\n");
        printf("jobs - 列出当前子进程\n");
        printf("quit/exit/bye - 退出Shell\n");
    }
    else if (strcmp(command, "jobs") == 0) {
        // 列出当前子进程
        printf("当前子进程:\n");
        pid_t pid = fork();
        if (pid == 0) {
            execlp("ps", "", NULL);
        }
        else if (pid > 0) {
            wait(NULL);
        }
    }
    else if (strcmp(command, "quit") == 0 ||
        strcmp(command, "exit") == 0 ||
        strcmp(command, "bye") == 0) {
        // 退出shell
        exit(0);
    }
    else {
        printf("未知的命令: %s\n", command);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char* args[MAX_ARGUMENTS];
    char* token;

    while (1) {
        printf("shell> ");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';  // 去除换行符

        // 解析命令行参数
        int argCount = 0;
        token = strtok(command, " ");
        while (token != NULL && argCount < MAX_ARGUMENTS - 1) {
            args[argCount] = token;
            argCount++;
            token = strtok(NULL, " ");
        }
        args[argCount] = NULL;

        if (argCount > 0) {
            // 内部命令处理
            handleInternalCommand(args[0], args);
        }
        else {
            // 外部命令处理
            pid_t pid = fork();
            if (pid == 0) {
                // 子进程执行命令
                execvp(args[0], args);
                exit(0);  // execvp失败则退出子进程
            }
            else if (pid > 0) {
                // 等待子进程结束
                int childStatus;
                waitpid(pid, &childStatus, 0);
            }
            else {
                printf("无法创建子进程\n");
            }
        }
    }

    return 0;
}

/*
* 用gcc编译时，加上后缀-std=c99
*/