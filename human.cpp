#include "human.h"
human::human(){}
human::human(int x,int y, int xEnd, int yEnd, int faceSize):
	m_x(x),m_y(y), m_xEnd(xEnd), m_yEnd(yEnd), m_direction(0), m_faceSize(faceSize)
{
	srand(time(NULL));
	m_faceID = rand() % 6;
}
human::~human()
{
}
int human::get_x()
{
	return m_x;
}
int human::get_y()
{
	return m_y;
}
int human::get_faceID()
{
	return m_faceID;
}
void human::walk()
{
	if (m_y < (m_yEnd-60 - m_faceSize))
	{
		m_y = m_y + 40;
	}
}

void human::print(Mat show_img, vector<Mat> face)
{
	Mat temp=backgroundClear(face[m_faceID], show_img, m_x, m_y);
	pictback(temp, show_img, m_x, m_y);
}