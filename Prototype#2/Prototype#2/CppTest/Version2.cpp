#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\video\video.hpp>
#include<vector>
#include<deque>
#include<iostream>
#include<windows.h>

using namespace cv;
using namespace std;

void posterize(Mat& inputArray);
bool canBeBackgroundModel(Mat& inputArray, int thresholds);
void fillMask(Mat& mask, Mat& inputArray);

void posterize(Mat& inputArray) {

	for (int i = 0; i < inputArray.rows; ++i)
		for (int j = 0; j < inputArray.cols; ++j) {
			int pixel = inputArray.at<uchar>(i, j);
			if (0 <= pixel && pixel < 32)
				inputArray.at<uchar>(i, j) = 0;
			else if (32 <= pixel && pixel < 64)
				inputArray.at<uchar>(i, j) = 32;
			else if (64 <= pixel && pixel < 96)
				inputArray.at<uchar>(i, j) = 64;
			else if (96 <= pixel && pixel < 128)
				inputArray.at<uchar>(i, j) = 96;
			else if (128 <= pixel && pixel < 160)
				inputArray.at<uchar>(i, j) = 128;
			else if (160 <= pixel && pixel < 192)
				inputArray.at<uchar>(i, j) = 160;
			else if (192 <= pixel && pixel < 224)
				inputArray.at<uchar>(i, j) = 192;
			else if (224 <= pixel && pixel < 256)
				inputArray.at<uchar>(i, j) = 224;
		}
}

bool canBeBackgroundModel(Mat& inputArray, int thresholds) {

	int count = 0;

	auto rows = inputArray.rows;
	auto cols = inputArray.cols;

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			if (inputArray.at<uchar>(i, j) == 255) {
				if ((i == 0 || i == rows - 1) ||
					(j == 0 || j == cols - 1))
					return false;

				++count;
			}

	return count < thresholds;
}

void fillMask(Mat& mask, Mat& inputArray) {

	mask.setTo(Scalar{ 1 });

	for (int i = 0; i < inputArray.rows; ++i)
		for (int j = 0; j < inputArray.cols; ++j)
			if (inputArray.at<uchar>(i, j) == 255)
				mask.at<uchar>(i, j) = 0;
}

int main() {

init:
	cout << "Object Counting Application [Demo version] - Made by Doryeong Park" << endl;

	VideoCapture camera;
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	while (true) {

		string option = "";

		cout << "1. Camera   2. Video file " << endl;
		cout << "Please input option: ";
		cin >> option;

		if (option == "1") {
			camera = VideoCapture{ 0 };
			break;
		}
		else if (option == "2") {
			string fileName = "";
			cout << "Please input file name: ";
			cin >> fileName;
			camera = VideoCapture{ fileName };
			break;
		}
		else {
			cout << "Please input right option" << endl;
		}
	}

	Mat currentFrame{ 240, 320, CV_8UC3 };
	Mat currentFrameGrayscaled{ 240, 320, CV_8UC1 };
	Mat accumulator = Mat::zeros(Size{ 320, 240 }, CV_32FC1);
	Mat mask{ 240, 320, CV_8U };
	mask = Scalar{ 1 };
	Mat background{ 240, 320, CV_8UC1 };
	Mat result;
	Mat noiseRemover = getStructuringElement(MORPH_CROSS, Size{ 3, 3 }, Point{ 1, 1 });
	Mat expander = getStructuringElement(MORPH_RECT, Size{ 5, 7 }, Point{ 2, 3 });
	deque<Point> pointQueue;

	vector<vector<Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	camera.read(currentFrame);

	cvtColor(currentFrame, currentFrameGrayscaled, COLOR_BGR2GRAY);
	
	accumulateWeighted(currentFrameGrayscaled, accumulator, 1, mask);
	convertScaleAbs(accumulator, background);

	while (true) {
		camera.read(currentFrame);

		if (currentFrame.empty())
			break;

		cvtColor(currentFrame, currentFrameGrayscaled, COLOR_BGR2GRAY);

		absdiff(currentFrameGrayscaled, background, result);
		threshold(result, result, 20, 255, THRESH_BINARY);

		accumulateWeighted(currentFrameGrayscaled, accumulator, 0.001, mask);
		convertScaleAbs(accumulator, background);

		morphologyEx(result, result, MORPH_OPEN, noiseRemover);
		morphologyEx(result, result, MORPH_CLOSE, expander);
		
		findContours(
			result, contours, hierarchy,
			CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point{ 0, 0 });

		vector<vector<Point>> contoursPolygon{ contours.size() };
		vector<Rect> rectBounded{ contours.size() };

		drawContours(
			result, contours, -1, cv::Scalar::all(255),
			CV_FILLED, 8, hierarchy, INT_MAX
		);

		for (int i = 0; i < contours.size(); ++i){
			approxPolyDP(Mat{ contours[i] }, contoursPolygon[i], 3, true);
			rectBounded[i] = boundingRect(Mat{ contoursPolygon[i] });
		}

		for (auto rectStandard = rectBounded.begin(); rectStandard != rectBounded.end(); ++rectStandard) {
			int width = rectStandard->width * 3;
			int height = rectStandard->height * 3;
			int x = rectStandard->x - rectStandard->width * 3 / 4;
			int y = rectStandard->y - rectStandard->height * 3 / 4;
			
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x + width >= 320) width = 320 - x;
			if (y + height >= 240) height = 240 - y;

			Rect rectExpanded(x, y, width, height);

			for(auto rectAnother = rectBounded.begin(); rectAnother != rectBounded.end(); ++rectAnother){
				if (*rectStandard == *rectAnother) 
					continue;
				
				if (rectExpanded.contains(Point{rectAnother->x + rectAnother->width / 2,
												rectAnother->y + rectAnother->height / 2})) {
					if (rectStandard->area() / 8 > rectAnother->area()) {
						rectBounded.erase(rectAnother);
						rectAnother = rectBounded.begin();
						rectStandard = rectBounded.begin();
					}
				}
				
			}
		}
		
		for (Rect& finalRect : rectBounded) {
			rectangle(currentFrame, finalRect, Scalar{ 0, 0, 255 }, 3);
			Point center = Point{ finalRect.tl().x + finalRect.width / 2,
			finalRect.tl().y + finalRect.height / 2 };
			circle(currentFrame,
			center, 1,
			Scalar{ 0, 255, 0 },
			2, CV_FILLED);

			pointQueue.emplace_back(center);
		}

		for (Point& p : pointQueue)
			circle(currentFrame, p, 1, Scalar{ 255, 0, 0 }, 1.5, CV_FILLED);

		if (pointQueue.size() > 30)
			pointQueue.pop_front();

		putText(currentFrame, "Count: " + to_string(rectBounded.size()),
			Point{ 15, 15 },
			3,
			0.5,
			Scalar{ 255, 0, 0 });

		imshow("Input", currentFrame);
		imshow("Background", background);
		imshow("Result", result);

		Sleep(35);
		char ch = waitKey(1);

		if (ch == 27)
			break;
	}

	destroyAllWindows();
	goto init;
}