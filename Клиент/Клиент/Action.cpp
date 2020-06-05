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

//------------------------Ввод числа------------------------------------------------------------------------
template <typename T>
void Get_number(T& chislo) {
	while (true) { // Цикл продолжается до тех пор, пока пользователь не введет корректное значение
		cin >> chislo;
		cin.ignore(32767, '\n');

		if (cin.fail()) { // если предыдущее извлечение оказалось провальным
			cout << "Ошибка при вводе числа. Повторите ввод: ";
			cin.clear(); // возвращаем cin в 'обычный' режим работы
			cin.ignore(32767, '\n');
		}
		else {
			break;
		}
	}
}

//------------------------Вывод текущей даты и времени------------------------------------------------------------
void Time() {
	time_t now = time(0);
	tm* ltm = localtime(&now);

	cout << "Дата  " <<
		setw(2) << setfill('0') << right << ltm->tm_mday << "." <<
		setw(2) << setfill('0') << right << ltm->tm_mon + 1 << "." <<
		ltm->tm_year + 1900 << " " <<
		"\nВремя " <<
		setw(2) << setfill('0') << right << ltm->tm_hour << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_min << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_sec << endl;
	cout.fill(' ');
}

// Логирование-------------------------------------------------------------------------------------------------
int Login(string &l) {
	try {
		string log, pass;
		char buf[200];
		char result[200];

		cout << "Введите логин: ";
		cin >> log;
		cout << "Введите пароль: ";
		cin >> pass;
		string str_buf = "L " + log + " " + pass;

		strcpy(buf, str_buf.c_str());

		int Otvet;
		Otvet = Server(buf, result);
		if (Otvet != 0) {
			throw MyException("Нет связи с сервером.");
		}
		l = log;
		return atoi(result);
	}
	catch (MyException exc) {
		cout << exc.Get_error() << endl;
		system("pause");
	}
}

