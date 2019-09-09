#include"function.h"
Mat backgroundClear(Mat image, Mat& background, int x, int y)
{
	Mat output;
	image.copyTo(output);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image.at<Vec3b>(i, j) == Vec3b(0,0,0))
			{
				output.at<Vec3b>(i, j) = background.at<Vec3b>(y + i, x + j);
			}
		}
	}
	return output;
}

void pictback(Mat& image, Mat& background, int x, int y)
{
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			background.at<Vec3b>(y + i, x + j) = image.at<Vec3b>(i, j);
		}
	}
}

Mat cut(Mat video0, Mat video1, VideoWriter& writer, VideoWriter& writer0,
	VideoWriter& writer1)
{
	Mat video(video0.rows, video0.cols + video1.cols, CV_8UC3);
	//Mat video(video0.rows, video0.cols, CV_8UC3);
	for (int i = 0; i < video0.rows; i++)
	{
		for (int j = 0; j < video0.cols; j++)
		{
			video.at<Vec3b>(i, j) = video0.at<Vec3b>(i, j);
		}
	}
	for (int i = 0; i < video1.rows; i++)
	{
		for (int j = 0; j < video1.cols; j++)
		{
			video.at<Vec3b>(i , (video0.cols + j)) = video1.at<Vec3b>(i, j);
		}
	}
	return video;
}

void bb_sort(double* input, int* record, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = i; j >= 0; j--)
		{
			if (input[j]< input[j+1])
			{
				double temp = input[j];
				input[j] = input[j + 1];
				input[j + 1] = temp;
				int tep = record[j];
				record[j] = record[j + 1];
				record[j + 1] = tep;
			}
		}
	}

}