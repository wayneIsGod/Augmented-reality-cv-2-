#include"people.h"
#include<time.h>
void sword_speed(vector<vector<Point> > hull, int& hull_record, sword_resure swordResure, int& x_speed, int& y_speed, int &i)
{
	x_speed = hull[hull_record][i].x - swordResure.resure_x;
	y_speed = hull[hull_record][i].y - swordResure.resure_y;
	if (fabs(hull[hull_record][i].x - swordResure.resure_x) > 40)
	{
		x_speed = (hull[hull_record][i].x - swordResure.resure_x) * 40 / fabs(hull[hull_record][i].x - swordResure.resure_x);
	}
	if (fabs(hull[hull_record][i].y - swordResure.resure_y) > 40)
	{
		y_speed = (hull[hull_record][i].y - swordResure.resure_y) * 40 / fabs(hull[hull_record][i].y - swordResure.resure_y);
	}
}
void bidPower(human** people, sword** swords, Mat* sword_face, Mat& show_img, int& people_num, int& sword_num, int& power_num, const int sword_size, const int bigPower_num)
{
	if (power_num > 0)
	{
		for (int i = 0; i < bigPower_num; i++)
		{
			delete swords[i];
			swords[i] = 0;
		}
	}
	power_num = 5;
	for (int i = 0; i < bigPower_num; i++)
	{
		srand((unsigned)time((NULL)) * 1000 * i);
		swords[i] = new sword(((int)(((show_img.cols - sword_size) - 1) * (double)rand() / RAND_MAX)), ((int)(((show_img.rows - sword_size) - 1) * (double)rand() / RAND_MAX)), show_img.cols, show_img.rows, 0, 80, sword_size);
	}
	for (int i = 0; i < bigPower_num; i++)
	{
		if (swords[i]->get_y() < (show_img.rows - 80 - sword_size))
		{
			swords[i]->walk();
		}
		swords[i]->print(show_img, sword_face);
	}
	for (int i = 0; i < people_num; i++)
	{
		delete people[i];
		people[i] = 0;
	}
	people_num = 0;
	power_num--;
	putText(show_img, string("ten thousand swords"), Point(show_img.cols / 2 - 170, 100), 0, 1, Scalar(255, 150, 0), 2);
}

void swordGame(human* person, human** people, sword* swordPtr, sword** swords, sword** inv_swords,
	int& people_num, int& sword_num, vector<Mat>& face, Mat* sword_face, Mat* inv_sword_face,
	Mat show_img, vector<vector<Point> >& contours, vector<vector<Point> > hull, int& hull_record,
	int& point, int& power_num, string talk[], const int face_size, const int sword_size)
{
	sword_resure swordResure;
	string pointch;
	circle(show_img, Point(swordResure.resure_x, swordResure.resure_y), 5, Scalar(0, 0, 255), -1);
	if (people_num < 20)
	{
		int temp = 0;
		srand((unsigned)time(NULL));
		int kk = rand() % ((show_img.cols - face_size) - 1);
		person = new human((rand() % ((show_img.cols - 2 * face_size) - 1)), 0, show_img.cols, show_img.rows, face_size);
		for (int i = 0; i < people_num; i++)
		{
			if (people[i] == 0)
			{
				people[i] = person;
				temp = 1;
				break;
			}
		}
		if (temp == 0)
		{
			people[people_num] = person;
		}
		people_num++;
	}
	for (int i = 0; i < people_num; i++)
	{
		if (people[i] == 0)
		{
			continue;
		}
		people[i]->walk();
		people[i]->print(show_img, face);
		pointch = to_string(point);
		putText(show_img, string("point:"), Point(show_img.cols / 2 - 100, 50), 0, 1, Scalar(0, 150, 255), 2);
		putText(show_img, pointch, Point(show_img.cols / 2, 50), 0, 1, Scalar(0, 150, 255), 2);
		if (people[i]->get_y() >= show_img.rows - 60 - face_size)
		{
			delete people[i];
			people[i] = 0; 
			people_num--;
		}
	}
	if (hull_record > 0 && sword_num < 20)
	{
		for (int i = 0; i < hull[hull_record].size(); i = i + 4)
		{
			if (hull[hull_record][i].x > (3 / 2)*sword_size&&hull[hull_record][i].x< show_img.cols - sword_size / 2 && hull[hull_record][i].y>(3 / 2)*sword_size&&hull[hull_record][i].y < show_img.rows - sword_size / 2)
			{
				int x_speed = 40, y_speed = 40;
				sword_speed(hull, hull_record, swordResure, x_speed, y_speed, i);
				swordPtr = new sword(swordResure.resure_x - sword_size / 2, swordResure.resure_y - sword_size / 2, show_img.cols, show_img.rows, x_speed, y_speed, sword_size);
				if (swords[i] == 0)
				{
					swords[i] = swordPtr;
					break;
				}
				swords[sword_num] = swordPtr;
				sword_num++;
			}
		}
	}
	int tap = 0;
	for (int i = 0; i < sword_num; i++)
	{
		if (swords[i] == 0)
		{
			continue;
		}
		if (tap == 1)
		{
			break;
		}
		for (int t = 0; t < people_num; t++)
		{
			if (people[t] == 0)
			{
				continue;
			}
			if (abs(swords[i]->get_x() - people[t]->get_x()) < 50 && abs(swords[i]->get_y() - people[t]->get_y()) < 50)
			{
				srand(time(NULL));
				putText(show_img, string(talk[rand() % 6]), Point(people[t]->get_x(), people[t]->get_y()), 0, 1, Scalar(0, 0, 255), 2);
				delete people[t];
				people[t] = 0;
				people_num--;
				point++;
				tap = 1;
				break;
			}
		}
	}
	for (int i = 0; i < sword_num; i++)
	{
		if (swords[i] == 0)
		{
			continue;
		}
		if ((swords[i]->get_x() > -swords[i]->get_xSpeed() + (1/2)*sword_size) && (swords[i]->get_x() < -swords[i]->get_xSpeed() + show_img.cols - (3/2)*sword_size) && (swords[i]->get_y() > -swords[i]->get_ySpeed() + (1/2)*sword_size) && (swords[i]->get_y() < -swords[i]->get_ySpeed() + show_img.rows - (3/2)*sword_size))
		{
			swords[i]->walk();
			swords[i]->print(show_img, sword_face);
		}
		else
		{
			delete swords[i];
			swords[i] = 0; 
			sword_num--;
		}
	}
	const int bigPower_num = 100;
	if (point >= 100)
	{
		point = 0;
		bidPower(people, inv_swords, inv_sword_face, show_img, people_num, sword_num, power_num, sword_size, bigPower_num);
	}
	else if (power_num > 0 && power_num < 5)
	{
		for (int i = 0; i < bigPower_num; i++)
		{
			if (inv_swords[i]->get_y()< (show_img.rows - 80 - sword_size))
			{
				inv_swords[i]->walk();
			}
			inv_swords[i]->print(show_img, inv_sword_face);
		}
		power_num--;
	}
}