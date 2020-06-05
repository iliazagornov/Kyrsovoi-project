#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <stdio.h> 
#include <locale.h>
#include <Windows.h> 
#include "Classes.h"
#include "Metod.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;
int ExpertMark::m_all_level = 0; // статичная переменная - сумма компетентности всех экспертов ///!!!!!
int numcl = 0;

DWORD WINAPI ThreadFunc(LPVOID client_socket) {
	SOCKET s2 = ((SOCKET*)client_socket)[0];

	char buf[200], result[200];
	recv(s2, buf, 200, NULL);
	if (buf[0] == 'M') { // рассчитать оценку
		string sResult;
		sResult = buf;
		strcpy(buf, sResult.substr(1).c_str());
		Metod(buf, result);
	}
	else if (buf[0] == 'L') { // смотреть логин и пароль
		string sResult;
		sResult = buf;
		strcpy(buf, sResult.substr(1).c_str());
		Metod_log(buf, result);
	}
	else if (buf[0] == 'A') { // добавить пользователя
		string sResult;
		sResult = buf;
		strcpy(buf, sResult.substr(1).c_str());
		Users_add(buf, result);
	}
	else if (buf[0] == 'D') { // удалить пользователя
		string sResult;
		sResult = buf;
		strcpy(buf, sResult.substr(1).c_str());
		Users_del(buf, result);
	}
	else if (buf[0] == 'U') { // gользователя
		string sResult;
		sResult = buf;
		strcpy(buf, sResult.substr(1).c_str());
		Users_list(buf, result);
	}

	send(s2, (char*)& result, sizeof(result), NULL);

	closesocket(s2);
	cout << "Клиент отключен" << endl;
	return 0;
}

void print() {
	if (numcl) {
		cout << numcl << " клиент подсоеденён" << endl;
	}
	else cout << "Нет соединений." << endl;
}

void main() {
	setlocale(LC_ALL, "Russian");

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	if (::bind(s, (sockaddr*)&local_addr, sizeof(local_addr)) != SOCKET_ERROR) {
		printf("Сокет привязан.\n");
	}
	if (listen(s, 5) != SOCKET_ERROR) {
		printf("Старт прослушивания порта: %u\n", ntohs(local_addr.sin_port));
	}

	cout << "Сервер подключен." << endl;
	cout << endl;

	SOCKET client_socket;
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);

	while ((client_socket = accept(s, (sockaddr*)& client_addr, &client_addr_size))) {
		numcl++;
		print();
		DWORD thID;
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, NULL);
	}
}