#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Classes.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

using namespace std;

extern FILE* report_file;
struct about_users {   //данные о пользователях   
	char login[20];
	char password[30];
	int role;
};
about_users users[100];

//Вычисление экспертной оценка-------------------------------------------------
void Metod(char* str, char* result) {
	string sResult;
	float chislo;
	vector <ExpertMark> expertMark;
	int new_expert = 1;
	ExpertMark::Erase_all_level(); // обнуление суммы компетентности всех экспертов

	// Указатель, на непреобразованный остаток строки str, в начале указывает на начало строки buf
	char* nstr = str;
	// Указатель, используемый для определения завершения преобразования строки.
	char* tstr;

	// Пришедшую от клиента строку - разобрать по экспертам.
	// buf - строка, где m экспертов, у каждого - его компетентность и n оценок 
	// компет1 оценка11 оценка12 ...оценка1n 99 компет2 оценка21 оценка22 ...оценка2n 99 ... 99 компетm оценкаm1 оценкаm2 ...оценкаmn ;
	while (true) {
		tstr = nstr;
		// Преобразуем очередной участок строки в число
		chislo = strtod(nstr, &nstr);
		// Проверяем вся ли строка преобразована и если вся строка  преобразована - выходим из цикла
		if (chislo == 0 && tstr == nstr) break;

		if (chislo == 99) {
			new_expert = 1;
		}
		else {
			if (new_expert == 1) {
				expertMark.push_back(chislo); // добавим эксперта и его компетентность
				new_expert = 0;
				ExpertMark::Add_all_level(chislo); //добавим  его компетентность к сумме
			}
			else {
				expertMark.back().Add_mark(chislo); //добавим оценку
			}
		}
	}

	//Высчитать взвешенные оценки для целей
	vector <float> result_mark;  //искомые веса
	float mark = 0, sum_mark = 0;
	for (int j = 0; j < expertMark[0].Get_mark().size(); j++) {
		mark = 0;
		for (int i = 0; i < expertMark.size(); i++) {
			mark = mark + expertMark[i].Get_mark()[j] * expertMark[i].Get_level() / expertMark[i].Get_all_level();
		}

		if (j == expertMark[0].Get_mark().size() - 1 &&
			sum_mark + mark != 1) { // для последней оценку м.б. нужна корректировка, чтобы суммы оценок = 1
			mark = 1 - sum_mark;
		}
		sum_mark = sum_mark + mark;

		sResult.append(to_string(mark));
		sResult.append(" ");
	}
	strcpy(result, sResult.c_str());
}

//------------------------Шифрование------------------------------------------------------------------------
void Crypt(char* password) {
	for (int i = 0; i < strlen(password); i++)
	{
		password[i] -= 7;
		if (password[i] > 175)
			password[i] += 49;
		if (password[i] > 241)
			password[i] -= 208;
	}
}

//Проверка пользователя----------------------------------------------------------------------------------------
void Metod_log(char* str, char* result) {
	FILE* log_file;
	int i = 0, role;
	char log[20], pass[30], login[20], password[30];

	if ((log_file = fopen("Login_and_password.txt", "r")) == NULL) {
		strcpy(result, to_string(-2).c_str());
		return;
	}

	char * pch = strtok(str, " "); // во втором параметре указаны разделитель (пробел)

	while (pch != NULL) {   
		if (i == 0) {
			strcpy(log, pch);
		}
		else {
			strcpy(pass, pch);
		}
		pch = strtok(NULL, " ");
		i++;
	}

	Crypt(pass);

	while (fscanf(log_file, "%s%s%d", login, password, &role) != EOF) {
		if (strcmp(log, login) == 0 && strcmp(pass, password) == 0) {
			strcpy(result, to_string(role).c_str());
			return;
		}
	}
	strcpy(result, to_string(-1).c_str());
	fclose(log_file);
}

//------------------------Управление пользователями(добавление)--------------------------------------------------
void Users_add(char* str, char* result) {
	//char log[20], pass[30];
	int i = 0; 

	ofstream outf("Login_and_password.txt", ios::app);

	if (!outf) { // если мы не можем открыть этот файл для записи его содержимого
		strcpy(result, to_string(-2).c_str());
		return;
	}

	char sep[10] = " ";
	char * pch = strtok(str, sep); // во втором параметре разделители

	while (pch != NULL) {
		if (i == 0) { 
			outf << pch; //логин
			strcpy(result, "Пользователь ");
			strcat(result, pch);
			strcat(result, " добавлен.");
		}
		else { 
			Crypt(pch); //пароль
			outf << " " << pch << " 2" << endl;
		}
	pch = strtok(NULL, sep);
	i++;
	}
	
	outf.close();

	//strcpy(result, to_string(1).c_str());
}
//------------------------Управление пользователями(удаление)------------------------------------------------
void Users_del(char* str, char* result) {
	char log[20], pass[30], login[20], password[30];
	int i = 0, kol_users = 0;
	ifstream inf("Login_and_password.txt");

	if (!inf) { // если мы не можем открыть этот файл для чтения его содержимого
		strcpy(result, to_string(-2).c_str());
		return;
	}

	while (inf) {
		inf >> users[i].login;
		inf >> users[i].password;
		inf >> users[i].role;
		i++;
	}
	inf.close();
	kol_users = i - 1;
	char * pch = strtok(str, " "); // во втором параметре  разделитель (пробел)

	ofstream outf("Login_and_password.txt");

	if (!outf) { // если мы не можем открыть этот файл для записи его содержимого
		strcpy(result, to_string(-2).c_str());
		return;
	}

	strcpy(result, "Пользователь не найден.");
	for (int i = 0; i <= kol_users - 1; i++) {
		if (strcmp(users[i].login, pch) != 0) { // это не пользователь для удаления
			outf << users[i].login << " " << users[i].password << " " << users[i].role << endl;
		}
		else {
			strcpy(result, "Пользователь ");
			strcat(result, users[i].login);
			strcat(result, " удалён.");
		}
	}
	outf.close();
}

//------------------------Вывод списка пользователей------------------------------------------------
void Users_list(char* str, char* result) {
	char log[20];
	int i = 0;//, kol_users = 0;
	ifstream inf("Login_and_password.txt");

	if (!inf) { // если мы не можем открыть этот файл для чтения его содержимого
		strcpy(result, to_string(-2).c_str());
		return;
	}

	strcpy(result, "Пользователи:\n\n");
	while (inf) {
		inf >> users[i].login;
		inf >> users[i].password;
		inf >> users[i].role;
		strcpy(log, users[i].login);
		strcat(result, log);
		strcat(result, "\n");
		i++;
	}
	inf.close();
}