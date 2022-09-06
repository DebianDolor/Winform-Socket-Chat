#pragma once
#define _AFXDLL
#include "Definition.h"

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

	bool isConnected();
	bool signUp(User* user);
	bool logIn(SOCKET socket, User user);
	bool isOnlineUser(wstring username);
	bool isOnlineGroup(wstring groupname);

	void startListenClient();
	void setHWND(HWND hwnd);
	void sendMessageGroup(wstring groupname, wstring sender, WCHAR* message, int len, bool isSendToSender = false);
	void addUser(User* user);
	
	int recClient(SOCKET recSocket);
	int sendMessageClient(ClientPacket* client, WCHAR* message, int len);
	
	list<User*>& getUser();
private:
	bool _isConnected;
	
	int _serverPort;
	
	SOCKET _socClient;
	SOCKET _socListenClient;
	
	HWND _hwnd;
	
	list<ClientPacket*> _clientList;
	list<User*> _userData;
	list<GroupChat*> _groupchatList;
};

