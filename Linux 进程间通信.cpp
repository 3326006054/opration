#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <stdlib.h> 
#include <unistd.h>
#define MSGKEY 75
#define MSGKEY_1 100
struct msgform
{
	long mtype;
	char mtext[300];
}msg;
int msgqid, i, msgqid_1;//新建一个消息队列，实现乒乓通信
void CLIENT()
{
	int i, result;
	msgqid = msgget(MSGKEY, 0777);
	if (msgqid == -1)
	{
		perror("client msgget");
		exit(1);
	}
	for (i = 0; i < 10; i++)
	{
		printf("(client) sent \n");
		result = msgsnd(msgqid, &msg, 300, 0);
		if (result == -1)
		{
			perror("client msgsnd");
			exit(1);
		}
		msgrcv(msgqid_1, &msg, 300, 0, 0);//等待server接收数据
	}
	exit(0);
}
void SERVER()
{
	int i;
	int getsize = 0;
	msgqid = msgget(MSGKEY, 0777);
	if (msgqid == -1)
	{
		perror("server msgget");
		exit(1);
	}
	for (i = 0; i < 10; i++)
	{
		getsize = msgrcv(msgqid, &msg, 300, 0, 0);//接受消息
		printf("(Server) recieved %d bytes\n", getsize);
		msgsnd(msgqid_1, &msg, 300, 0);//发送信息
	};
	//删除消息队列
	exit(0);
}
void main()
{
	msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);//777是权限控制bit，IPC_creat表示创建
	msgqid_1 = msgget(MSGKEY_1, 0777 | IPC_CREAT);//第二个消息队列
	if (msgqid == -1)
	{
		perror("main msgget");
	}
	msg.mtype = 1;//必须是非负数，否则报错

	while ((i = fork()) == -1);//一直创建子进程，直到创建成功为止
	if (!i) SERVER();//子进程
	while ((i = fork()) == -1);
	if (!i) CLIENT();//子进程
	//父进程等待
	wait(0);
	wait(0);
	msgctl(msgqid, IPC_RMID, 0);
	msgctl(msgqid_1, IPC_RMID, 0);
}
