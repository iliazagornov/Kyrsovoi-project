#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
	string m_FIO;
public:
	Person(string FIO = "") {
		m_FIO = FIO;
	}
	void Set_FIO(string FIO = "") {
		m_FIO = FIO;
	}
	string Get_FIO() const {
		return m_FIO;
	}
	string Who() const {
		return m_FIO + " ";
	}
};

class Position {
	string m_position;
public:
	Position(string position = "") {
		m_position = position;
	}
	void Set_position(string position = "") {
		m_position = position;
	}
	string Get_position() const {
		return m_position;
	}
	string  Who() const {
		return m_position + " ";
	}
};

class Expert :public Person, public Position {
	int m_skill;
public:
	Expert(string FIO = "?", string position = "?", int skill = 1): Person(FIO), Position(position) {
		m_skill = skill;
	}
	Expert(const Expert &expert) { // конструктор копирования
		m_skill = expert.m_skill;
		Set_FIO(expert.Get_FIO());
		Set_position(expert.Get_position());
	}
	int Get_skill() {
		return m_skill;
	}
	void Set_expert(const Expert& expert) {
		m_skill = expert.m_skill;
		Set_FIO(expert.Get_FIO());
		Set_position(expert.Get_position());
	}
	string Who() const {
		return Position::Who() + Person::Who();
	}
	friend ostream& operator<< (ostream& out, const Expert& expert);
};

class ExpertMark : public Expert {
	vector <float> m_marks;  //оценки эксперта 
public:
	ExpertMark(Expert expert) {
		Set_expert(expert);
	}
	void Set_marks(vector <float> marks) {
		m_marks = marks;
	}
	float Sum_marks() {
		float s = 0;
		for (int i = 0; i < m_marks.size(); i++) {
			s += m_marks[i];
		}
		return s;
	}
	string Get_skill_and_marks() {
		string str;
		str = to_string(this->Get_skill()) + " ";
		for (int i = 0; i < m_marks.size(); i++) { //оценки через пробел
			str += to_string(m_marks[i]) + " ";
		}
		return str;
	}
};

class MyException {
private:
	string m_error;

public:
	MyException(string error) {
		m_error = error;
	}
	const char* Get_error() { return m_error.c_str(); }
};