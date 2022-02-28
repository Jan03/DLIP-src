/*------------------------------------------------------------------------------------------
@ DLIP by Young-Keun Kim - Handong Global University

Author           : Jaeeun Park
Created          : 2017/09/01
Modified         : 2022/02/28
Language/ver     : C++ in MSVS2017

Description     : basic_webcam_program.cpp
------------------------------------------------------------------------------------------*/
#include <opencv.hpp>
#include <iostream>
#include <time.h>

#define WEBCAM		1	// 웹캠 활용시 1, 아니면 0
#define VIDEO		0	// 영상파일로 구동시 1, 아니면 0

using namespace cv;
using namespace std;

Mat cvtGray(Mat& color);

int main(int argc, char* argv[]) {

	//============================ User Setting ====================================┓
	unsigned int fps = 60;					// 초당 프레임 값 지정					│
	unsigned int window_resize_factor = 1;	// 0: 출력창 리사이즈 가능, 1 : 불가	│
	string output_data_format = ".jpg";		// 저장할 이미지 포맷 설정				│
	//==============================================================================┘

#if WEBCAM
	VideoCapture cap(0); // check your webcam device number
#elif VIDEO
	VideoCapture cap("testVideo.mp4");
#endif
	if (!cap.isOpened()) { // if not success, exit program
		cout << "Cannot open the video cam" << endl;
		system("pause");
		return -1;
	}

	int keyIn = -1;
	int flip_H = 1;
	int flip_V = 2;
	int flipIn = 4;
	bool bPause = false;
	bool bGrayScale = false;
	bool bSave = false;
	bool bReset = false;
	bool bExit = false;
	unsigned int delay_waitKey = 1000 / fps;

	time_t t;
	Mat src, dst;
	string output_data_name = "";

	namedWindow("MyVideo", window_resize_factor);

	while (1) {

		keyIn = waitKey(delay_waitKey);

		if (keyIn == 'h' || keyIn == 'H') flip_H *= -1;
		else if (keyIn == 'v' || keyIn == 'V') flip_V *= -1;
		else if (keyIn == 'g' || keyIn == 'G') bGrayScale = !bGrayScale;
		else if (keyIn == 'r' || keyIn == 'R') bReset = !bReset;
		else if (keyIn == 's' || keyIn == 'S') bSave = !bSave;
		else if (keyIn == 32) bPause = !bPause;  // space bar
		else if (keyIn == 27) bExit = !bExit;    // ESC button
		flipIn = flip_H + flip_V + 1;

		if (!bPause) {
			bool bSuccess = cap.read(src);
			if (!bSuccess) {
				cout << "Cannot read a frame from video stream" << endl;
				break;
			}
		}

		if (flipIn != 4)
			flip(src, dst, flipIn);
		else
			src.copyTo(dst);

		if (bGrayScale)
			dst = cvtGray(dst);

		if (bReset) {
			flip_V = 2;
			flip_H = 1;
			bGrayScale = false;
			bPause = false;
			bReset = false;
		}

		if (bSave) {
			output_data_name = to_string(time(&t)) + output_data_format;
			imwrite(output_data_name, dst);
			bSave = false;
		}

		if (bExit) {
			cout << "esc key is pressed by user" << endl;
			break;
		}

		imshow("MyVideo", dst);

	}
	return 0;
}


Mat cvtGray(Mat& color) {
	if (color.channels() == 1)
		return color;

	else {
		Mat gray;
		cvtColor(color, gray, CV_RGB2GRAY);
		return gray;
	}
}
