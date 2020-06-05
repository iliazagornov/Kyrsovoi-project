#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Classes.h"
#include "Read_write.h"
#include "Action.h"

using namespace std;
extern vector <Expert> spr_expert;
extern vector <string> spr_goal;
extern vector <ExpertMark> experts;
extern vector <string> goals;
extern vector <float> marks;
extern string problem;

//���������� ����� �� ������� �� ��������
string Max_marks() {
	int i_max = 0;
	float max_mark = marks[0];
	for (int i = 0; i < marks.size(); i++) {
		if (marks[i] > max_mark) {
			i_max = i;
			max_mark = marks[i];
		}
	}
	marks[i_max] = -1;
	return goals[i_max] + " - " + to_string(max_mark);
}

//������ �� ����� EXPERT.txt---------------------------------------------------------------------------------------------------
void Read_expert() {
	int found1, found2;
	string strInput;
	ifstream inf;
	inf.open("EXPERT.TXT");

	while (inf) { // ���� ���� ������, ������� �� ����� ���������
		getline(inf, strInput); // ������ ������ �����
		if (strInput.length() == 0) { // ��������� ������ - ��� ������
			break;
		}
		found1 = strInput.find(";");
		found2 = strInput.find(";", found1 + 1);

		spr_expert.push_back(Expert(strInput.substr(0, found1),
			strInput.substr(found1 + 1, found2 - found1 - 1),
			stoi(strInput.substr(found2 + 1))));
	}
	inf.close();
}

void Write_expert() {
	try {
		ofstream ouf;
		ouf.open("EXPERT.TXT", ios::trunc);
		
		for (int i = 0; i < spr_expert.size(); i++) {
			ouf << spr_expert[i].Get_FIO() + ";" + spr_expert[i].Get_position() + ";" + to_string(spr_expert[i].Get_skill()) << "\n";
			if (!ouf) {
				throw runtime_error("\n������ � ���� EXPERT.TXT ����������\n");
			}
		}
		ouf.close();
	}
	catch (runtime_error err) { cout << err.what() << endl; system("pause"); } 
}

// ������ �� ����� GOAL.txt-------------------------------------------------------------------------------------------------- -
void Read_goal() {
	string strInput;
	ifstream inf;
	inf.open("GOAL.TXT");

	while (inf) { // ���� ���� ������, ������� �� ����� ���������
		getline(inf, strInput); // ������ ������ �����
		if (strInput.length() == 0) { // ��������� ������ - ��� ������
			break;
		}
		spr_goal.push_back(strInput);
	}
	inf.close();
}

//������ � ���� GOAL.txt---------------------------------------------------------------------------------------------------
void Write_goal() {
	try {
		ofstream ouf;
		ouf.open("GOAL.TXT", ios::trunc);
	
		for (int i = 0; i < spr_goal.size(); i++) {
			ouf << spr_goal[i] << "\n";
			if (!ouf) {
				throw runtime_error("\n������ � ���� GOAL.TXT ����������\n");
			}
		}
		ouf.close();
	}
	catch (runtime_error err) { cout << err.what() << endl; system("pause"); } 
}

//������ � ���� ������ ARCHIVE.TXT----------------------------------------------------------------------------------
void Write_result(string filename) {
	ofstream ouf_arch, ouf; // ������� ofstream
	int A;
	A=Get_number_s("\n�������� ��������� � ����� �������? 1 - ��, 0 - ���\n��� �����: ", 0, 1);
	if (A == 1) {
		ouf_arch.open("ARCHIVE.TXT", ios::app);
	}
	ouf.open(filename);

	time_t now = time(0);
	tm* ltm = localtime(&now);

	ouf << "����:  " <<
		setw(2) << setfill('0') << right << ltm->tm_mday << "." <<
		setw(2) << setfill('0') << right << ltm->tm_mon + 1 << "." <<
		ltm->tm_year + 1900 << " " <<
		"\n�����: " <<
		setw(2) << setfill('0') << right << ltm->tm_hour << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_min << ":" <<
		setw(2) << setfill('0') << right << ltm->tm_sec << endl;
	ouf.fill(' ');

	if (A == 1) {
		ouf_arch << "\n_________________________________________________________________________";
		ouf_arch << "\n����:  " <<
			setw(2) << setfill('0') << right << ltm->tm_mday << "." <<
			setw(2) << setfill('0') << right << ltm->tm_mon + 1 << "." <<
			ltm->tm_year + 1900 << " " <<
			"\n�����: " <<
			setw(2) << setfill('0') << right << ltm->tm_hour << ":" <<
			setw(2) << setfill('0') << right << ltm->tm_min << ":" <<
			setw(2) << setfill('0') << right << ltm->tm_sec << endl << endl;
		ouf_arch.fill(' ');

		ouf_arch << "��������: " << endl;
	}

	ouf << "��������: " << endl;
	for (int i = 0; (i < experts.size()); i++) {
		if (A == 1) {
			ouf_arch << experts[i].Who() << endl;
		}
		ouf << experts[i].Who() << endl;
	}

	if (A == 1) {
		ouf_arch << "\n��������: " << problem << "\n\n����: " << endl;
	}
	ouf << "\n��������: " << problem << "\n\n����: " << endl;

	string str;
	for (int i = 0; i < marks.size(); i++) {
		str = Max_marks();
		ouf_arch << str << endl;
		ouf << str << endl;
	}
	ouf_arch.close();
	ouf.close();
}

//����� �� ����� ����������� �����
void Display_file(string filename) {
	string str;
	ifstream inf;//������� ifstream
	system("cls");
	inf.open(filename);
	while (inf) { // ���� ���� ������, ������� �� ����� ���������
		getline(inf, str); // ������ ������ �����
		cout << str << endl;
	}
	inf.close();
	system("pause");
}