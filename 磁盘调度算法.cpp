// 磁盘调度算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#define MAXINT 2147483647
struct dispatch {
    int start;
    double avg_distance;
    std::vector<int> order;
    std::vector<bool> unfind;
    std::vector<int> distance;
    void init() {
        std::cout << "输入起始点" << std::endl;
        std::cin >> this->start;
        getchar();
        std::cout << "输入访问序列" << std::endl;
        std::string line;
        std::getline(std::cin, line); 
        std::istringstream iss(line);
        int num;
        while (iss >> num) {
            this->order.push_back(num);
        }
        for (int i = 0; i < this->order.size(); i++) {
            this->unfind.push_back(true);
        }
    }
    void FIFO() {
        std::cout << "FIFO" << std::endl;
        this->distance.clear();
        for (int i = 0; i < this->unfind.size(); i++) {
            this->unfind[i] = true;
        }
        int point = this->start;
        double distance = 0;
        for (int i = 0; i < this->order.size(); i++) {
            distance += std::abs(this->order[i] - point);
            this->distance.push_back(std::abs(this->order[i] - point));
            std::cout << this->order[i] << " ";
            point = this->order[i];
        }
        std::cout << std::endl;
        this->avg_distance = distance / this->order.size();
        std::cout << "平均寻道时间" << std::endl;
        std::cout << this->avg_distance << std::endl;
    }
    void SSTF() {
        std::cout << "SSTF" << std::endl;
        this->distance.clear();
        for (int i = 0; i < this->unfind.size(); i++) {
            this->unfind[i] = true;
        }
        int unfind_num = this->order.size();
        int found_num = 0;
        int point = this->start;
        while (found_num < unfind_num) {
            //寻找最短磁道
            int distance_min = MAXINT;
            int min_num = -1;
            for (int i = 0; i < this->order.size(); i++) {
                if (!this->unfind[i]) continue;
                if (std::abs(this->order[i] - point) < distance_min) {
                    distance_min = std::abs(this->order[i] - point);
                    min_num = i;
                }
            }
            point = this->order[min_num];
            this->distance.push_back(distance_min);
            this->unfind[min_num] = false;
            std::cout << this->order[min_num] << " ";
            found_num++;//计数加1
        }
        double distance_avg = 0;
        for (int i = 0; i < this->distance.size(); i++) {
            distance_avg += this->distance[i];
        }
        this->avg_distance = distance_avg / this->distance.size();
        std::cout << std::endl;
        std::cout << "平均寻道距离" << std::endl;
        std::cout << this->avg_distance << std::endl;
    }
    void SCAN() {
        std::cout << "SCAN" << std::endl;
        this->distance.clear();
        std::sort(this->order.begin(), this->order.end());
        int point = this->start;
        int begin = 0;
        for (int i = 0; i < this->order.size(); i++) {
            if (this->order[i] > point) {
                begin = i;
                break;
            }
        }
        for (int i = begin; i < this->order.size(); i++) {
            this->distance.push_back(std::abs(this->order[i] - point));
            point = this->order[i];
            std::cout << this->order[i] << " ";
        }
        for (int i = begin - 1; i >= 0; i--) {
            this->distance.push_back(std::abs(this->order[i] - point));
            point = this->order[i];
            std::cout << this->order[i] << " ";
        }
        double distance_avg = 0;
        for (int i = 0; i < this->distance.size(); i++) {
            distance_avg += this->distance[i];
        }
        this->avg_distance = distance_avg / this->distance.size();
        std::cout << std::endl << "平均寻道距离" << std::endl;
        std::cout << this->avg_distance << std::endl;
    }
    void C_SCAN(bool direction) {
        std::cout << "C_SCAN" << std::endl;
        this->distance.clear();
        int point = this->start;
        int begin = 0;
        if (direction) {
            std::sort(this->order.begin(), this->order.end());
            for (int i = 0; i < this->order.size(); i++) {
                if (this->order[i] > point) {
                    begin = i;
                    break;
                }
            }
        }
        else {
            std::sort(this->order.begin(), this->order.end(), std::greater<int>());
            for (int i = 0; i < this->order.size(); i++) {
                if (this->order[i] < point) {
                    begin = i;
                    break;
                }
            }
        }
        for (int i = begin; i < this->order.size(); i++) {
            this->distance.push_back(std::abs(this->order[i] - point));
            point = this->order[i];
            std::cout << this->order[i] << " ";
        }
        for (int i = 0; i < begin; i++) {
            this->distance.push_back(std::abs(this->order[i] - point));
            point = this->order[i];
            std::cout << this->order[i] << " ";
        }
        double distance_avg = 0;
        for (int i = 0; i < this->distance.size(); i++) {
            distance_avg += this->distance[i];
        }
        this->avg_distance = distance_avg / this->distance.size();
        std::cout << std::endl << "平均寻道距离" << std::endl;
        std::cout << this->avg_distance << std::endl;
    }
};
int main()
{
    dispatch d;
    d.init();
    d.FIFO();
    d.SSTF();
    d.SCAN();
    d.C_SCAN(false);
}

