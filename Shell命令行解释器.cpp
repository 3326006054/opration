#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGUMENTS 10

// �ڲ��������
void handleInternalCommand(char* command, char* args[]) {
    if (strcmp(command, "cd") == 0) {
        if (args[1] == NULL) {
            // �����ǰ����Ŀ¼
            char cwd[MAX_COMMAND_LENGTH];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("��ǰĿ¼: %s\n", cwd);
            }
            else {
                printf("shell> ");
            }
        }
        else {
            // ���ĵ�ǰ����Ŀ¼
            if (chdir(args[1]) != 0) {
                printf("δ�ҵ�Ŀ¼: %s\n", args[1]);
            }
            else {
                // ����PWD��������
                if (setenv("PWD", args[1], 1) != 0) {
                    printf("shell> ");
                }
            }
        }
    }
    else if (strcmp(command, "environ") == 0) {
        // �г����л�������
        extern char** environ;
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]);
        }
    }
    else if (strcmp(command, "echo") == 0) {
        // ��ʾ���ݲ�����
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    }
    else if (strcmp(command, "help") == 0) {
        // ���������Ϣ
        printf("Shell�����б�:\n");
        printf("cd <directory> - ���ĵ�ǰ����Ŀ¼\n");
        printf("environ - �г����л�������\n");
        printf("echo <content> - ��ʾ���ݲ�����\n");
        printf("help - ��ʾ������Ϣ\n");
        printf("jobs - �г���ǰ�ӽ���\n");
        printf("quit/exit/bye - �˳�Shell\n");
    }
    else if (strcmp(command, "jobs") == 0) {
        // �г���ǰ�ӽ���
        printf("��ǰ�ӽ���:\n");
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
        // �˳�shell
        exit(0);
    }
    else {
        printf("δ֪������: %s\n", command);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char* args[MAX_ARGUMENTS];
    char* token;

    while (1) {
        printf("shell> ");
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';  // ȥ�����з�

        // ���������в���
        int argCount = 0;
        token = strtok(command, " ");
        while (token != NULL && argCount < MAX_ARGUMENTS - 1) {
            args[argCount] = token;
            argCount++;
            token = strtok(NULL, " ");
        }
        args[argCount] = NULL;

        if (argCount > 0) {
            // �ڲ������
            handleInternalCommand(args[0], args);
        }
        else {
            // �ⲿ�����
            pid_t pid = fork();
            if (pid == 0) {
                // �ӽ���ִ������
                execvp(args[0], args);
                exit(0);  // execvpʧ�����˳��ӽ���
            }
            else if (pid > 0) {
                // �ȴ��ӽ��̽���
                int childStatus;
                waitpid(pid, &childStatus, 0);
            }
            else {
                printf("�޷������ӽ���\n");
            }
        }
    }

    return 0;
}

/*
* ��gcc����ʱ�����Ϻ�׺-std=c99
*/