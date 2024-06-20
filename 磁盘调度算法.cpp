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
    static void init(dispatch* self) {
        std::cout << "输入起始点" << std::endl;
        std::cin >> self->start;
        getchar();
        std::cout << "输入访问序列" << std::endl;
        std::string line;
        std::getline(std::cin, line); 
        std::istringstream iss(line);
        int num;
        while (iss >> num) {
            self->order.push_back(num);
        }
        for (int i = 0; i < self->order.size(); i++) {
            self->unfind.push_back(true);
        }
    }
    void FIFO(dispatch* d) {
        d->distance.clear();
        for (int i = 0; i < d->unfind.size(); i++) {
            d->unfind[i] = true;
        }
        int point = d->start;
        double distance = 0;
        for (int i = 0; i < d->order.size(); i++) {
            distance += std::abs(d->order[i] - point);
            d->distance[i] = std::abs(d->order[i] - point);
            std::cout << d->order[i] << " ";
            point = d->order[i];
        }
        std::cout << std::endl;
        d->avg_distance = distance / d->order.size();
    }
    void SSTF(dispatch* d) {
        d->distance.clear();
        for (int i = 0; i < d->unfind.size(); i++) {
            d->unfind[i] = true;
        }
        int unfind_num = d->order.size();
        int found_num = 0;
        int point = d->start;
        while (found_num < unfind_num) {
            //寻找最短磁道
            int distance_min = MAXINT;
            int min_num = -1;
            for (int i = 0; i < d->order.size(); i++) {
                if (!d->unfind[i]) continue;
                if (std::abs(d->order[i] - point) < distance_min) {
                    distance_min = std::abs(d->order[i] - point);
                    min_num = i;
                }
            }
            point = d->order[min_num];
            d->distance.push_back(distance_min);
            d->unfind[min_num] = false;
            std::cout << d->order[min_num] << " ";
            found_num++;//计数加1
        }
        double distance_avg = 0;
        for (int i = 0; i < d->distance.size(); i++) {
            distance_avg += d->distance[i];
        }
        d->avg_distance = distance_avg / d->distance.size();
        std::cout << "平均寻道距离" << std::endl;
        std::cout << d->avg_distance << std::endl;
    }
    void SCAN(dispatch* d) {
        d->distance.clear();
        std::sort(d->order.begin(), d->order.end());
        int point = d->start;
        int begin = 0;
        for (int i = 0; i < d->order.size(); i++) {
            if (d->order[i] > point) {
                begin = i;
                break;
            }
        }
        for (int i = begin; i < d->order.size(); i++) {
            d->distance.push_back(std::abs(d->order[i] - point));
            point = d->order[i];
            std::cout << d->order[i] << " ";
        }
        for (int i = begin - 1; i >= 0; i--) {
            d->distance.push_back(std::abs(d->order[i] - point));
            point = d->order[i];
            std::cout << d->order[i] << " ";
        }
        double distance_avg = 0;
        for (int i = 0; i < d->distance.size(); i++) {
            distance_avg += d->distance[i];
        }
        d->avg_distance = distance_avg / d->distance.size();
        std::cout << std::endl << "平均寻道距离" << std::endl;
        std::cout << d->avg_distance << std::endl;
    }
    void C_SCAN(dispatch * d, bool direction) {
        d->distance.clear();
        int point = d->start;
        int begin = 0;
        if (direction) {
            std::sort(d->order.begin(), d->order.end());
            for (int i = 0; i < d->order.size(); i++) {
                if (d->order[i] > point) {
                    begin = i;
                    break;
                }
            }
        }
        else {
            std::sort(d->order.begin(), d->order.end(), std::greater<int>());
            for (int i = 0; i < d->order.size(); i++) {
                if (d->order[i] < point) {
                    begin = i;
                    break;
                }
            }
        }
        for (int i = begin; i < d->order.size(); i++) {
            d->distance.push_back(std::abs(d->order[i] - point));
            point = d->order[i];
            std::cout << d->order[i] << " ";
        }
        for (int i = 0; i < begin; i++) {
            d->distance.push_back(std::abs(d->order[i] - point));
            point = d->order[i];
            std::cout << d->order[i] << " ";
        }
        double distance_avg = 0;
        for (int i = 0; i < d->distance.size(); i++) {
            distance_avg += d->distance[i];
        }
        d->avg_distance = distance_avg / d->distance.size();
        std::cout << std::endl << "平均寻道距离" << std::endl;
        std::cout << d->avg_distance << std::endl;
    }
};
int main()
{
    dispatch d;
    d.init(&d);
    d.C_SCAN(&d, false);
}

