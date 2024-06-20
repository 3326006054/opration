// 缺页置换算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
struct Page_replace {
    struct Pages {
        int pages_num;
        int* page_arr;
        int top;
        bool is_full() {
            if (this->top >= this->pages_num) {
                return true;
            }
            else {
                return false;
            }
        }
        void append(int page) {
            if (this->is_full()) return;
            this->page_arr[this->top] = page;
            (this->top)++;
        }
        void replace(int place, int page) {
            if (place >= this->pages_num) return;
            this->page_arr[place] = page;
        }
        int check_broken(int page) {
            for (int i = 0; i < this->top; i++) {
                if (page == this->page_arr[i]) return i;
            }
            if (this->is_full()) {
                return -1;//发生缺页且驻留集已满
            }
            else {
                return -2;//发生缺页且驻留集未满
            }
        }
        void clear() {
            this->top = 0;
        }
    }pages;
    std::vector<int> page_order;
    std::vector<bool> pages_broken;
    void init() {
        //输入驻留集大小
        std::cout << "输入驻留集数量" << std::endl;
        std::cin >> this->pages.pages_num;
        getchar();
        this->pages.page_arr = new int[this->pages.pages_num];
        this->pages.top = 0;
        std::cout << "输入页面访问顺序" << std::endl;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        //输入访问页流
        int num;
        while (iss >> num) {
            this->page_order.push_back(num);
        }
        //初始化缺页中断数组
        for (int i = 0; i < this->page_order.size(); i++) {
            this->pages_broken.push_back(false);
        }
    }
    void show_pages() {
        std::vector<int>::iterator it;
        //输出页流
        for (it = this->page_order.begin(); it != this->page_order.end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    void show_broken() {
        for (int i = 0; i < this->pages_broken.size(); i++) {
            std::cout << this->pages_broken[i] << " ";
        }
        std::cout << std::endl;
    }
    void OPT() {
        for(int i =0;i<this->page_order.size();i++){
            //驻留集未满
            
            int broken = 1;
            for (int j = 0; j < this->pages.top; j++) {
                if (this->page_order[i] == this->pages.page_arr[j]) {
                    broken = 0;
                    break;
                }
                if (j == this->pages.top - 1) broken = 1;
            }
            if (broken) {
                this->pages_broken[i] = true;
                if (this->pages.is_full()) {
                    //寻找需要替换的页面
                    int page = -1;
                    int distance_max = 0;
                    for (int j = 0; j < this->pages.top; j++) {//遍历驻留集
                        int distance_count = 0;//记录最远访问
                        for (int k = i+1; k < this->page_order.size(); k++) {//遍历页流
                            distance_count++;
                            if (this->page_order[k] == this->pages.page_arr[j]) break;//未来最近访问
                        }
                        if (distance_count > distance_max) {//记录最远访问的页面
                            distance_max = distance_count;
                            page = j;
                        }
                    }
                    this->pages.replace(page, this->page_order[i]);
                    std::cout << "时刻"<< i << " 替换" << page << "->" << this->page_order[i] << std::endl;
                }
                else {
                    this->pages.append(this->page_order[i]);
                    std::cout << "时刻" << i  << " " << this->page_order[i] << " 加入" << std::endl;
                }
            }
            else {
                this->pages_broken[i] = false;
            }
        }
        this->show_pages();
        this->show_broken();
        this->pages.clear();
    }
    void FIFO() {
        int* distance = new int[this->pages.pages_num];
        for (int i = 0; i < this->pages.pages_num; i++) {
            distance[i] = 0;
        }
        for (int i = 0; i < this->page_order.size(); i++) {
            int page = this->pages.check_broken(this->page_order[i]);
            if (page == -1) {//发生缺页，且驻留集满
                int distance_max = 0;
                int page_replace = -1;
                for (int j = 0; j < this->pages.top; j++) {
                    if (distance_max < distance[j]) {
                        distance_max = distance[j];
                        page_replace = j;
                    }
                }
                distance[page_replace] = 0;
                std::cout << "时刻" << i << " 替换" << this->pages.page_arr[page_replace] << "->" << this->page_order[i] << std::endl;
                this->pages.replace(page_replace, this->page_order[i]);
                this->pages_broken[i] = true;
            }
            else if (page == -2) {//发生缺页，且驻留集未满
                std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                this->pages.append(this->page_order[i]);
                this->pages_broken[i] = true;
            }
            else this->pages_broken[i] = false;
            //else distance[page] = 0;
            for (int j = 0; j < this->pages.top; j++) {
                (distance[j])++;
            }
        }
        this->show_pages();
        this->show_broken();
        this->pages.clear();
        delete distance;
    }
    void LRU() {
        int* distance = new int[this->pages.pages_num];
        for (int i = 0; i < this->pages.pages_num; i++) {
            distance[i] = 0;
        }
        for (int i = 0; i < this->page_order.size(); i++) {
            int page = this->pages.check_broken(this->page_order[i]);
            if (page == -1) {//发生缺页，且驻留集满
                int distance_max = 0;
                int page_replace = -1;
                for (int j = 0; j < this->pages.top; j++) {
                    if (distance_max < distance[j]) {
                        distance_max = distance[j];
                        page_replace = j;
                    }
                }
                distance[page_replace] = 0;
                std::cout << "时刻" << i << " 替换" << this->pages.page_arr[page_replace] << "->" << this->page_order[i] << std::endl;
                this->pages.replace(page_replace, this->page_order[i]);
                this->pages_broken[i] = true;
            }
            else if (page == -2) {//发生缺页，且驻留集未满
                std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                this->pages.append(this->page_order[i]);
                this->pages_broken[i] = true;
            }
            else this->pages_broken[i] = false;

            for (int j = 0; j < this->pages.top; j++) {
                (distance[j])++;
                if (this->pages.page_arr[j] == this->page_order[i]) {
                    distance[j] = 0;
                }
            }

        }
        this->show_pages();
        this->show_broken();
        this->pages.clear();
        delete distance;
    }
    void CLOCK() {
        int* visit = new int[this->pages.pages_num];
        for (int i = 0; i < this->pages.pages_num; i++) {
            visit[i] = 0;
        }
        int point = 0;
        for (int i = 0; i < this->page_order.size(); i++) {
            int page = this->pages.check_broken(this->page_order[i]);
            if (page == -1) {//发生缺页，且驻留集满
                while (1) {
                    int find = 0;
                    if (visit[point] == 0) {
                        std::cout << "时刻" << i << " 替换" << this->pages.page_arr[point] << "->" << this->page_order[i] << std::endl;
                        this->pages.replace(point, this->page_order[i]);
                        this->pages_broken[i] = true;
                        visit[point] = 1;
                        find = 1;
                    }
                    else {
                        visit[point] = 0;
                    }
                    point++;
                    point = point % this->pages.pages_num;
                    if (find) break;
                }
            }
            else if (page == -2) {//发生缺页，且驻留集未满
                std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                visit[point] = 1;
                point++;
                point = point % this->pages.pages_num;
                this->pages.append(this->page_order[i]);
                this->pages_broken[i] = true;
            }
            else {
                std::cout << "时刻" << i << " " << this->pages.page_arr[page] << " 访问" << std::endl;
                visit[page] = 1;
                this->pages_broken[i] = false;
            }
        }
        this->show_pages();
        this->show_broken();
        this->pages.clear();
        delete visit;
    }
};

int main()
{
    Page_replace P;
    P.init();
    P.CLOCK();
}


