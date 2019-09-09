#pragma once
#include"function.h"
class sword
{
private:
	int m_x;
	int m_y;
	int m_xEnd;
	int m_yEnd;
	int m_xSpeed;
	int m_ySpeed;
	int m_swordID;
	int m_swordSize;
public:
	sword();
	sword(int x, int y, int xEnd, int yEnd, int xSpeed, int ySpeed, int swordSize);
	virtual ~sword();
	int get_x();
	int get_y();
	int get_xSpeed();
	int get_ySpeed();
	int get_swordID();
	void walk();
	void print(Mat show_img, Mat* sword);
};

