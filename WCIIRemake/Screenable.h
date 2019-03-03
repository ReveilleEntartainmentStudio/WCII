#pragma once
#include "pch.h"
#include "Obj.h"

class MScreen;


// ��������� ������ ��� �������� ���������
struct cordScr : public Obj {
	int x;
	int y;
	cordScr(int x, int y) : x(x) , y(y) {}
	cordScr() : x(0), y(0) {}

	friend bool operator==(const cordScr left, cordScr right) {
		return (left.x == right.x) && (left.y == right.y);
	}
};

// ����� ��� ���� ���������, ������� ����� ���������� �� ������ (����������� �� ����)
class Screenable : public Obj {
	protected:
		cordScr cords;
		int width;
		int heigth;
		MScreen* scr;
	public:
		Screenable(cordScr cords, int width, int heigth, MScreen* scr);
		Screenable();
		virtual ~Screenable();
		int getWidth();
		int getHeigth();
		cordScr getCord(); //���������� ���������� 
		int setCord(cordScr cords); // ���������� ����������
		int setup(cordScr cords, int width, int heigth, MScreen* scr); // ���������������� ������� (�������� ����������, ���� �������� �������, ��� ������ � ������ � ��������� �� �����, ��� �� ����� ��������������)
		virtual void render(); // ������������(������������)
};

