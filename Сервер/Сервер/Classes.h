#pragma once
#include <vector>
using namespace std;

class ExpertMark {
	static int m_all_level; //сумма компетентности всех экспертов
	int m_level;            //компетентность эксперта
	vector <float> m_marks;  //оценки эксперта 
public:
	ExpertMark(int level = 0) {
		m_level = level;
	}
	void Add_mark(float mark) {
		m_marks.push_back(mark);
	}
	vector <float> Get_mark() {
		return m_marks;
	}
	int Get_level() {
		return m_level;
	}

	static void Erase_all_level() {
		m_all_level = 0;
	}
	static void Add_all_level(int level = 0) {
		m_all_level = m_all_level + level;
	}
	static int Get_all_level() {
		return m_all_level;
	}
};