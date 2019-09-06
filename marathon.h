 #ifndef _VIDEO_PROCESS_
#define _VIDEO_PROCESS_

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <cmath>
#include <raspicam/raspicam_cv.h>


#define PI 3.1415926
#define MIN_RECGONIZE 16

///////////////////////////------------함수-------------/////////////////////////////////////
void marathon();
void find_direction(std::vector<cv::Point2f> approx);
void GetColor(bool is_color, cv::Mat image, int j, int i, int* B, int* G, int* R );
void SetColor(bool is_color, cv::Mat image, int j, int i, int B, int G = 0, int R = 0);
void LineTrace(cv::Mat& image);
void BinaryColors(cv::Mat &frame, int color[], int num);
bool COLOR(cv::Mat& image, cv::Vec3b color, int y, int x);
void draw_approx(cv::Mat& image, std::vector<cv::Point2f> approx);
void *updateFrame(void* arg);
int GetAngleABC(cv::Point a, cv::Point b, cv::Point c);
std::vector<cv::Point2f> DrawShapeDetection_Control(cv::Mat &m_image, cv::Mat &out_image);
//////////////////////////////////////////////////////////////////////////////////////////////


 
//MODE
 
#define L_GOOD_LINE 1
#define L_CUT_LINE 2
#define L_RIGHT_LINE 3
#define L_LEFT_LINE 4
#define L_WAIT 10
#define L_FIND 11
 
#define L_NO_LINE 20
#define L_NO_LINE_2 21
#define L_NO_LINE_3 22
#define L_NO_LINE_4 23
 
#define L_GO_LEFT 30
#define L_GO_RIGHT 31



#define D_FIND 100
#define D_FORWARD 101
#define D_LEFT 102
#define D_RIGHT 103
#define D_END 104
 
//?숈옉
#define M_base 99
#define M_walk_forward 141//140
#define M_walk_left 138
#define M_walk_right 139
#define M_walk_slow 105//144 ?곸?
#define M_walk_back 106
#define M_hoke_left 252
#define M_hoke_right 253
#define M_turn_left 136
#define M_turn_right 137
 
#define H_turn_forward 158
#define H_turn_left_45 151
#define H_turn_right_45 155
#define H_turn_left_60 152
#define H_turn_right_60 156
#define H_turn_left_90 153
#define H_turn_right_90 157
 
#define H_down 135
#define H_up_small 134
#define H_up_large 165 
#define H_up_90 161
 
#define SEC_01 100000
 cv::Vec3b V_RED = cv::Vec3b(0, 0, 255);
cv::Vec3b V_BLUE = cv::Vec3b(255, 0, 0);
cv::Vec3b V_GREEN = cv::Vec3b(0, 255, 0);
cv::Vec3b V_YELLOW = cv::Vec3b(0, 255, 255);
cv::Vec3b V_PURPLE = cv::Vec3b(255, 0, 255);
cv::Vec3b V_WHITE = cv::Vec3b(255, 255, 255);
cv::Vec3b V_BLACK = cv::Vec3b(0, 0, 0);
 

 
cv::Mat Frame, current_Frame;
cv::Mat Binaryframe, small_Binaryframe;
cv::Mat squareframe;
 
int NINE = 0;
// ?곸긽 16諛?異뺤냼
int divide = 4;
int ScreenX = 640 / divide;
int ScreenY = 480 / divide;
int right = 0, left = 0;
int redposition;
bool Right = true;
int nored = 0;
int start_first = 0;
 
int thresh = 200;
int max_thresh = 255;
 
int one, two, three;
 
int UP = 80, DOWN = 180;
 
int FIND_D_cnt=0;
 
int c_black[] = { 6 }, c_line[] = { 1 };
int c_b_line[] = {1,6};
int DIRECTION = 0;
 
int gotocenter_cnt =0;

pthread_mutex_t frameLocker; //裕ㅽ뀓???좉툑
pthread_t updateThread; //thread ID??updateThread
cv::VideoCapture capture;
int CNT_BLACK(cv::Mat& image);

#endif 
 