#include "Connection.h"
#include <process.h>
#include <ws2tcpip.h>

struct WindowsSocketsInitializer
{
	bool success;

	WindowsSocketsInitializer()
	{
		WSADATA data;
		success = WSAStartup(MAKEWORD(2,2), &data) == 0;
	}

	~WindowsSocketsInitializer()
	{
		WSACleanup();
	}
} windowsSocketsInitializer;

Connection::Connection(HWND hWnd) : isServerMode(0), hWnd(hWnd)
{
	if(!windowsSocketsInitializer.success)
		throw L"Windows Sockets is not initialized";

	socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket == INVALID_SOCKET)
		throw L"Can't create socket";

	//создать поток для обработки соединения
	thread = (HANDLE)_beginthreadex(NULL, 0, StaticProcessThreadRoutine, this, CREATE_SUSPENDED, NULL);
	if(!thread)
		throw L"Can't create thread";
}

Connection::~Connection()
{
	closesocket(socket);
}

unsigned __stdcall Connection::StaticProcessThreadRoutine(void *data)
{
	((Connection*)data)->ProcessThreadRoutine();
	return 0;
}

void Connection::ProcessThreadRoutine()
{
	//если серверный режим
	if(isServerMode)
	{
		//дождаться сокета
		sockaddr_in addr;
		int len = sizeof(addr);
		SOCKET newSocket = accept(socket, (sockaddr*)&addr, &len);
		if(newSocket == INVALID_SOCKET)
		{
			PostMessage(hWnd, WM_CONNECTION, MessageConnection_AcceptFailed, 0);
			return;
		}

		//закрыть старый сокет
		closesocket(socket);

		//заменить его новым
		socket = newSocket;

		//отправить уведомление
		PostMessage(hWnd, WM_CONNECTION, MessageConnection_Accepted, 0);
	}
	//иначе клиентский режим
	else
	{
		//получить адреса сервера
		addrinfo* addresses;
		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		if(getaddrinfo(serverName.c_str(), NULL, &hints, &addresses) != 0)
		{
			PostMessage(hWnd, WM_CONNECTION, MessageConnection_ConnectFailed, 0);
			return;
		}

		//последовательно перебирать адреса, и пробовать подключаться к ним
		addrinfo* address;
		int error;
		for(address = addresses; address; address = address->ai_next)
		{
			sockaddr_in addr = *(sockaddr_in*)address->ai_addr;
			addr.sin_port = htons(port);
			//попытка подключиться к серверу
			if((error=connect(socket, (sockaddr*)&addr, address->ai_addrlen)) == 0)
				break;
		}
		error=WSAGetLastError();
		//освободить память адресов
		freeaddrinfo(addresses);
		//если все попытки были безуспешными
		if(!address)
		{
			PostMessage(hWnd, WM_CONNECTION, MessageConnection_ConnectFailed, 0);
			return;
		}

		//иначе сообщить об установленном соединении
		PostMessage(hWnd, WM_CONNECTION, MessageConnection_Connected, 0);
	}

	//если управление здесь, значит, сокет готов к приему/передаче данных
	//цикл считывания данных
	for(; ;)
	{
		char* message = new char[messageSize];
		int size = 0;
		while(size < messageSize)
		{
			int read = recv(socket, message + size, messageSize - size, 0);
			if(read <= 0)
			{
				closesocket(socket);
				PostMessage(hWnd, WM_CONNECTION, MessageConnection_ReadFailed, 0);
				return;
			}
			size += read;
		}

		//отправить сообщение окну
		PostMessage(hWnd, WM_CONNECTION, MessageConnection_IncomingMessage, (LPARAM)message);
	}
}

bool Connection::StartServer()
{
	//указать режим соединения
	isServerMode = true;

	//выполнить привязку сокета к порту
	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;
	if(bind(socket, (const sockaddr*)&addr, sizeof(addr)) != 0)
		return false;

	//начать прослушивание
	if(listen(socket, 1) != 0)
		return false;

	//запустить поток для принятия клиентов
	ResumeThread(thread);

	//все
	return true;
}

bool Connection::ConnectClient(const char* serverName)
{
	//указать режим соединения
	isServerMode = false;

	//запомнить имя сервера
	this->serverName = serverName;

	//запустить поток для принятия клиентов
	ResumeThread(thread);

	//все
	return true;
}

void Connection::Send(const char* message)
{
	char buffer[messageSize];
	memset(buffer, 0, messageSize);
	strcpy(buffer, message);

	int size = 0;
	while(size < messageSize)
	{
		int sent = send(socket, buffer, messageSize, 0);
		if(sent <= 0)
			break;
		size += sent;
	}
}
