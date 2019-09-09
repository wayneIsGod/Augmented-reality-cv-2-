#pragma once
#include"human.h"
#include"sword.h"
struct sword_resure
{
	const int resure_x = 320;
	const int resure_y = 360;
};
void backgroundClear(Mat& image, Mat& background);
void Drawtoback(Mat& image, Mat& background, int x, int y);
void swordGame(human* person, human** people, sword* swordPtr, sword** swords, sword** inv_swords,
	int& people_num, int& sword_num, vector<Mat>& face, Mat* sword_face, Mat* inv_sword_face,
	Mat show_img, vector<vector<Point> >& contours, vector<vector<Point> > hull, int& hull_record, 
	int& point, int& power_num, string talk[], const int face_size, const int sword_size);