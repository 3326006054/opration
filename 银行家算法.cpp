// 银行家算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。


#include <stdio.h>
#include <iostream>
int main()
{
    int m = 0;//资源类型的数量
    int n = 0;//进程的数量
    printf("输入资源类型数");
    scanf_s("%d", &m);
    printf("输入进程数");
    scanf_s("%d", &n);
    int* resource = new int[m];
    int* available = new int[m];
    int** need = new int* [n];
    int** Allocation = new int* [n];
    for (int i = 0; i < n; i++)
    {
        need[i] = new int[m];
        Allocation[i] = new int[m];
    }
    std::cout << "输入Allocation矩阵" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> Allocation[i][j];
        }
    }

    std::cout << "输入need矩阵" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> need[i][j];
        }
    }
    std::cout << "输入available数组" << std::endl;
    for (int i = 0; i < m; i++) {
        std::cin >> available[i];
    }
    //计算resource数组
    for (int j = 0; j < m; j++) {
        int count = 0;
        for (int i = 0; i < n; i++) {
            count += Allocation[i][j];
        }
        resource[j] = count + available[j];
    }
    std::cout << "Allocation" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << Allocation[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "need" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << need[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "available" << std::endl;
    for (int i = 0; i < m; i++) {
        std::cout << available[i] << " ";
    }
    //保存当前状态
    int pid = 0;
    int* require = new int[m];
    int** Allocation_0 = new int* [n];
    int** need_0 = new int* [n];
    int* available_0 = new int[m];
    for (int i = 0; i < n; i++) {
        Allocation_0[i] = new int[m];
        need_0[i] = new int[m];
        for (int j = 0; j < m; j++) {
            Allocation_0[i][j] = Allocation[i][j];
            need_0[i][j] = need[i][j];
        }
    }
    for (int i = 0; i < m; i++) {
        available_0[i] = available[i];
    }
    while (1) {
        std::cout << "输入进程 请求" << std::endl;
        std::cin >> pid;
        for (int i = 0; i < m; i++) {
            std::cin >> require[i];
        }
        int available_judge = 0;//是否可以分配
        int plethora_judge = 0;//请求的资源大于need
        //进行判断
        //判断available数组是否有空余资源可以分配
        for (int i = 0; i < m; i++) {
            if (require[i] > available[i]) {
                std::cout << "可用资源不足" << std::endl;
                available_judge = 1;
                break;
            }
        }

        for (int i = 0; i < m; i++) {
            if (require[i] > need[pid][i]) {
                std::cout << "请求过多资源" << std::endl;
                plethora_judge = 1;
                break;
            }
        }
        if (available_judge) continue;
        if (plethora_judge) continue;
        
        //尝试分配
        for (int i = 0; i < m; i++) {
            available[i] -= require[i];
            need[pid][i] -= require[i];
            Allocation[pid][i] += require[i];
        }
        //寻找安全序列
        int* find = new int[n];
        for (int i = 0; i < n; i++) {
            find[i] = 0;
        }
        int end_num = 0;
        int* safe_order = new int[n];
        int unsafe;
        while (end_num < n) {
            int success_find = 0;
            unsafe = 0;
            for (int i = 0; i < n; i++) {
                if (find[i]) continue;//判断是否该进程是否结束
                for (int j = 0; j < m; j++) {
                    if (available[j] < need[i][j]) {
                        break;//该进程无法分配，直接寻找下一个进程
                    }
                    if (j == (m - 1)) {
                        for (int k = 0; k < m; k++) {
                            available[k] += Allocation[i][k];
                        }
                        find[i] = 1;
                        safe_order[end_num] = i;
                        end_num++;
                        success_find = 1;
                    }
                }
                if (success_find) break;//本次循环找到了可执行的程序，结束本次循环
                if (i == (n - 1)) {
                    std::cout << "无法寻找到安全序列" << std::endl;
                    unsafe = 1;
                    break;
                }
            }
            if (unsafe) break;

        }
        //还原初始状态
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                Allocation[i][j] = Allocation_0[i][j];
                need[i][j] = need_0[i][j];
            }
        }
        for (int i = 0; i < m; i++) {
            available[i] = available_0[i];
        }
        if (!unsafe) {//如果允许分配，更新数组
            for (int i = 0; i < n; i++) {
                std::cout << safe_order[i] << " ";
            }
            for (int i = 0; i < m; i++) {
                available[i] -= require[i];
                need[pid][i] -= require[i];
                Allocation[pid][i] += require[i];
            }
        }
        std::cout << std::endl;
    }
}
