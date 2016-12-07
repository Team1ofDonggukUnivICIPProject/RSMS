#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>
#include<iostream>

using namespace cv;
using namespace std;

int main() {

	Mat currentFrame;
	Mat previousFrame;
	Mat result;
	
	VideoCapture camera{ 0 };

	while (true) {

		camera.read(currentFrame);

		cvtColor(currentFrame, currentFrame, CV_BGR2GRAY);

		imshow("Grayscale", currentFrame);

		//8 Level - Posterizing
		for (int i = 0; i < currentFrame.cols; ++i) {
			for (int j = 0; j < currentFrame.rows; ++j) {
				auto currentPixel = currentFrame.at<uchar>(j, i);
				if (0 <= currentPixel && currentPixel <= 32)
					currentFrame.at<uchar>(j, i) = 0;
				else if (32 < currentPixel && currentPixel <= 64)
					currentFrame.at<uchar>(j, i) = 32;
				else if (64 < currentPixel && currentPixel <= 96)
					currentFrame.at<uchar>(j, i) = 64;
				else if (96 < currentPixel && currentPixel <= 128)
					currentFrame.at<uchar>(j, i) = 96;
				else if (128 < currentPixel && currentPixel <= 160)
					currentFrame.at<uchar>(j, i) = 128;
				else if (160 < currentPixel && currentPixel <= 192)
					currentFrame.at<uchar>(j, i) = 160;
				else if (192 < currentPixel && currentPixel <= 224)
					currentFrame.at<uchar>(j, i) = 192;
				else
					currentFrame.at<uchar>(j, i) = 224;
			}
		}

		
		//Make difference
		result.release();
		result = currentFrame - previousFrame;
		
		auto count = 0;
		
		//Remove noises
		for (int i = 0; i < currentFrame.cols; ++i) {
			for (int j = 0; j < currentFrame.rows; ++j) {
				auto currentPixel = result.at<uchar>(j, i);
				if (currentPixel <= 96)
					result.at<uchar>(j, i) = 0;
				else
					++count;
			}
		}

		imshow("Input", currentFrame);
		imshow("Result", result);
	
		char ch = waitKey(1);
		if (ch == 27)
			break;
		
		previousFrame.release();
		previousFrame = currentFrame;
	}
}