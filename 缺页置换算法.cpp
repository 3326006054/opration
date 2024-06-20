// 缺页置换算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
struct Page_replace {
    std::vector<int> page_order;

    struct Pages {
        int pages_num;
        int* page_arr;
        int top;

        bool is_full() {
            return (this->top >= this->pages_num) ? true : false;
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
            return this->is_full() ? -1 : -2;
        }

        void clear() {
            this->top = 0;
        }
    }pages;

    void init() {
        std::cout << "输入驻留集数量：";
        std::cin >> this->pages.pages_num;
        getchar();
        this->pages.page_arr = new int[this->pages.pages_num];
        this->pages.top = 0;
        std::cout << "输入页面访问顺序：";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        int num;
        while (iss >> num) {
            this->page_order.push_back(num);
        }
    }

    void handle_pages_init(int**& pages_show, std::vector<bool>& pages_broken) {
        for (int j = 0; j < this->pages.pages_num; j++) {
            pages_show[j] = new int[this->page_order.size()];
        }
        for (int i = 0; i < this->page_order.size(); i++) {
            pages_broken.push_back(false);
        }
    }

    void handle_pages_show(int** pages_show, std::vector<bool> pages_broken) {
        std::vector<int>::iterator it;
        std::cout << "页流：";
        for (it = this->page_order.begin(); it != this->page_order.end(); it++) {
            std::cout << *it << " ";
        }
        for (int j = 0; j < this->pages.pages_num; j++) {
            std::cout << std::endl << "      ";
            for (int i = 0; i < this->page_order.size(); i++) {
                std::cout << (pages_show[j][i] > 0 ? std::to_string(pages_show[j][i]) + " " : "N ");
            }
        }
        std::cout << std::endl << "缺页：";
        for (int i = 0; i < pages_broken.size(); i++) {
            std::cout << (pages_broken[i] ? "F " : "  ");
        }
        std::cout << std::endl;
        this->pages.clear();
    }

    void OPT() {
        int** pages_show = new int* [this->pages.pages_num];
        std::vector<bool> pages_broken;
        handle_pages_init(pages_show, pages_broken);
        std::cout << std::endl << "OPT" << std::endl;
        for (int i = 0; i < this->page_order.size(); i++) {
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
                pages_broken[i] = true;
                if (this->pages.is_full()) {
                    //寻找需要替换的页面
                    int page = -1;
                    int distance_max = 0;
                    for (int j = 0; j < this->pages.top; j++) {//遍历驻留集
                        int distance_count = 0;//记录最远访问
                        for (int k = i + 1; k < this->page_order.size(); k++) {//遍历页流
                            distance_count++;
                            if (this->page_order[k] == this->pages.page_arr[j]) break;//未来最近访问
                        }
                        if (distance_count > distance_max) {//记录最远访问的页面
                            distance_max = distance_count;
                            page = j;
                        }
                    }
                    this->pages.replace(page, this->page_order[i]);
                    std::cout << "时刻" << i << " 替换" << page << "->" << this->page_order[i] << std::endl;
                }
                else {
                    this->pages.append(this->page_order[i]);
                    std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                }
            }
            else {
                pages_broken[i] = false;
            }
            for (int j = 0; j < this->pages.top; j++) {
                pages_show[j][i] = this->pages.page_arr[j];
            }
        }
        this->handle_pages_show(pages_show, pages_broken);
    }

    void FIFO() {
        int** pages_show = new int* [this->pages.pages_num];
        std::vector<bool> pages_broken;
        handle_pages_init(pages_show, pages_broken);
        std::cout << std::endl << "FIFO" << std::endl;
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
                pages_broken[i] = true;
            }
            else if (page == -2) {//发生缺页，且驻留集未满
                std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                this->pages.append(this->page_order[i]);
                pages_broken[i] = true;
            }
            else pages_broken[i] = false;
            //else distance[page] = 0;
            for (int j = 0; j < this->pages.top; j++) {
                (distance[j])++;
            }
            for (int j = 0; j < this->pages.top; j++) {
                pages_show[j][i] = this->pages.page_arr[j];
            }
        }
        this->handle_pages_show(pages_show, pages_broken);
        delete distance;
    }

    void LRU() {
        int** pages_show = new int* [this->pages.pages_num];
        std::vector<bool> pages_broken;
        handle_pages_init(pages_show, pages_broken);
        std::cout << std::endl << "LRU" << std::endl;
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
                pages_broken[i] = true;
            }
            else if (page == -2) {//发生缺页，且驻留集未满
                std::cout << "时刻" << i << " " << this->page_order[i] << " 加入" << std::endl;
                this->pages.append(this->page_order[i]);
                pages_broken[i] = true;
            }
            else pages_broken[i] = false;

            for (int j = 0; j < this->pages.top; j++) {
                (distance[j])++;
                if (this->pages.page_arr[j] == this->page_order[i]) {
                    distance[j] = 0;
                }
            }
            for (int j = 0; j < this->pages.top; j++) {
                pages_show[j][i] = this->pages.page_arr[j];
            }
        }
        this->handle_pages_show(pages_show, pages_broken);
        delete distance;
    }

    void CLOCK() {
        int** pages_show = new int* [this->pages.pages_num];
        std::vector<bool> pages_broken;
        handle_pages_init(pages_show, pages_broken);
        std::cout << std::endl << "CLOCK" << std::endl;
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
                        pages_broken[i] = true;
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
                pages_broken[i] = true;
            }
            else {
                std::cout << "时刻" << i << " " << this->pages.page_arr[page] << " 访问" << std::endl;
                visit[page] = 1;
                pages_broken[i] = false;
            }
            for (int j = 0; j < this->pages.top; j++) {
                pages_show[j][i] = this->pages.page_arr[j];
            }
        }
        this->handle_pages_show(pages_show, pages_broken);
        delete visit;
    }
};

int main()
{
    Page_replace P;
    P.init();
    P.OPT();
    P.LRU();
    P.FIFO();
    P.CLOCK();
}
