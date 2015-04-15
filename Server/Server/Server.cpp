#include <windows.h>
#include <process.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib") 
#include "SServer.h"
#include "CSocket.h"
#include <vector>
#include "ClientList.h"
#include <string> 
const int BUF_LEN=1024;
  
void recv(PVOID pt)
{
	CSocket* csocket=(CSocket*)pt;
	if(csocket!=NULL)
	{
		int count= csocket->Receive(BUF_LEN); 
		if(count==0)
		{  
			ClientList* list=ClientList::GetInstance(); 
			list->printTime();
			cout<<"User logoff: user"<<csocket->getNickname()<<"Online User Num is :"<<list->Count()-1<<endl;
			list->Remove(csocket);
			_endthread(); //用户下线，终止接收数据线程
		}
	} 
}

void sends(PVOID pt)
{
	ClientList* list=(ClientList*)pt;
	while(1)
	{
		char* buf=new char[BUF_LEN] ;
		cin>>buf;
		int bufSize=0;
		while(buf[bufSize++]!='\0'); 
		for(int i=list->Count()-1;i>=0;i--)
		{
			(*list)[i]->Send(buf,bufSize);  
		} 
		delete buf;
	}
}
 
int main(int argc, char* argv[])
{
	SServer server;
	bool isStart=server.Start(1986);
	if(isStart)
	{
		server.printTime();
		cout<<"Server Start Success..."<<endl;
	}else
	{
		server.printTime();
		cout<<"Server Start Rrror"<<endl;
	} 
	ClientList* list=ClientList::GetInstance();/* 维护一个用户队列 */
	_beginthread(sends,0,list);//启动一个线程广播数据
	
	char str[100];
	int i=1;
	while(1)
	 {
		CSocket* csocket=server.Accept();
		itoa(i++, str, 10);
		csocket->setNickname(str);
		list->Add(csocket);
		server.printTime();
		cout<<"New Comer: user"<<csocket->getNickname()<<", Online Num is "<<list->Count()<<endl;
		_beginthread(recv,0,csocket);//启动一个接收数据的线程
	 }
 
	getchar();
	return 0;
} 
