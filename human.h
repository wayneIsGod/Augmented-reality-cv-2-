#pragma once
#include "function.h"

class human
{
private:
	int m_x;
	int m_y;
	int m_xEnd;
	int m_yEnd;
	int m_direction;
	int m_faceID;
	int m_faceSize;
public:
	human();
	human(int x, int y, int xEnd, int yEnd, int faceSize);
	virtual ~human();
	int get_x();
	int get_y();
	int get_faceID();
	void walk();
	void print(Mat show_img, vector<Mat> face);
};

