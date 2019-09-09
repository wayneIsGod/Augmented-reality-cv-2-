#pragma once
#include "human.h"
void skin_color(Mat& color, Mat_<uchar>& mask, int R_thr, int G_thr, int B_thr,
	int Max_min_diff_thr, int R_G_diff_thr);
Mat Contours(Mat& input, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy);
int hand();