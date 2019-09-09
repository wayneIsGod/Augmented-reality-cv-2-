#include"interestPoint.h"
bool return_small(Point2f a, Point2f b)
{
	return a.y < b.y;
}
Mat interestPoint(Mat input)
{
	const int featureNum = 100;
	Mat output;
	input.copyTo(output);
	vector<Mat> temp;
	cvtColor(input, input, CV_BGR2HSV);
	split(input, temp);
	vector<Point2f>  corners;
	goodFeaturesToTrack(temp[2], corners, featureNum, 0.01, 10);
	stable_sort(corners.begin(), corners.end(), return_small);
	if (corners.size() > 0)
	{
		circle(output, corners[0], 5, Scalar(0, 255, 0), -1);
	}
	vector<Mat> channel;
	for (int i = 0; i < 3; i++)
	{
		channel.push_back(temp[i]);
	}
	merge(channel, input);
	cvtColor(input, input, CV_HSV2BGR);
	return output;
}
