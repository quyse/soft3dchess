#ifndef ___CONNECTION_H___
#define ___CONNECTION_H___

#include "windows.h"
#include <string>
#include <winsock2.h>

/*
���� �������� ����� ����������. ���������� ����� ���� ��� ���������, ��� � ����������.
*/

//������� ��������� �� ����������
enum ConnectionMessages
{
	//����� ��������� ��� ���� �������
	WM_CONNECTION = WM_USER + 1,
	//� ���������� ���������� ����������� ������
	MessageConnection_Accepted = 0,
	//����� accept ��������� ��������
	MessageConnection_AcceptFailed,
	//���������� ���������� ������������ � �������
	MessageConnection_Connected,
	//����� connect ��������� ��������
	MessageConnection_ConnectFailed,
	//������ �������� ���������
	MessageConnection_IncomingMessage,
	//����� ������ ���������� ��������
	MessageConnection_ReadFailed
};

class Connection
{
public:
	//������ ���������
	static const int messageSize = 64;
private:
	//����, ������������ ��� ����������
	static const int port = 7894;
	//����� ����������
	SOCKET socket;
	//�����, ������������ ��� ��������� ����������
	HANDLE thread;
	//����� ���������� - ������
	bool isServerMode;
	//����, �������� ���� ���������� �����������
	HWND hWnd;
	//��� �������, ������������ ��� ����������� (� ���������� ������)
	std::string serverName;

	//������� ��� ������ ��������� ����������
	static unsigned __stdcall StaticProcessThreadRoutine(void* data);
	void ProcessThreadRoutine();

public:
	Connection(HWND hWnd);
	~Connection();

	//��������� ��������� ����������
	bool StartServer();

	//��������� ����������� � �������
	bool ConnectClient(const char* serverName);

	//��������� ���������
	void Send(const char* message);
};

#endif
