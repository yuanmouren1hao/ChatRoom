// Client.cpp  
#include <windows.h>
#include <process.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib") 
#include "CSocket.h"

const int BUF_LEN=1024;
char serverIp[20];
const int serverPort=1986;

void recv(PVOID pt)
{
	CSocket* csocket=(CSocket*)pt;
	if(csocket!=NULL)
	{
		csocket->Receive(BUF_LEN); 
		_endthread(); 
	} 
}
 
int main(int argc, char* argv[]){
	CSocket csocket;
	cout<<"Input The Server IP: ";
	cin>>serverIp;
	bool connect=csocket.Connect(serverIp,serverPort);
	csocket.SetBlocking(false);
	if(connect)
	{
		csocket.printTime();cout<<"connected"<<endl;
		uintptr_t threadId=_beginthread(recv,0,&csocket);//启动一个线程接收数据的线程
		while(1)
		{
			char buf[BUF_LEN];
			cin>>buf;
			 
			csocket.Send(buf,strlen(buf));
			if(csocket.IsExit())
			{  
				csocket.Close(); 
				cout<<"exit success"<<endl;
				break;
			} 
		}
	}else
	{
		csocket.printTime();
		cout<<"not connect"<<endl;
		cout<<csocket.GetSocketError()<<endl;
	}
	 
	getchar();
	return 0;
} 

