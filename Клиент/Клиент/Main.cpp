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
		register int who_user = -1; // входящий не идентифицирован
		string l;

		while (who_user < 0) {  // пока входящий не идентифицирован
			who_user = Login(l); // логин и пароль
			if (who_user == -2) {
				throw MyException("Ошибка, файл логинов и паролей не открылся!");
			}
			system("cls");
			if (who_user >= 0) {
				while (who_user == 1) {
					system("cls");
					cout << "Добро пожаловать, Администратор.\n";
					cout << "1. Справочник экспертов\n";
					cout << "2. Справочник целей\n";
					cout << "3. Архив решений\n";
					cout << "4. Управление пользователями\n";
					cout << "5. Текущая дата и время\n";
					cout << "6. Выход из аккаунта\n";
					cout << "0. Выход из программы.\n\n";
					cout << "Ваш выбор: ";

					Get_number(choise);
					cout << "\n";

					switch (choise) {
					case 1: Spr_expert(); break;
					case 2: Spr_goal(); break;
					case 3: Display_file("ARCHIVE.TXT");
						break;
					case 4: Users();
						break;
					case 5: cout << "Время - самый главный ресурс в нашей жизни, поэтому всегда полезно знать, который сейчас час." << endl;
						Time();
						system("pause");
						break;
					case 6: system("cls");
						who_user = -1;
						break;
					case 0:
						break;
					default: cout << "Нет такого варианта!\n";
						system("pause");
						break;
					}
					if (choise == 0) break;
				}
				while (who_user == 2) {
					system("cls");
					cout << "Добро пожаловать, " << l << "!\n";
					cout << "1. Справочник целей\n";
					cout << "2. Решение новой задачи\n";
					cout << "3. Архив решений\n";
					cout << "4. Текущая дата и время\n";
					cout << "5. О программе\n";
					cout << "6. Выход из аккаунта\n";
					cout << "0. Выход из программы.\n\n";
					cout << "Ваш выбор: ";

					Get_number(choise);
					cout << "\n";

					switch (choise) {

					case 1: Spr_goal(); break;
					case 2: New_problem(); break;
					case 3: Display_file("ARCHIVE.TXT");
						break;
					case 4: cout << "Время - самый главный ресурс в нашей жизни, поэтому всегда полезно знать, который сейчас час." << endl;
						Time();
						system("pause");
						break;
					case 5: system("cls"); 
						cout << "Программа помогает определить лучшее решение проблемы с помощью метода взвешенных экспертных оценок.\n\n";
						cout << "Входными данными для данного метода является список целей для предприятия, эксперты, их компетентности и оценки целей экспертами. \nВсе эксперты выставляют свои оценки независимо от других.\n\n";
						cout << "Выходными данными является отсортированный, полученный в ходе применения метода взвешенных экспертных оценок, список целей и их весов.\n\n";
						system("pause");
						break;
					case 6: system("cls");
						who_user = -1;
						break;
					case 0: break;
					default: cout << "Нет такого варианта!\n";
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