// 读者写者问题.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
HANDLE RCmutex;//读者互斥锁
HANDLE Wmutex;//写者互斥锁
static int count = 0;//临界资源
static int reading_count = 0;
DWORD WINAPI Writer(LPVOID);
DWORD WINAPI Reader(LPVOID);
bool p_continue = true;
int main()
{
	
	const unsigned short READER_NUM = 1;
	const unsigned short WRITER_NUM = 1;
	RCmutex = CreateMutex(NULL, FALSE, NULL);
	Wmutex = CreateMutex(NULL, FALSE, NULL);
	const unsigned short THREADS_NUM = READER_NUM + WRITER_NUM;
	HANDLE hThreads[THREADS_NUM];
	DWORD writerID[WRITER_NUM];
	DWORD readerID[READER_NUM];
	for (int i = 0; i < WRITER_NUM; i++) {
		hThreads[i] = CreateThread(NULL, 0, Writer, NULL, 0, &writerID[i]);
		if (hThreads[i] == NULL) return -1;
	}
	for (int i = 0; i < READER_NUM; i++) {
		hThreads[i + WRITER_NUM] = CreateThread(NULL, 0, Reader, NULL, 0, &readerID[i]);
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
		WaitForSingleObject(Wmutex, INFINITE);
		count++;
		Sleep(1000);
		std::cout << "写入了临界资源count count:" << count << std::endl;
		ReleaseMutex(Wmutex);
	}
	return 0;
}
DWORD WINAPI Reader(LPVOID l) {
	while (p_continue) {
		WaitForSingleObject(RCmutex, INFINITE);
		reading_count++;
		if (reading_count == 1) WaitForSingleObject(Wmutex, INFINITE);
		ReleaseMutex(RCmutex);
		std::cout << "读取临界资源count count:" << count << std::endl;
		Sleep(500);
		WaitForSingleObject(RCmutex, INFINITE);
		reading_count--;
		if (reading_count == 0) ReleaseMutex(Wmutex);
		ReleaseMutex(RCmutex);
	}
	return 0;
}