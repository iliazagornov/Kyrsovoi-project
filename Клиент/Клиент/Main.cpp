#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <Windows.h> 
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <iomanip>
#include <vector>
#include "Classes.h"
#include "Read_write.h"
#include "Action.h"

using namespace std;

vector <Expert> spr_expert;
vector <string> spr_goal;
vector <ExpertMark> experts;
vector <string> goals;
vector <float> marks;
string problem;

int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try {
		Read_expert();
		Read_goal();

		register int choise;
		register int who_user = -1; // �������� �� ���������������
		string l;

		while (who_user < 0) {  // ���� �������� �� ���������������
			who_user = Login(l); // ����� � ������
			if (who_user == -2) {
				throw MyException("������, ���� ������� � ������� �� ��������!");
			}
			system("cls");
			if (who_user >= 0) {
				while (who_user == 1) {
					system("cls");
					cout << "����� ����������, �������������.\n";
					cout << "1. ���������� ���������\n";
					cout << "2. ���������� �����\n";
					cout << "3. ����� �������\n";
					cout << "4. ���������� ��������������\n";
					cout << "5. ������� ���� � �����\n";
					cout << "6. ����� �� ��������\n";
					cout << "0. ����� �� ���������.\n\n";
					cout << "��� �����: ";

					Get_number(choise);
					cout << "\n";

					switch (choise) {
					case 1: Spr_expert(); break;
					case 2: Spr_goal(); break;
					case 3: Display_file("ARCHIVE.TXT");
						break;
					case 4: Users();
						break;
					case 5: cout << "����� - ����� ������� ������ � ����� �����, ������� ������ ������� �����, ������� ������ ���." << endl;
						Time();
						system("pause");
						break;
					case 6: system("cls");
						who_user = -1;
						break;
					case 0:
						break;
					default: cout << "��� ������ ��������!\n";
						system("pause");
						break;
					}
					if (choise == 0) break;
				}
				while (who_user == 2) {
					system("cls");
					cout << "����� ����������, " << l << "!\n";
					cout << "1. ���������� �����\n";
					cout << "2. ������� ����� ������\n";
					cout << "3. ����� �������\n";
					cout << "4. ������� ���� � �����\n";
					cout << "5. � ���������\n";
					cout << "6. ����� �� ��������\n";
					cout << "0. ����� �� ���������.\n\n";
					cout << "��� �����: ";

					Get_number(choise);
					cout << "\n";

					switch (choise) {

					case 1: Spr_goal(); break;
					case 2: New_problem(); break;
					case 3: Display_file("ARCHIVE.TXT");
						break;
					case 4: cout << "����� - ����� ������� ������ � ����� �����, ������� ������ ������� �����, ������� ������ ���." << endl;
						Time();
						system("pause");
						break;
					case 5: system("cls"); 
						cout << "��������� �������� ���������� ������ ������� �������� � ������� ������ ���������� ���������� ������.\n\n";
						cout << "�������� ������� ��� ������� ������ �������� ������ ����� ��� �����������, ��������, �� �������������� � ������ ����� ����������. \n��� �������� ���������� ���� ������ ���������� �� ������.\n\n";
						cout << "��������� ������� �������� ���������������, ���������� � ���� ���������� ������ ���������� ���������� ������, ������ ����� � �� �����.\n\n";
						system("pause");
						break;
					case 6: system("cls");
						who_user = -1;
						break;
					case 0: break;
					default: cout << "��� ������ ��������!\n";
						system("pause");
						break;
					}
					if (choise == 0) break;
				}
			}
		}
	}
	catch (MyException exc) {
		cout << exc.Get_error() << endl;
		system("pause");
	}
}