#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <winsock2.h> 
#include <conio.h>
#include <locale.h>
#include <Windows.h> 
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <malloc.h>
#include <time.h>
#include "Action.h"
#include "Classes.h"
#include "Read_write.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

extern vector <Expert> spr_expert;
extern vector <string> spr_goal;
extern vector <ExpertMark> experts;
extern vector <string> goals;
extern string problem;
extern vector <float> marks;

//------------------------���� �����------------------------------------------------------------------------
template <typename T>
void Get_number(T& chislo) {
	while (true) { // ���� ������������ �� ��� ���, ���� ������������ �� ������ ���������� ��������
		cin >> chislo;
		cin.ignore(32767, '\n');

		if (cin.fail()) { // ���� ���������� ���������� ��������� ����������
			cout << "������ ��� ����� �����. ��������� ����: ";
			cin.clear(); // ���������� cin � '�������' ����� ������
			cin.ignore(32767, '\n');
		}
		else {
			break;
		}
	}
}

//------------------------����� ������� ���� � �������------------------------------------------------------------
void Time() {
	time_t now = time(0);
	tm* ltm = localtime(&now);

	cout << "����  " <<
		setw(2) << setfill('0') << right << ltm->tm_mday << "." <<
		setw(2) << setfill('0') << right << ltm->tm_mon + 1 << "." <<
		ltm->tm_year + 1900 << " " <<
		"\n����� " <<
		setw(2) << setfill('0') << right << ltm->tm_hour << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_min << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_sec << endl;
	cout.fill(' ');
}

// �����������-------------------------------------------------------------------------------------------------
int Login(string &l) {
	try {
		string log, pass;
		char buf[200];
		char result[200];

		cout << "������� �����: ";
		cin >> log;
		cout << "������� ������: ";
		cin >> pass;
		string str_buf = "L " + log + " " + pass;

		strcpy(buf, str_buf.c_str());

		int Otvet;
		Otvet = Server(buf, result);
		if (Otvet != 0) {
			throw MyException("��� ����� � ��������.");
		}
		l = log;
		return atoi(result);
	}
	catch (MyException exc) {
		cout << exc.Get_error() << endl;
		system("pause");
	}
}

//���������� ��������������-----------------------------------------------------------------
void Users() {
	try {
		char buf[200], result[200];
		int choise, Otvet;
		string log, pass, str_buf;

		while (true) {
			system("cls");
			str_buf = "U";
			strcpy(buf, str_buf.c_str());
			Otvet = Server(buf, result);
			if (Otvet == 0) {
				cout << result << "\n";
			}
			else throw MyException("�� ������� ��������� ���� � �������� � ��������.");
			cout << "1. �������� ������������\n";
			cout << "2. ���������� ������������\n";
			cout << "0. ����� �� ���������� ��������������\n\n";
			cout << "��� �����: ";

			Get_number(choise);
			cout << "\n";

			switch (choise) {
			case 1:
				cout << "\n������� ����� ������������, �������� ������ �������: ";
				cin >> log;
				str_buf = "D " + log;
				strcpy(buf, str_buf.c_str());
				Otvet = Server(buf, result);
				if (Otvet == 0) {
					cout << result << "\n";
				}
				system("pause");
				break;
			case 2:
				cout << "������� ������ � ������������:\n" << "�����: ";
				cin >> log;
				cout << "������: ";
				cin >> pass;
				str_buf = "A " + log + " " + pass;
				strcpy(buf, str_buf.c_str());
				Otvet = Server(buf, result);
				if (Otvet == 0) {
					cout << result << "\n";
				}
				system("pause");
				break;

			case 0: { break; }
			default:
				cout << "��� ������ ��������!\n";
				system("pause");
				break;
			}
			if (choise == 0) break;
		}
	}
	catch (MyException exc) {
		cout << exc.Get_error() << endl;
		system("pause");
	}
}

//���� ����� (� ������������� �������)-----------------------------------------------------------
template <typename T, typename T1>
T Get_number_s(T1 str, T min_n, T max_n) {
	T chislo; 
	while (true) { //���� ���� �� ����� ����������
		cout << str;
		cin >> chislo;
		if (cin.good()) { //���� ��� ������
			cin.ignore(32767, '\n');
			if (chislo >= min_n && chislo <= max_n) {
				return chislo;
			}
			else { cout << "����� � ��������� " << min_n << " - " << max_n << endl; }
		}
		else {
			cin.clear(); // �������� ���� ������
			cin.ignore(32767, '\n');
			cout << "������������ ���� �����\n";
		}
	}
}

