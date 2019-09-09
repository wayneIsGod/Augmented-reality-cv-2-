#include "sword.h"
sword::sword(){}
sword::sword(int x, int y, int xEnd, int yEnd, int xSpeed, int ySpeed, int swordSize):
	m_x(x), m_y(y), m_xEnd(xEnd), m_yEnd(yEnd), m_xSpeed(xSpeed), m_ySpeed(ySpeed), m_swordSize(swordSize)
{
	srand(time(NULL)*1000*x);
	m_swordID = rand() % 3;
}
sword::~sword()
{
}
int sword::get_x()
{
	return m_x;
}
int sword::get_y()
{
	return m_y;
}
int sword::get_xSpeed()
{
	return m_xSpeed;
}
int sword::get_ySpeed()
{
	return m_ySpeed;
}
int sword::get_swordID()
{
	return m_swordID;
}
void sword::walk()
{
	m_x = m_x + m_xSpeed;
	m_y = m_y + m_ySpeed;
}
void sword::print(Mat show_img, Mat* sword)
{
	Mat temp = backgroundClear(sword[m_swordID], show_img, m_x, m_y);
	pictback(temp, show_img, m_x, m_y);
}