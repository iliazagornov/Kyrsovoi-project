#pragma once
#include <string>
using namespace std;
template <typename T, typename T1>
T Get_number_s(T1 str, T min_n, T max_n);
void Spr_expert();
void Show_problem();
void Write_expert();
void Spr_goal();
void Write_goal();
void New_problem();
void Choose_expert();
void Choose_problem();
void Choose_goal();
void Solve();
int Server(char*  buf, char*  result);
void Time();
template <typename T>
void Get_number(T& chislo);
int Login(string &l);
void Users();