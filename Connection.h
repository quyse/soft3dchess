#ifndef ___CONNECTION_H___
#define ___CONNECTION_H___

#include "windows.h"
#include <string>
#include <winsock2.h>

/*
Файл содержит класс соединения. Соединение может быть как серверным, так и клиентским.
*/

//оконные сообщения от соединения
enum ConnectionMessages
{
	//общее сообщение для всех событий
	WM_CONNECTION = WM_USER + 1,
	//к серверному соединению подключился клиент
	MessageConnection_Accepted = 0,
	//вызов accept окончился неудачей
	MessageConnection_AcceptFailed,
	//клиентское соединение подключилось к серверу
	MessageConnection_Connected,
	//вызов connect окончился неудачей
	MessageConnection_ConnectFailed,
	//пришло входящее сообщение
	MessageConnection_IncomingMessage,
	//вызов чтения завершился неудачей
	MessageConnection_ReadFailed
};

class Connection
{
public:
	//размер сообщения
	static const int messageSize = 64;
private:
	//порт, используемый для соединения
	static const int port = 7894;
	//сокет соединения
	SOCKET socket;
	//поток, используемый для обработки соединения
	HANDLE thread;
	//режим соединения - сервер
	bool isServerMode;
	//окно, которому надо отправлять уведомления
	HWND hWnd;
	//имя сервера, используемое для подключения (в клиентском режиме)
	std::string serverName;

	//функция для потока обработки соединения
	static unsigned __stdcall StaticProcessThreadRoutine(void* data);
	void ProcessThreadRoutine();

public:
	Connection(HWND hWnd);
	~Connection();

	//запустить серверное соединение
	bool StartServer();

	//запустить подключение к серверу
	bool ConnectClient(const char* serverName);

	//отправить сообщение
	void Send(const char* message);
};

#endif
