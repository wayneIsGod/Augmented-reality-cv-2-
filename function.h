#pragma once
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>
using namespace cv;
using namespace std;
Mat backgroundClear(Mat image, Mat& background, int x, int y);
void pictback(Mat& image, Mat& background, int x, int y);
Mat cut(Mat video0, Mat video1, VideoWriter& writer, VideoWriter& writer0,
	VideoWriter& writer1);
void bb_sort(double* input, int* record, int size);