//���������� ���������--------------------------------------------------------------------------
void Spr_expert() {
	int j, n, skill;
	string fio, position;
	while (true) {
		cout << "\n�������� �����������:" << endl;
		for (int i = 0; (i < spr_expert.size()); i++) {
			cout << i + 1 << "." << spr_expert[i] << endl;
		}

		if (spr_expert.size() == 0)
			cout << "\n�������� - 1, ����� - 0\n";
		else
			cout << "\n�������� - 1, �������� - 2, ������� - 3, ����� - 0\n��� �����: ";

		Get_number(n);
		if (n == 0) {
			Write_expert();
			break;
		}
		switch (n) {
		case 1:
			cout << "\n���������: ";
			getline(cin, position);
			cout << "\n���: ";
			getline(cin, fio);
			skill = Get_number_s("\n��������������: ", 1, 10);
			spr_expert.push_back(Expert(fio, position, skill));
			break;
		case 2:
			while (true) {
				j = Get_number_s("������� ����� ������ ��� ��������������: ", 0, (int)spr_expert.size());
				if (j == 0) break;

				cout << "\n���������: ";
				getline(cin, position);
				cout << "\n���: ";
				getline(cin, fio);
				skill = Get_number_s("\n��������������: ", 1, 10);
				spr_expert[j-1] = Expert(fio, position, skill);
				cout << "\n���������������.\n";
				break;
	
			}
			break;
		case 3:
			while (true) {
				j = Get_number_s("������� ����� ������ ��� ��������: ", 0, (int)spr_expert.size());
				if (j == 0)  break; 

				spr_expert.erase(spr_expert.begin() + j - 1);
				cout << "\n�������.\n";
				break;
			}
			break;
		default:
			break;
		}
	}
}
//���������� �����-----------------------------------------------------------------------------------
void Spr_goal() {
	int j, n;
	string goal;
	while (true) {
		cout << "\n����:" << endl;
		for (int i = 0; (i < spr_goal.size()); i++) {
			cout << i + 1 << "." << spr_goal[i] << endl;
		}

		if (spr_goal.size() == 0)
			cout << "\n�������� - 1, ����� - 0\n";
		else
			cout << "\n�������� - 1, �������� - 2, ������� - 3, ����� - 0\n��� �����: ";

		Get_number(n);
		if (n == 0) {
			Write_goal();
			break;
		}
		switch (n) {
		case 1:
			cout << "\n����: ";
			getline(cin, goal);
			spr_goal.push_back(goal);
			break;
		case 2:
			while (true) {
				j = Get_number_s("������� ����� ������ ��� ��������������: ", 0, (int)spr_goal.size());
				if (j == 0) break;
		
				cout << "\n����: ";
				getline(cin, goal);
				auto it = spr_goal.begin(); // �������� ��� ������ ������ � �������
				*(it + j - 1) = goal;
				cout << "\n���������������\n";
				break;
			}
			break;
		case 3:
			while (true) {
				j = Get_number_s("������� ����� ������ ��� ��������: ", 0, (int)spr_goal.size());
				if (j == 0) break; 

				spr_goal.erase(spr_goal.begin() + j - 1);
				cout << "\n�������.\n";
				break;
			}
			break;
		default:
			break;
		}
	}
}
// �������� ��� �������----------------------------------------------------------------------------
void New_problem() {
	int i, n;

	while (true) {
		system("cls");
		Show_problem();

		cout << "\n1.�������� \n2.���� \n3.�������� \n4.�������� ��������� \n0.����� \n��� �����: ";
		Get_number(n);
		if (n == 0) {
			break;
		}
		switch (n) {
		case 1:
			cout << "\n������������� �������� ��� �������: ";
			getline(cin, problem);
			break;
		case 2:
			Choose_goal();
			break;
		case 3:
			Choose_expert();
			break;
		case 4:
			Solve();
			break;
		default:
			break;
		}
	}
}
//����� ���������----------------------------------------------------------------------------------
void Choose_expert() {
	int i, j, k;
	cout << "\n�������� �����������:" << endl;
	for (int i = 0; (i < spr_expert.size()); i++) {
		cout << i + 1 << "." << spr_expert[i] << endl;
	}
	experts.clear();
	char buf[200];
	char* nstr = buf; // � ������ ��������� �� ������ ������ str
	char* tstr; // ���������, ������������ ��� ����������� ����������  �������������� ������.

	cout << "������ ����� ������: " << endl;
	cin.getline(buf, 200);
	while (true) {
		tstr = nstr;
		// ����������� ��������� ������� ������
		k = strtod(nstr, &nstr);
		// ��������� ��� �� ������ ������������� � ���� ��� ������  ������������� ������� �� �����
		if (k == 0 && tstr == nstr) break;
		if (k > 0 && k <= spr_expert.size()) experts.push_back(spr_expert[k - 1]);
	}
}

