// 银行家算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
struct Banker {
    int process_num;
    int m;
    int* Resource;
    int* Available;
    int** Allocation;
    int** Need;
    void init() {
        std::cout << "输入进程数量" << std::endl;
        std::cin >> this->process_num;
        getchar();
        this->Allocation = new int* [this->process_num];
        this->Need = new int* [this->process_num];
        std::cout << "输入Allocation" << std::endl;
        int m = 0;
        for (int i = 0; i < this->process_num; i++) {
            std::string line;
            std::getline(std::cin, line);
            std::istringstream count(line);
            std::istringstream iss(line);
            int num;
            if (i == 0) {//分配空间
                for (m = 0; count >> num; m++)
                {

                }
                this->m = m;
                this->Available = new int[this->m];
                for (int n = 0; n < this->process_num; n++) {
                    this->Allocation[n] = new int[this->m];
                    this->Need[n] = new int[this->m];
                }
            }
            for (m = 0; iss >> num; m++) {
                this->Allocation[i][m] = num;
            }
        }
        std::cout << "输入Need矩阵" << std::endl;
        for (int i = 0; i < this->process_num; i++) {
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int num;
            for (int m = 0; iss >> num; m++) {
                this->Need[i][m] = num;
            }
        }
        std::cout << "输入Available矩阵" << std::endl;
        for (int i = 0; i < this->m; i++) {
            std::cin >> this->Available[i];
        }
        this->show();
    }
    inline void show() {
        for (int i = 0; i < this->process_num; i++) {
            for (int j = 0; j < this->m; j++)std::cout << this->Allocation[i][j] << " ";
            std::cout << "         ";
            for (int j = 0; j < this->m; j++)std::cout << this->Need[i][j] << " ";
            std::cout << "         ";
            if (i == 0)for (int j = 0; j < this->m; j++)std::cout << this->Available[j] << " ";
            std::cout << std::endl;
        }
    }
    bool safe_check() {
        int** Allocation = new int*[this->process_num];
        int* Available = new int[this->m];
        for (int i = 0; i < this->process_num; i++) {
            Allocation[i] = new int[this->m];
            std::copy_n(this->Allocation[i],this->m, Allocation[i]);
        }
        std::copy_n(this->Available, this->m, Available);
        int* found = new int[this->process_num];
        for (int i = 0; i < this->process_num; i++) found[i] = 0;
        int found_num = 0;
        int* safe_order = new int[this->process_num];
        while (found_num < this->process_num) {
            int check = 1;
            for (int i = 0; i < this->process_num; i++) {
                if (found[i])continue;
                for (int j = 0; j < this->m; j++) {
                    if (Available[j] < this->Need[i][j]) {
                        break;
                    }
                    if (j == this->m - 1) {
                        found[i] = 1;
                        check = 0;
                        safe_order[found_num] = i;
                        for (int m = 0; m < this->m; m++) {
                            Available[m] += Allocation[i][m];//回收资源
                        }
                        found_num++;
                    }
                }
                if (i == this->process_num - 1 && check) {
                    std::cout << "此状态不安全" << std::endl;
                    /* 内存回收 */
                    delete Available;
                    delete safe_order;
                    delete found;
                    for (int n = 0; n < this->process_num; n++) {
                        delete Allocation[n];
                    }
                    delete Allocation;
                    return false;
                }
            }
        }
        for (int i = 0; i < found_num; i++)std::cout << safe_order[i] << " ";
        std::cout << std::endl;
        /* 内存回收 */
        delete Available;
        delete safe_order;
        delete found;
        for (int n = 0; n < this->process_num; n++) {
            delete Allocation[n];
        }
        delete Allocation;
        return true;
    }
    void request() {
        std::cout << "输入请求" << std::endl;
        int pid;
        std::cin >> pid;
        getchar();
        int* request = new int[this->m];
        for (int i = 0; i < this->m; i++) {
            std::cin >> request[i];
            if (request[i] > this->Available[i]) {
                std::cout << "请求的资源不足" << std::endl;
                return;
            }
            if (request[i] > this->Need[pid][i]) {
                std::cout << "请求的资源过多" << std::endl;
                return;
            } 
        }
        //尝试分配
        for (int i = 0; i < this->m; i++) {
            this->Allocation[pid][i] += request[i];
            this->Need[pid][i] -= request[i];
            this->Available[i] -= request[i];
        }
        if (!this->safe_check()) {
            //状态不安全，还原之前状态
            std::cout << "还原" << std::endl;
            for (int i = 0; i < this->m; i++) {
                this->Allocation[pid][i] -= request[i];
                this->Need[pid][i] += request[i];
                this->Available[i] += request[i];
            }
        }
        else std::cout << "分配成功" << std::endl;
        delete request;
        this->show();
    }
};
int main()
{
    Banker b;
    b.init();
    b.request();
}
