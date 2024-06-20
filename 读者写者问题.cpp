// 读者写者问题.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
HANDLE RCmutex;//读者互斥锁
HANDLE Wmutex;//写者互斥锁
static int count = 0;//临界资源
static int reading_count = 0;//正在读文件的读者数量
DWORD WINAPI Writer(LPVOID);//声明写者进程
DWORD WINAPI Reader(LPVOID);//声明读者进程
bool p_continue = true;//控制进程
int main()
{
	const unsigned short READER_NUM = 1;
	const unsigned short WRITER_NUM = 1;
	RCmutex = CreateMutex(NULL, FALSE, NULL);//创建reading_count 互斥信号量
	Wmutex = CreateMutex(NULL, FALSE, NULL);//创建count 互斥信号量
	const unsigned short THREADS_NUM = READER_NUM + WRITER_NUM;//进程总数
	HANDLE hThreads[THREADS_NUM];//所有线程
	DWORD writerID[WRITER_NUM];//写者进程PID
	DWORD readerID[READER_NUM];//读者进程PID
	for (int i = 0; i < WRITER_NUM; i++) {
		hThreads[i] = CreateThread(NULL, 0, Writer, NULL, 0, &writerID[i]);//创建写者进程
		if (hThreads[i] == NULL) return -1;
	}
	for (int i = 0; i < READER_NUM; i++) {
		hThreads[i + WRITER_NUM] = CreateThread(NULL, 0, Reader, NULL, 0, &readerID[i]);//创建读者进程
		if (hThreads[i + WRITER_NUM] == NULL) return -1;
	}
	while (p_continue) {
		if (getchar()) { //按回车后终止程序运行
			p_continue = false;
		}
	}
	return 0;
}
DWORD WINAPI Writer(LPVOID l) {
	while (p_continue) {
		WaitForSingleObject(Wmutex, INFINITE);//互斥访问count
		count++;
		Sleep(1000);
		std::cout << "写入了临界资源count count:" << count << std::endl;
		ReleaseMutex(Wmutex);
	}
	return 0;
}
DWORD WINAPI Reader(LPVOID l) {
	while (p_continue) {
		WaitForSingleObject(RCmutex, INFINITE);//互斥访问reading_count
		reading_count++;
		if (reading_count == 1) WaitForSingleObject(Wmutex, INFINITE);//有读者在读，写者上锁
		ReleaseMutex(RCmutex);
		std::cout << "读取临界资源count count:" << count << std::endl;
		Sleep(500);
		WaitForSingleObject(RCmutex, INFINITE);
		reading_count--;
		if (reading_count == 0) ReleaseMutex(Wmutex);//没有读者，写者解锁
		ReleaseMutex(RCmutex);
	}
	return 0;
}