// ����� ����� ��� �������--------------------------------------------------------------------------
void Choose_goal() {
	int i, j, k;
	cout << "\n���� �����������:" << endl;
	for (int i = 0; (i < spr_goal.size()); i++) {
		cout << i + 1 << "." << spr_goal[i] << endl;
	}
	goals.clear();
	char buf[200];
	char* nstr = buf; // � ������ ��������� �� ������ ������ str
	char* tstr; // ���������, ������������ ��� ����������� ����������  �������������� ������.

	cout << "������ ����� ������: " << endl;
	cin.getline(buf, 200);
	while (true) {
		tstr = nstr;
		// ����������� ��������� ������� ������
		k = strtod(nstr, &nstr);
		// ��������� ��� �� ������ ������������� � ���� ��� ������  ������������� ������� �� �����
		if (k == 0 && tstr == nstr) break;
		if (k > 0 && k <= spr_goal.size()) goals.push_back(spr_goal[k - 1]);
	}
}

// �������� ��������---------------------------------------------------------------------------------
void Show_problem() {
	cout << "��������: " << problem << endl;
	cout << "\n��������� ����: " << endl;

	vector<string>::const_iterator it; // �������� ��� ������ �� �����
	it = goals.begin();
	while (it != goals.end()) {
		cout << *it << endl;
		++it;
	}

	cout << "\n�������� ��� ������: " << endl;
	for (int i = 0; (i < experts.size()); i++) {
		cout << experts[i] << endl;
	}
}

//�������� ���������� ������ � ��������� ���������� �� �������-----------------------------------------
void Solve() {
	char char_marks[200]; // ������ ����� ������
	char* nstr; // ���������, �� ����������������� ������� ������ str, � ������ ��������� �� ������ ������ buf
	char* tstr; // ���������, ������������ ��� ����������� ���������� �������������� ������.
	
	float mark;
	string str;
	char buf[200], result[200];
	
	// ��������� ������ ���������
	for (int i = 0; i < experts.size(); i++) {
		while (true) {
			cout << endl << i + 1 << "-� �������: " << experts[i].Get_position() << " " << experts[i].Get_FIO() << "\n��� ������:\n";
			cin.getline(char_marks, 200); //��� ������ ������ �������� ����� ������

			marks.clear(); // �������� �����!
			nstr = char_marks;
			while (true) {
				tstr = nstr;
				mark = strtod(nstr, &nstr); // ����������� ��������� ������� ������ � �����
				if (mark == 0 && tstr == nstr) break; // ���� ��� �� ������ ������������� - ������� �� �����
				marks.push_back(mark); // ���������� ������ � ������ ������
			}
			marks.resize(goals.size(), 0); // ����������� ������� ������ = ���-�� �����
			experts[i].Set_marks(marks);
			if (experts[i].Sum_marks() == 1) { // ����� ������ ������ �������� �.�. = 1
				break;
			}
		}
	}
	
	//������ ��� �������� �������
	str = "M"; // ������� M - �����
	for (int i = 0; i < experts.size(); i++) {
		str += experts[i].Get_skill_and_marks() + " 99 "; //�������������� + ������ + 99(������� ��������� ���� ������ ��������)
	}

	strcpy(buf, str.c_str());

	//�������� �� ������ ������� � ��������� ������
	int Otvet = Server(buf, result);

	//������������ ������ char_marks
	marks.clear();
	nstr = result;
	while (true) {
		tstr = nstr;
		mark = strtod(nstr, &nstr); // ����������� ��������� ������� ������ � �����
		if (mark == 0 && tstr == nstr) break; // ���� ��� �� ������ ������������� - ������� �� �����
		marks.push_back(mark); // ���������� ������ � ������ ������
	}
	
	//������ ����������� ������� � ����� 
	string filename = "PROBLEM.TXT";
	Write_result(filename);
	//�����  �� �����
	Display_file(filename);
}

// �������� ������� �� ������ � ��������� ������---------------------------------------------------------
int Server(char*  buf, char*  result) {
	setlocale(LC_ALL, "Russian");

	char R[200];

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(s, (sockaddr*)& dest_addr, sizeof(dest_addr));
	send(s, buf, 200, NULL);

	recv(s, (char*)& R, sizeof(R), NULL);
	strcpy(result, R);

	closesocket(s);
	WSACleanup();
	return 0;
}