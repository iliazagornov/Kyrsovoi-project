#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "Classes.h"
#include <iomanip>

using namespace std;

ostream& operator<< (ostream& out, const Expert& expert) {
	out << setw(30) << std::left << expert.Get_position() << " "
		<< setw(25) << std::left << expert.Get_FIO() << "- "
		<< setw(2) << std::right << expert.m_skill;
	return out;
}