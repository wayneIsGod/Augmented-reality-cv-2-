#include "hand.h"
#include "human.h"
#include "sword.h"
#include "people.h"
#include "interestPoint.h"
void skin_color(Mat& color, Mat_<uchar>& mask, int R_thr, int G_thr, int B_thr, 
	int Max_min_diff_thr, int R_G_diff_thr)
{
	mask = Mat_<uchar>::zeros(color.size());
	for (int y = 0; y < color.rows; y++)
	{
		Vec3b* b = (Vec3b*)color.ptr(y);//OpenCV裡可用Vec3b代替將3個uchar組成的容器(vector)，
										//且可在後面加上[]，註明是要操作此像素的哪個通道。
		Vec3b* eb = b + color.cols;
		uchar*p = mask.ptr(y);
		for (; b != eb; ++b, ++p)
		{
			Vec3b& px = *b;
			if (px[2] > R_thr&& px[0] > B_thr && px[1] > G_thr &&
				px[2] > px[1] && px[2] > px[0] && (px[2] - px[1]) > R_G_diff_thr &&
				(px[2] - (px[0] <= px[1] ? px[0] : px[1])) > Max_min_diff_thr)
			{
				*p = 255;
			}
		}
	}
} 

Mat Contours(Mat& input, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy)
{
	Mat output, hsvImg; 
	vector<Mat>temp;
	cvtColor(input, hsvImg, CV_BGR2HSV);
	split(hsvImg, temp);
	findContours(temp[2], contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	merge(temp, output);
	cvtColor(hsvImg, input, CV_HSV2BGR);
	cvtColor(output, output, CV_HSV2BGR);
	/*for (int i = 0; i < contours.size(); i++)
	{
		drawContours(output, contours, i, Scalar(255, 0, 0), 1, 8, hierarchy, 0, Point());
	}*/
	return output;
}

struct full
{
	int record;
	double Area;
};
bool return_big(struct full a, struct full b)
{
	return a.Area > b.Area;
}

Mat findHull(Mat& input, vector<vector<Point>>& contours,vector<vector<Point>>& hull,int& take)
{
	Mat output;
	struct full* full_cup = (struct full*)malloc(sizeof(struct full)*contours.size());
	input.copyTo(output);
	int count = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 20)
		{
			convexHull(Mat(contours[i]), hull[count], false);
			full_cup[count].record = i;
			count++;
		}
	}
	for (int i = 0; i < count; i++)
	{
		full_cup[i].Area=contourArea(contours[full_cup[i].record]);
	}
	for (int i = 0; i < count; i++)
	{
		full_cup[i].record = i;
	}
	stable_sort(full_cup, full_cup+count, return_big);
	if (full_cup[0].record > 0)
	{
		drawContours(output, hull, full_cup[0].record, Scalar(0, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
		take = full_cup[0].record;
	}
	else
	{
		take = -1;
	}
	free(full_cup);
	return output;
}
int hand()
{
	VideoCapture cap;
	VideoWriter writer;
	VideoWriter writer0;VideoWriter writer1;
	cap.open(0);
	if (!cap.isOpened())
	{
		cout << "沒攝像孔!\n" << endl;
		return -1;
	}
	namedWindow("皮膚辨識", 1);

	Mat face0 = imread("0.png", CV_LOAD_IMAGE_COLOR);
	Mat face1 = imread("1.png", CV_LOAD_IMAGE_COLOR);
	Mat face2 = imread("2.png", CV_LOAD_IMAGE_COLOR);
	Mat face3 = imread("3.png", CV_LOAD_IMAGE_COLOR);
	Mat face4 = imread("4.png", CV_LOAD_IMAGE_COLOR);
	Mat face5 = imread("5.png", CV_LOAD_IMAGE_COLOR);
	Mat* sword_face = new Mat[3];
	Mat* inv_sword_face = new Mat[3];
	sword_face[0] = imread("sword_0.png", CV_LOAD_IMAGE_COLOR);
	sword_face[1] = imread("sword_1.png", CV_LOAD_IMAGE_COLOR);
	sword_face[2] = imread("sword_2.png", CV_LOAD_IMAGE_COLOR);
	vector<Mat> face;
	face.push_back(face0);
	face.push_back(face1);
	face.push_back(face2);
	face.push_back(face3);
	face.push_back(face4);
	face.push_back(face5);
	const int face_size = 40;
	const int sword_size = 80;
	for (int i = 0; i < face.size(); i++)
	{
		resize(face[i], face[i], Size(face_size, face_size));
	}
	for (int i = 0; i < 3; i++)
	{
		resize(sword_face[i], sword_face[i], Size(sword_size, sword_size));
		flip(sword_face[i], inv_sword_face[i], -1);
	}
	
	int* R = (int*)malloc(sizeof(int));
	*R = 130;
	int* G = (int*)malloc(sizeof(int));
	*G = 130;
	int* B = (int*)malloc(sizeof(int));
	*B = 127;
	int* Max_min_diff_thr = (int*)malloc(sizeof(int));
	*Max_min_diff_thr = 0;
	int* R_G_diff_thr = (int*)malloc(sizeof(int));
	*R_G_diff_thr = 0;
	int* dilate_num = (int*)malloc(sizeof(int));
	*dilate_num = 4;
	int* erode_num = (int*)malloc(sizeof(int));
	*erode_num = 1;
	Mat_<uchar> mask;
	vector<Mat> bgr;
	Mat skin,skinEdge;

	int people_num = 0;
	int sword_num = 0;
	int power_num = 0;
	human* person = 0;
	human** people = new human*[100];
	sword* swordPtr = 0;
	sword** swords = new sword*[100];
	sword** inv_swords = new sword*[100];

	bool sword_do = 0;
	int point = 0;
	string talk[6] = { "I'm did!!!","help me!!!","don't kill me!!!","No No!!!","arrrr!!!","God! God! please!!!" };
	Size videoSize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH) *2, (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	writer.open("機器視覺HW2.avi", CV_FOURCC('D', 'I', 'V', 'X'), 7, videoSize);

	Mat show_img;
	Mat hull_img;
	Mat frame;
	Mat output;
	int hull_record = 0;

	while (1)
	{
			cap >> frame;
			if (frame.empty())
				break;
			createTrackbar("R門檻", "皮膚辨識", R, 255);
			createTrackbar("G門檻", "皮膚辨識", G, 255);
			createTrackbar("B門檻", "皮膚辨識", B, 255);
			createTrackbar("Max_min_diff", "皮膚辨識", Max_min_diff_thr, 255);
			createTrackbar("R_G_diff", "皮膚辨識", R_G_diff_thr, 255);
			createTrackbar("膨脹次數", "皮膚辨識", dilate_num, 20);
			createTrackbar("侵蝕次數", "皮膚辨識", erode_num, 20);
			skin_color(frame, mask, *R, *G, *B, *Max_min_diff_thr, *R_G_diff_thr);
			split(frame, bgr);
			//imshow("mask", mask);
			for (unsigned i = 0; i < bgr.size(); i++)
			{
				cvAnd(&((CvMat)bgr[i]), &((CvMat)mask), &((CvMat)bgr[i]));//bgr And mask
			}
			merge(bgr, skin);
			dilate(skin, skin, Mat(3, 3, CV_8UC1), Point(-1, -1), *dilate_num, BORDER_REFLECT, 0);
			//對skin進行膨脹
			erode(skin, skin, Mat(3, 3, CV_8UC1), Point(-1, -1), *erode_num, BORDER_REFLECT, 0);
			//對skin進行侵蝕

			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			skinEdge = Contours(skin, contours, hierarchy);
			//show_img = interestPoint(skinEdge);
			vector<vector<Point>> hull(contours.size());
			hull_img = findHull(skinEdge, contours, hull, hull_record);
			

			if (sword_do == 0)
			{
				for (int i = 0; i < 100; i++)
				{
					srand(time(NULL)%(i+1));
					inv_swords[i] = new sword((rand() % (frame.cols - 2 * sword_size)) + sword_size, sword_size, frame.cols, frame.rows, 0, 80, sword_size);
				}
				sword_do = 1;
			}
			swordGame(person, people, swordPtr, swords, inv_swords, people_num, sword_num, face, sword_face,
				inv_sword_face, hull_img, contours, hull, hull_record, point, power_num, talk, face_size, sword_size);
			swordGame(person, people, swordPtr, swords, inv_swords, people_num, sword_num, face, sword_face,
				inv_sword_face, frame, contours, hull, hull_record, point, power_num, talk, face_size, sword_size);
			
			output=cut(frame, hull_img, writer, writer0, writer1);
			imshow("機器視覺HW2", output);
			writer.write(output);
		if (waitKey(10) == 'z'|| waitKey(10) == 'Z')
			break;
	}
	delete[] people;
	delete[] swords;
	delete[] inv_swords;
	delete[] sword_face;
	delete[] inv_sword_face;
	return 0;
}