//Управление пользователями-----------------------------------------------------------------
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
			else throw MyException("Не удалось прочитать файл с логинами и паролями.");
			cout << "1. Удаление пользователя\n";
			cout << "2. Добавление пользователя\n";
			cout << "0. Выход из управления пользователями\n\n";
			cout << "Ваш выбор: ";

			Get_number(choise);
			cout << "\n";

			switch (choise) {
			case 1:
				cout << "\nВведите логин пользователя, которого хотите удалить: ";
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
				cout << "Введите данные о пользователе:\n" << "Логин: ";
				cin >> log;
				cout << "Пароль: ";
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
				cout << "Нет такого варианта!\n";
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

//Ввод числа (с пояснительной строкой)-----------------------------------------------------------
template <typename T, typename T1>
T Get_number_s(T1 str, T min_n, T max_n) {
	T chislo; 
	while (true) { //Пока ввод не будет корректным
		cout << str;
		cin >> chislo;
		if (cin.good()) { //если нет ошибок
			cin.ignore(32767, '\n');
			if (chislo >= min_n && chislo <= max_n) {
				return chislo;
			}
			else { cout << "Число в диапазоне " << min_n << " - " << max_n << endl; }
		}
		else {
			cin.clear(); // Очистить биты ошибок
			cin.ignore(32767, '\n');
			cout << "Неправильный ввод числа\n";
		}
	}
}

//Справочник экспертов--------------------------------------------------------------------------
void Spr_expert() {
	int j, n, skill;
	string fio, position;
	while (true) {
		cout << "\nЭксперты предприятия:" << endl;
		for (int i = 0; (i < spr_expert.size()); i++) {
			cout << i + 1 << "." << spr_expert[i] << endl;
		}

		if (spr_expert.size() == 0)
			cout << "\nДобавить - 1, Выход - 0\n";
		else
			cout << "\nДобавить - 1, Изменить - 2, Удалить - 3, Выход - 0\nВаш выбор: ";

		Get_number(n);
		if (n == 0) {
			Write_expert();
			break;
		}
		switch (n) {
		case 1:
			cout << "\nДолжность: ";
			getline(cin, position);
			cout << "\nФИО: ";
			getline(cin, fio);
			skill = Get_number_s("\nКомпетентность: ", 1, 10);
			spr_expert.push_back(Expert(fio, position, skill));
			break;
		case 2:
			while (true) {
				j = Get_number_s("Укажите номер списка для редактирования: ", 0, (int)spr_expert.size());
				if (j == 0) break;

				cout << "\nДолжность: ";
				getline(cin, position);
				cout << "\nФИО: ";
				getline(cin, fio);
				skill = Get_number_s("\nКомпетентность: ", 1, 10);
				spr_expert[j-1] = Expert(fio, position, skill);
				cout << "\nОтредактировано.\n";
				break;
	
			}
			break;
		case 3:
			while (true) {
				j = Get_number_s("Укажите номер списка для удаления: ", 0, (int)spr_expert.size());
				if (j == 0)  break; 

				spr_expert.erase(spr_expert.begin() + j - 1);
				cout << "\nУдалено.\n";
				break;
			}
			break;
		default:
			break;
		}
	}
}
//Справочник целей-----------------------------------------------------------------------------------
void Spr_goal() {
	int j, n;
	string goal;
	while (true) {
		cout << "\nЦели:" << endl;
		for (int i = 0; (i < spr_goal.size()); i++) {
			cout << i + 1 << "." << spr_goal[i] << endl;
		}

		if (spr_goal.size() == 0)
			cout << "\nДобавить - 1, Выход - 0\n";
		else
			cout << "\nДобавить - 1, Изменить - 2, Удалить - 3, Выход - 0\nВаш выбор: ";

		Get_number(n);
		if (n == 0) {
			Write_goal();
			break;
		}
		switch (n) {
		case 1:
			cout << "\nЦель: ";
			getline(cin, goal);
			spr_goal.push_back(goal);
			break;
		case 2:
			while (true) {
				j = Get_number_s("Укажите номер списка для редактирования: ", 0, (int)spr_goal.size());
				if (j == 0) break;
		
				cout << "\nЦель: ";
				getline(cin, goal);
				auto it = spr_goal.begin(); // итератор для замены данных в векторе
				*(it + j - 1) = goal;
				cout << "\nОтредактировано\n";
				break;
			}
			break;
		case 3:
			while (true) {
				j = Get_number_s("Укажите номер списка для удаления: ", 0, (int)spr_goal.size());
				if (j == 0) break; 

				spr_goal.erase(spr_goal.begin() + j - 1);
				cout << "\nУдалено.\n";
				break;
			}
			break;
		default:
			break;
		}
	}
}
// Проблема для решения----------------------------------------------------------------------------
void New_problem() {
	int i, n;

	while (true) {
		system("cls");
		Show_problem();

		cout << "\n1.Проблема \n2.Цели \n3.Эксперты \n4.Получить результат \n0.Выход \nВаш выбор: ";
		Get_number(n);
		if (n == 0) {
			break;
		}
		switch (n) {
		case 1:
			cout << "\nСформулируйте проблему для решения: ";
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
//Выбор экспертов----------------------------------------------------------------------------------
void Choose_expert() {
	int i, j, k;
	cout << "\nЭксперты предприятия:" << endl;
	for (int i = 0; (i < spr_expert.size()); i++) {
		cout << i + 1 << "." << spr_expert[i] << endl;
	}
	experts.clear();
	char buf[200];
	char* nstr = buf; // в начале указывает на начало строки str
	char* tstr; // Указатель, используемый для определения завершения  преобразования строки.

	cout << "Список через пробел: " << endl;
	cin.getline(buf, 200);
	while (true) {
		tstr = nstr;
		// Преобразуем очередной участок строки
		k = strtod(nstr, &nstr);
		// Проверяем вся ли строка преобразована и если вся строка  преобразована выходим из цикла
		if (k == 0 && tstr == nstr) break;
		if (k > 0 && k <= spr_expert.size()) experts.push_back(spr_expert[k - 1]);
	}
}

// Выбор целей для решения--------------------------------------------------------------------------
void Choose_goal() {
	int i, j, k;
	cout << "\nЦели предприятия:" << endl;
	for (int i = 0; (i < spr_goal.size()); i++) {
		cout << i + 1 << "." << spr_goal[i] << endl;
	}
	goals.clear();
	char buf[200];
	char* nstr = buf; // в начале указывает на начало строки str
	char* tstr; // Указатель, используемый для определения завершения  преобразования строки.

	cout << "Список через пробел: " << endl;
	cin.getline(buf, 200);
	while (true) {
		tstr = nstr;
		// Преобразуем очередной участок строки
		k = strtod(nstr, &nstr);
		// Проверяем вся ли строка преобразована и если вся строка  преобразована выходим из цикла
		if (k == 0 && tstr == nstr) break;
		if (k > 0 && k <= spr_goal.size()) goals.push_back(spr_goal[k - 1]);
	}
}

// Показать проблему---------------------------------------------------------------------------------
void Show_problem() {
	cout << "Проблема: " << problem << endl;
	cout << "\nВыбранные цели: " << endl;

	vector<string>::const_iterator it; // итератор для вывода на экран
	it = goals.begin();
	while (it != goals.end()) {
		cout << *it << endl;
		++it;
	}

	cout << "\nЭксперты для оценки: " << endl;
	for (int i = 0; (i < experts.size()); i++) {
		cout << experts[i] << endl;
	}
}

//Введение экспертных оценок и получение результата от сервера-----------------------------------------
void Solve() {
	char char_marks[200]; // оценки через пробел
	char* nstr; // Указатель, на непреобразованный остаток строки str, в начале указывает на начало строки buf
	char* tstr; // Указатель, используемый для определения завершения преобразования строки.
	
	float mark;
	string str;
	char buf[200], result[200];
	
	// занесение оценок экспертов
	for (int i = 0; i < experts.size(); i++) {
		while (true) {
			cout << endl << i + 1 << "-й эксперт: " << experts[i].Get_position() << " " << experts[i].Get_FIO() << "\nЕго оценки:\n";
			cin.getline(char_marks, 200); //все оценки одного эксперта через пробел

			marks.clear(); // проблема здесь!
			nstr = char_marks;
			while (true) {
				tstr = nstr;
				mark = strtod(nstr, &nstr); // Преобразуем очередной участок строки в число
				if (mark == 0 && tstr == nstr) break; // Если вся ли строка преобразована - выходим из цикла
				marks.push_back(mark); // добавление оценки в вектор оценок
			}
			marks.resize(goals.size(), 0); // размерность вектора оценок = кол-ву целей
			experts[i].Set_marks(marks);
			if (experts[i].Sum_marks() == 1) { // сумма оценок одного эксперта д.б. = 1
				break;
			}
		}
	}
	
	//строка для передачи серверу
	str = "M"; // признак M - метод
	for (int i = 0; i < experts.size(); i++) {
		str += experts[i].Get_skill_and_marks() + " 99 "; //компетентность + оценки + 99(признак окончания инфы одного эксперта)
	}

	strcpy(buf, str.c_str());

	//отправка на сервер запроса и получение ответа
	int Otvet = Server(buf, result);

	//расшифоровка ответа char_marks
	marks.clear();
	nstr = result;
	while (true) {
		tstr = nstr;
		mark = strtod(nstr, &nstr); // Преобразуем очередной участок строки в число
		if (mark == 0 && tstr == nstr) break; // Если вся ли строка преобразована - выходим из цикла
		marks.push_back(mark); // добавление оценки в вектор оценок
	}
	
	//запись результатов решения в файлы 
	string filename = "PROBLEM.TXT";
	Write_result(filename);
	//вывод  на экран
	Display_file(filename);
}

// Отправка запроса на сервер и получение ответа---------------------------------------------------------
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