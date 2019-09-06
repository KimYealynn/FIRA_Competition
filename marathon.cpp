#include "marathon.h"
#include "RobotProtocol.h"
 
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <string>
 
 
 
///////////////////////////------------?®Ïàò-------------/////////////////////////////////////
//void find_direction(std::vector<cv::Point2f> approx);
//void GetColor(bool is_color, cv::Mat image, int j, int i, int* B, int* G , int* R );
//void SetColor(bool is_color, cv::Mat&  image, int j, int i, int B, int G = 0, int R = 0);
//void LineTrace(cv::Mat& image);
//void BinaryColors(cv::Mat &frame, int color[], int num);
//bool COLOR(cv::Mat& image, cv::Vec3b color, int y, int x);
//void draw_approx(cv::Mat& image, std::vector<cv::Point2f> approx);
//void *updateFrame(void* arg);
//int GetAngleABC(cv::Point a, cv::Point b, cv::Point c)
//std::vector<cv::Point2f> DrawShapeDetection_Control(cv::Mat &m_image, cv::Mat &out_image);
//////////////////////////////////////////////////////////////////////////////////////////////
 
 
int NEW = 0;
int MODE = L_NO_LINE;



void ImShow(std::string str, cv::Mat &image){
    imshow(str, image);
}


int C_LEFT=0, C_RIGHT=0;

 void check_black (){
     int cnt =0;
		cv::Mat image;

		//Motion2(H_turn_forward);
		//usleep(100000);

		pthread_mutex_lock(&frameLocker);
		current_Frame = Frame;
		pthread_mutex_unlock(&frameLocker);

     if (!current_Frame.empty())
		{
		cv::resize(current_Frame, image , cv::Size(320, 240), 0, 0, CV_INTER_NN);
			BinaryColors(image, c_b_line, 2);
			std::cout<< "#####"<<CNT_BLACK(small_Binaryframe)<<std::endl;
            ImShow("1000",small_Binaryframe);
                
			if(CNT_BLACK(small_Binaryframe)>4000){
                MODE = D_FIND;
                std::cout<< "black!!!!"<<CNT_BLACK(small_Binaryframe)<<std::endl;
            		}
         /*   cnt =CNT_COLOR(small_Binaryframe);
            std::cout<< "#####"<<cnt<< "L "<<C_LEFT<<"R"<<C_RIGHT<<std::endl;
            if(cnt<3000){
				ImShow("1000",small_Binaryframe);
                Motion2(199);Motion2(199);Motion2(199);
				if(C_LEFT >200 && C_LEFT>C_RIGHT+10) {Motion2(M_walk_left);}
                else if (C_RIGHT>200 && C_LEFT+10 < C_RIGHT) {Motion2(M_walk_right);}
            		}*/	
		}
}

//Motion(216);
//usleep(1800000);
 
void marathon()
{
	usleep(SEC_01*20);
	Motion2(198);

	int check=0;
 
	capture.open(0);
	cv::Mat image;
	std::vector<cv::Point2f> approx_temp;
	
	usleep(SEC_01 * 10);
	Motion2(H_down);
	Motion2(H_turn_forward);
 
	pthread_mutex_init(&frameLocker, NULL);
	pthread_create(&updateThread, NULL, updateFrame, NULL);
 
	if (!capture.isOpened()) {
		std::cerr << "Could not open camera" << std::endl;
		exit(1);
	}
 
	int NO_LINE_CNT = 0;
	while (true) {
		
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "<<MODE>> " << MODE << std::endl;
 
		pthread_mutex_lock(&frameLocker);
		current_Frame = Frame;
		pthread_mutex_unlock(&frameLocker);
 
		if (!current_Frame.empty())
		{
			
			cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
 			//check_black();
	
			if(MODE ==L_GO_LEFT){
				BinaryColors(current_Frame, c_line, 1);
			
				std::cout<< "****"<<CNT_COLOR(small_Binaryframe)<<std::endl;
				if(CNT_COLOR(small_Binaryframe)>500){ Motion2(M_walk_left); usleep(1000000);}
				else {Motion2(H_turn_forward); MODE = L_FIND;}
	

			}
			else if (MODE == L_GO_RIGHT){
				
				std::cout<< "****"<<CNT_COLOR(small_Binaryframe)<<std::endl;
				BinaryColors(current_Frame, c_line, 1);
				if(CNT_COLOR(small_Binaryframe)>500){ Motion2(M_walk_right);usleep(1000000);}
				else {Motion2(H_turn_forward); MODE = L_FIND;}
			}
			else if (MODE == L_CUT_LINE) {
 
				int cnt_Left = 0, cnt_Right = 0;
				usleep(SEC_01*15);
				Motion2(M_walk_back);
 				usleep(SEC_01*20);
 
				//LEFT-----------------------
				Motion2(H_turn_left_45);
				usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Left = CNT_COLOR(small_Binaryframe);
 
				}
				//RIGHT-------------------------
 
				Motion2(H_turn_right_45);
				usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);;
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Right = CNT_COLOR(small_Binaryframe);
				}
 				std::cout<< cnt_Left<< " "<<cnt_Right<<std::endl;		
				if (cnt_Left<1000 && cnt_Right < 1000) {
					Motion2(199);
					Motion2(M_walk_back);
					Motion2(M_walk_back);
					MODE = D_FIND;
				}
				else if (cnt_Left<=cnt_Right) {
					MODE = L_FIND;
					Motion2(H_turn_forward);
					Motion2(M_turn_right);
					Motion2(M_turn_right);
					std::cout << "right" << std::endl;
				}
				else if (cnt_Left>cnt_Right) {
					MODE = L_FIND;
					Motion2(H_turn_forward);
					Motion2(M_turn_left);
					Motion2(M_turn_left);
					std::cout << "left" << std::endl;
				}
				else{
					Motion2(199);	Motion2(199); Motion2(199); Motion2(199);
					std::cout<< cnt_Left<< "------- "<<cnt_Right<<std::endl;			
				}
			}
			else if (MODE == D_FIND) {				Motion2(H_turn_forward);
				Motion2(M_walk_back);
				usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						
						BinaryColors(image, c_black, 1);
 					FIND_D_cnt++;

					std::cout<<FIND_D_cnt<<">>>>>>>>>>>>>>>>>>>"<<std::endl;
					approx_temp = DrawShapeDetection_Control(image, squareframe);
 
					if (NINE) { find_direction(approx_temp); }
 
					if (MODE == D_LEFT) {
						DIRECTION = 1;
						MODE = D_END;
						Motion2(H_turn_left_90);
						Motion2(H_up_large);
						Motion2(M_walk_left);
						Motion2(M_walk_left);
						Motion2(M_walk_left);
					usleep(1000000);
					}
					else if (MODE == D_RIGHT) {
						std::cout<< "?îÏÇ¥???§Î•∏Ï™?<<std::endl;
						
						DIRECTION = 3;
						MODE = D_END;
					
						Motion2(H_turn_right_90);
						Motion2(M_walk_right);
						Motion2(M_walk_right);
						Motion2(M_walk_right);
						Motion2(H_up_large);
						
						usleep(1000000);
					}
					else if (MODE == D_FORWARD) {
						DIRECTION = 2;
						MODE = D_END;
						Motion2(H_up_large);
						Motion2(H_turn_forward);
						Motion2(H_turn_forward);
						usleep(1000000);

					}
					else if(FIND_D_cnt%2==0){
						Motion(M_walk_back);
					}
					else if(FIND_D_cnt%6==0){
						MODE = L_CUT_LINE;
					}
				}
 
 
				
			}
			else if (MODE == D_END){
				Motion(M_walk_forward);
				BinaryColors(image, c_line, 1);
				int cnt_new = CNT_COLOR(small_Binaryframe);
					
					std::cout<<"dend"<<cnt_new<<std::endl;
				if(cnt_new >1000){
					if (DIRECTION == 1) {//left
						Motion2(H_turn_forward);
						Motion2(H_down);
						Motion2(M_turn_left);
						Motion2(M_turn_left);
						Motion2(M_turn_left);
						Motion2(H_down);
						Motion(M_walk_forward);
						Motion(M_walk_forward);
						
						MODE = L_NO_LINE;
					}
					else if (DIRECTION == 2) {//forward
						Motion2(H_down);
						Motion(M_walk_forward);
						Motion(M_walk_forward);
						Motion(M_walk_forward);
						MODE = L_NO_LINE;
					}
					else if (DIRECTION == 3) {//right
						
						Motion2(H_turn_forward);
						Motion2(H_down);
						Motion2(M_turn_right);
						Motion2(M_turn_right);
						Motion2(M_turn_right);
						Motion2(H_down);
						Motion2(M_walk_forward);
						MODE = L_NO_LINE;
					}
				}
 

			}//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			else if (MODE == L_NO_LINE) {
				check =0;
				int cnt_Left=0;
				int cnt_Right=0;
				int cnt_Forward=0;
			//	std::cout<<"NOLINE :" << NO_LINE_CNT<<std::endl;
				
				if (NO_LINE_CNT - 2 == 0) { // ÏßÑÏßú ?∏Îùº??					NO_LINE_CNT = 0;
	
 					
					//LEFT-----------------------
					Motion2(H_turn_left_45);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);
 
					if (!current_Frame.empty())
					{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						BinaryColors(image, c_line, 1);
						cnt_Left = CNT_COLOR(small_Binaryframe);
					
					}

//FORWARD-----------------------
					Motion2(H_turn_forward);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);
 
					if (!current_Frame.empty())
					{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						BinaryColors(image, c_line, 1);
						cnt_Forward = CNT_COLOR(small_Binaryframe);
					
					}
 
//333
					//usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						
						BinaryColors(image, c_black, 1);
 					FIND_D_cnt++;

					std::cout<<FIND_D_cnt<<">>>>>>>>>>>>>>>>>>>"<<std::endl;
					approx_temp = DrawShapeDetection_Control(image, squareframe);
 
					if (NINE) { find_direction(approx_temp); }
 
					if (MODE == D_LEFT) {
						check=1;
						DIRECTION = 1;
						MODE = D_END;
						Motion2(H_turn_left_90);
						Motion2(H_up_large);
						Motion2(M_walk_left);
						Motion2(M_walk_left);
						usleep(1000000);
					
					}
					else if (MODE == D_RIGHT) {
						std::cout<< "?îÏÇ¥???§Î•∏Ï™?<<std::endl;
						check=1;
						DIRECTION = 3;
						MODE = D_END;
						Motion2(H_up_large);
						Motion2(H_turn_right_90);
						Motion2(M_walk_right);
						Motion2(M_walk_right);
						usleep(1000000);
						
					}
					else if (MODE == D_FORWARD) {
						check=1;
						DIRECTION = 2;
						MODE = D_END;
						Motion2(H_up_small);
						Motion2(H_turn_forward);
						usleep(1000000);
						
					}
				}



		//RIGHT-------------------------
				
					
					Motion2(H_turn_right_45);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);;
 
					if (!current_Frame.empty())
					{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						BinaryColors(image, c_line, 1);
						cnt_Right = CNT_COLOR(small_Binaryframe);
					}
				
 std::cout<<"L"<<cnt_Left <<" "<< cnt_Right<<std::endl; 
				if(check){}
				else if(cnt_Forward > 1000){
					MODE = L_FIND;
				}
				else if (cnt_Left<1000 && cnt_Right < 1000) {
						Motion2(199);
						//std::cout << "?òÏó•?"<< std::endl;
						Motion2(H_turn_forward);
						MODE = L_NO_LINE_2;
					}
					else if (cnt_Left<cnt_Right) {
						//MODE = L_FIND;
						//Motion2(H_turn_forward);
						Motion2(M_hoke_right);
						//Motion2(M_walk_right);
						Motion2(H_turn_right_45);
						usleep(1000000);
						MODE =L_GO_RIGHT;
					std::cout << "right" << std::endl;
					}
					else if (cnt_Left>cnt_Right){
						//MODE = L_FIND;
						//Motion2(H_turn_forward);
						Motion2(M_hoke_left);
						Motion2(H_turn_left_45);
						usleep(1000000);
						MODE =L_GO_LEFT;
				 	std::cout << "left" << std::endl;
					}
 
				}
				if(MODE == L_NO_LINE){
					NO_LINE_CNT++;
					Motion2(H_turn_forward);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);
 
					if (!current_Frame.empty())
					{
						BinaryColors(image, c_line, 1);
						cut_bin(small_Binaryframe);
						line_detect(small_Binaryframe);
						if (MODE != L_NO_LINE) { 
							NO_LINE_CNT = 0; 
							MODE = L_FIND;
						}
					}
				}
			}
			else if (MODE == L_NO_LINE_2) {
				check =0;
				int cnt_Left=0;
				int cnt_Right=0;
				int cnt_Forward=0;
				//std::cout<<"NOLINE2" <<std::endl;
			
				NO_LINE_CNT = 0;
 
				//LEFT-----------------------
				Motion2(H_turn_left_45);
				Motion2(M_walk_left);
				usleep(SEC_01*15);
 
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Left = CNT_COLOR(small_Binaryframe);
					
				}
				Motion2(M_walk_right);
 
				//FORWARD-----------------------
					Motion2(H_turn_forward);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);
 
					if (!current_Frame.empty())
					{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						BinaryColors(image, c_line, 1);
						cnt_Forward = CNT_COLOR(small_Binaryframe);
					
					}



//333
					//usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						
						BinaryColors(image, c_black, 1);
 					FIND_D_cnt++;

					std::cout<<FIND_D_cnt<<">>>>>>>>>>>>>>>>>>>"<<std::endl;
					approx_temp = DrawShapeDetection_Control(image, squareframe);
 
					if (NINE) { find_direction(approx_temp); }
 
					if (MODE == D_LEFT) {
						check=1;
						DIRECTION = 1;
						MODE = D_END;
						Motion2(H_turn_left_90);
						Motion2(H_up_large);
						Motion2(M_walk_left);
						Motion2(M_walk_left);
					usleep(1000000);
					}
					else if (MODE == D_RIGHT) {
						std::cout<< "?îÏÇ¥???§Î•∏Ï™?<<std::endl;
						check=1;
						DIRECTION = 3;
						MODE = D_END;
					
						Motion2(H_turn_right_90);
						Motion2(M_walk_right);
						Motion2(M_walk_right);
						Motion2(H_up_large);
						usleep(1000000);
					}
					else if (MODE == D_FORWARD) {
						check=1;
						DIRECTION = 2;
						MODE = D_END;
						Motion2(H_up_small);
							Motion2(H_turn_forward);
						usleep(1000000);
					}
				}


 
				//RIGHT-------------------------
				
				Motion2(H_turn_right_45);
				Motion2(M_walk_right);
				usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);;
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Right = CNT_COLOR(small_Binaryframe);
				}
				Motion2(M_walk_left);
 				//std::cout<<"line2"<<cnt_Left <<" "<< cnt_Right<<std::endl; 
				
				if(check){}
				else if(cnt_Forward > 1000){
					MODE = L_FIND;
				}
				else if (cnt_Left<1000 && cnt_Right < 1000) {
					Motion2(199);
					Motion2(199);
					//std::cout << "???????òÏó•?"<< std::endl;
					Motion2(H_turn_forward);
					MODE = L_NO_LINE_3;
				}
				else if (cnt_Left<cnt_Right) {
					//MODE = L_FIND;
					Motion2(H_turn_right_45);
						usleep(1000000);
					MODE =L_GO_RIGHT;
					//Motion2(H_turn_forward);
					Motion2(M_walk_right);
					Motion2(M_walk_right);
					usleep(SEC_01);
 
				//std::cout << "right2" << std::endl;
				}
				else if (cnt_Left>cnt_Right){
					//MODE = L_FIND;
					Motion2(H_turn_left_45);
						usleep(1000000);
					MODE =L_GO_LEFT;
					//Motion2(H_turn_forward);
					Motion2(M_walk_left);
					Motion2(M_walk_left);
					usleep(SEC_01);
					//std::cout << "left2" << std::endl;
				}			
			}
			else if (MODE == L_NO_LINE_3) {
				int cnt_Left = 0;
				check =0;
				int cnt_Right = 0, cnt_Forward=0;
				//std::cout << "NOLINE3" << std::endl;
 
				//LEFT-----------------------
				Motion2(H_turn_left_45);
				Motion2(M_walk_left);
				Motion2(M_walk_left);
				usleep(SEC_01*15);
 
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Left = CNT_COLOR(small_Binaryframe);
 
				}
				Motion2(M_walk_right);
				Motion2(M_walk_right);
				//FORWARD-----------------------
					Motion2(H_turn_forward);
					usleep(SEC_01*15);
					pthread_mutex_lock(&frameLocker);
					current_Frame = Frame;
					pthread_mutex_unlock(&frameLocker);
 
					if (!current_Frame.empty())
					{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						BinaryColors(image, c_line, 1);
						cnt_Forward = CNT_COLOR(small_Binaryframe);
					
					}
 

				
//333
					//usleep(SEC_01*15);
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);
 
				if (!current_Frame.empty())
				{
						cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
						
						BinaryColors(image, c_black, 1);
 					FIND_D_cnt++;

					std::cout<<FIND_D_cnt<<">>>>>>>>>>>>>>>>>>>"<<std::endl;
					approx_temp = DrawShapeDetection_Control(image, squareframe);
 
					if (NINE) { find_direction(approx_temp); }
 
					if (MODE == D_LEFT) {
						check=1;
						DIRECTION = 1;
						MODE = D_END;
						Motion2(H_turn_left_90);
						Motion2(H_up_large);
						Motion2(M_walk_left);
						Motion2(M_walk_left);
					usleep(1000000);
					}
					else if (MODE == D_RIGHT) {
						std::cout<< "?îÏÇ¥???§Î•∏Ï™?<<std::endl;
						check=1;
						DIRECTION = 3;
						MODE = D_END;
					
						Motion2(H_turn_right_90);
						Motion2(M_walk_right);
						Motion2(M_walk_right);
						Motion2(H_up_large);
						usleep(1000000);
					}
					else if (MODE == D_FORWARD) {
						check=1;
						DIRECTION = 2;
						MODE = D_END;
						Motion2(H_up_small);
						Motion2(H_turn_forward);
						usleep(1000000);
					}
				}




				//RIGHT-------------------------
 
				Motion2(H_turn_right_45);
				Motion2(M_walk_right);
				Motion2(M_walk_right);
				usleep(SEC_01*15);
 
				pthread_mutex_lock(&frameLocker);
				current_Frame = Frame;
				pthread_mutex_unlock(&frameLocker);;
 
				if (!current_Frame.empty())
				{
					cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
					BinaryColors(image, c_line, 1);
					cnt_Right = CNT_COLOR(small_Binaryframe);
				}
				Motion2(M_walk_left);
				Motion2(M_walk_left);
 					//std::cout<<"Line3"<<cnt_Left <<" "<< cnt_Right<<std::endl; 
				
				if(check){}
				else if(cnt_Forward > 1000){
					MODE = L_FIND;
				}
				else if (cnt_Left<1000 && cnt_Right < 1000) {
					Motion2(199);
					Motion2(199);
					//std::cout << "???????òÏó•??????" << std::endl;
					Motion2(H_turn_forward);
					MODE = L_NO_LINE_4;
				}
				else if (cnt_Left<cnt_Right) {
					//MODE = L_FIND;
					//Motion2(H_turn_forward);
					Motion2(M_walk_right);
					Motion2(M_walk_right);
					Motion2(M_walk_right);
					Motion2(H_turn_right_45);
						usleep(1000000);
					MODE =L_GO_RIGHT;
					//std::cout << "right3" << std::endl;
				}
				else if (cnt_Left>cnt_Right) {
					//MODE = L_FIND;
					//Motion2(H_turn_forward);
					Motion2(M_walk_left);
					Motion2(M_walk_left);
					Motion2(M_walk_left);
					Motion2(H_turn_left_45);
						usleep(1000000);
					MODE =L_GO_LEFT;
					//std::cout << "left3" << std::endl;
				}
			}
			else if (MODE == L_NO_LINE_4) {
                Motion2(M_walk_back);
				MODE = L_NO_LINE;
			}
			else {
				if (MODE == L_LEFT_LINE) { Motion2(M_hoke_left); }
				else if (MODE == L_RIGHT_LINE) { Motion2(M_hoke_right); }
				else if (MODE == L_WAIT) {Motion2(M_walk_slow);}
				else if (MODE == L_GOOD_LINE) { Motion(M_walk_forward);} //usleep(1000000);}
				//if(MODE != L_FIND){
				BinaryColors(image, c_line, 1);
				cut_bin(small_Binaryframe);
				line_detect(small_Binaryframe);//}
			}
			if(MODE >20 && MODE<25){
				Motion2(H_turn_forward);
				Motion2(M_walk_back);
				Motion2(M_walk_back);
			}			
 
			if (MODE == L_FIND) {
				std::cout<<"------goto------"<<std::endl;
				gotocenter_cnt++;
				if(gotocenter_cnt%3!=0){
				GoToCenter();}
				else {gotocenter_cnt=0;}
			}
			ImShow("koominju", image);
			ImShow("binary", small_Binaryframe);
 
		}
		/////////////////////////
		if (cv::waitKey(10) == 27) break;
	}
}
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void line_detect(cv::Mat &m_image)
{
	cv::Mat img_result, img_gray = m_image, a = m_image;
 
	int X = 0, Y = 0;
	one = 0; two = 0;  three = 0;
	//Í∑∏Î†à?¥Ïä§ÏºÄ???¥Î?ÏßÄÎ°?Î≥Ä??
	if (!m_image.empty()) {
		cvtColor(m_image, img_gray, CV_RGB2GRAY);
	}
 
	//?¥ÏßÑ???¥Î?ÏßÄÎ°?Î≥Ä??	
cv::Mat binary_image;
	if (!m_image.empty()) {
		threshold(img_gray, img_gray, 180, 200, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
	}
	//contourÎ•?Ï∞æÎäî??
	std::vector<std::vector<cv::Point> > contours;
	findContours(img_gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
 
	//contourÎ•?Í∑ºÏÇ¨?îÌïú??(?§Í≥Ω??
	std::vector<cv::Point2f> approx;
	std::vector<cv::Point2f> approx_1;
	std::vector<cv::Point2f> approx_2;
	std::vector<cv::Point2f> approx_3;
	//	std::vector<std::vector<cv::Point2f> > out_approxs;
 
	img_result = m_image.clone();
	cv::Mat rec = m_image.clone();
 
	for (size_t i = 0; i < contours.size(); i++)
	{
		X = 0; Y = 0;
		//Í∞Ä??Î∞îÍπ•???àÎäî ?∏Í≥Ω?†Îì§ Ï¢åÌëú
		cv::Point first(1, 1);
		cv::Point second(1, 238);
		cv::Point third(318, 238);
		cv::Point fourth(318, 1);
 
		if ((contours[i][0] != first) && (contours[i][1] != second) && (contours[i][2] != third) && (contours[i][3] != fourth)) //Í∞Ä??Î∞îÍπ•???àÎäî ?∏Í≥Ω???àÏû°Í≤?			cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
		else
			continue;
 
		if (fabs(cv::contourArea(cv::Mat(approx))) > 500)//Î©¥Ï†Å???ºÏ†ï?¨Í∏∞ ?¥ÏÉÅ?¥Ïñ¥???úÎã§.//isContourConvex(Mat(approx)) //convex?∏Ï? Í≤Ä?¨Ìïú??
		{
			draw_approx(img_result, approx, cv::Scalar(255, 255, 255));
		}
		//Î™®Îì† ÏΩîÎÑà??Í∞ÅÎèÑÎ•?Íµ¨Ìï¥???îÌïú??
		int ang_sum = 0;
		int size = approx.size();
 
		for (int k = 0; k < size; k++) {
			int ang = GetAngleABC(approx[k], approx[(k + 1) % size], approx[(k + 2) % size]);
 
			ang_sum += abs(ang);
		}
 
		int ang_threshold = 8;
		int ang_sum_min = (180 - ang_threshold) * (size - 2);
		int ang_sum_max = (180 + ang_threshold) * (size - 2);
		ang_sum = abs(ang_sum);
 
		//ang_sum >= ang_sum_min && ang_sum <= ang_sum_max
		if (1)
		{
			if (fabs(cv::contourArea(cv::Mat(approx))) > 500) {
				cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
				for (int z = 0; z < approx.size(); z++)
				{
					X += approx[z].x;
					Y += approx[z].y;
				}
				X /= approx.size();
				Y /= approx.size();
				cv::circle(rec, cv::Point2f(X, Y), 3, cv::Scalar(255, 255, 0));
				int x[size] = { 0, }, y[size] = { 0, }, cnt = 0;;
				for (int h = 0; h < size; h++) {
					x[h] = (X * 1 + approx[h].x * 1) / 2;
					y[h] = (Y * 1 + approx[h].y * 1) / 2;
					cv::circle(rec, cv::Point2f(x[h], y[h]), 3, cv::Scalar(255, 255, 255));
					if (COLOR(m_image, V_GREEN, y[h], x[h]) && Y > UP && Y < DOWN) { cnt++; }
					if (COLOR(m_image, V_BLUE, y[h], x[h]) && Y < UP || Y>DOWN) { cnt++; }
				}
				if ((float)cnt / (float)size > 0.5) {
					if (Y > UP && Y < DOWN) {
						cv::approxPolyDP(cv::Mat(contours[i]), approx_2, arcLength(cv::Mat(contours[i]), true)*0.02, true);
						draw_approx(rec, approx_2, cv::Scalar(255, 0, 0));
						two = 1;
					}
					else if (Y < UP) {
						cv::approxPolyDP(cv::Mat(contours[i]), approx_1, arcLength(cv::Mat(contours[i]), true)*0.02, true);
						draw_approx(rec, approx_1, cv::Scalar(0, 255, 0));
						one = 1;
					}
					else {
						cv::approxPolyDP(cv::Mat(contours[i]), approx_3, arcLength(cv::Mat(contours[i]), true)*0.02, true);
						draw_approx(rec, approx_3, cv::Scalar(0, 0, 255));
						three = 1;
					}
 
				}
			}
		}
	}
	ImShow("recc", rec);
 	NEW =0;
	if (two) {
		LineTrace(approx_2);
		NEW = 1;
	}
 	else if (MODE ==D_END && one) {
		FindCenter(approx_1);
	}
	if(MODE!=D_END){
		if (!one && two && three) { std::cout << "???äÍ∏∏?ºÌï¥ Ï≤úÏ≤ú??Í±∑Ïûê" << std::endl; MODE = L_WAIT; }
		else if (!one && !two && three) { 
            if (1) {
                //std::cout << "                 " << idx_cnt << std::endl;
                int centerX = 0, centerY = 0;
                for (int i = 0; i < approx_3.size(); i++) {
                    centerX += approx_3[i].x;
                    centerY += approx_3[i].y;
                }
                centerX /= approx_3.size();
                centerY /= approx_3.size();
                //std::cout<<"center"<<centerX<<" " << centerY<<std::endl;
                if (centerX < 120) {
                    std::cout << "?ºÏ™Ω?ºÎ°ú Í≤åÍ±∏??33" << std::endl;
                    //Motion(M_walk_left); return;
                    Motion2(M_walk_left); return;
                }
                if (centerX > 200) {
                    std::cout << "?§Î•∏Ï™ΩÏúºÎ°?Í≤åÍ±∏??33" << std::endl;
                    //Motion(M_walk_right); return;
                    Motion2(M_walk_right); return;
                    
                }
            }
            
            
          //  GoToCenter();
			//LineTrace(approx_3);
			std::cout << "?¥Ï†ú Í≤Ä?ïÎÑ§Î™®Ï∞æ?? << std::endl; MODE = D_FIND; }
		else if (!one && !two && !three) {
			std::cout << "Í≤Ä???§Î™® ?ÑÎãàÎ©???Ï∞æÏïÑ??Í≥†Í∞ú ?åÎ¶¨?? << std::endl; MODE = L_NO_LINE;
		}
	}
	//?àÎèÑ?∞Ïóê Ï∂úÎ†• 
	if (!current_Frame.empty())
	{
		ImShow("Shape ", img_result);
	}
 
}
 
 
int REC,CNTT=0;
std::vector<cv::Point2f> DrawShapeDetection_Control(cv::Mat &m_image, cv::Mat &out_image)
{
	cv::Mat img_result, img_gray = m_image, a = m_image;
 
	NINE = 0;
 
	int black_x = -1, black_y = -1;
 
	int X_7[50] = { 0, }, Y_7[50] = { 0, };
	int X_9[50] = { 0, }, Y_9[50] = { 0, };
	int IDX_9[50];
	int cnt_7 = 0, cnt_9 = 0;
 
	//Í∑∏Î†à?¥Ïä§ÏºÄ???¥Î?ÏßÄÎ°?Î≥Ä??
	if (!m_image.empty()) {
		cvtColor(m_image, img_gray, CV_RGB2GRAY);
	}
 
	//?¥ÏßÑ???¥Î?ÏßÄÎ°?Î≥Ä??	cv::Mat binary_image;
	if (!m_image.empty()) {
		threshold(img_gray, img_gray, 180, 200, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
	}
	//contourÎ•?Ï∞æÎäî??
	std::vector<std::vector<cv::Point> > contours;
	findContours(img_gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
 
	//contourÎ•?Í∑ºÏÇ¨?îÌïú??(?§Í≥Ω??
	std::vector<cv::Point2f> approx;
	std::vector<cv::Point2f> rectangle_approx;
	std::vector<cv::Point2f> nine_approx;
 
	img_result = m_image.clone();
 
 
	for (size_t i = 0; i < contours.size(); i++)
	{
		//Í∞Ä??Î∞îÍπ•???àÎäî ?∏Í≥Ω?†Îì§ Ï¢åÌëú
		cv::Point first(1, 1);
		cv::Point second(1, 238);
		cv::Point third(318, 238);
		cv::Point fourth(318, 1);
 
		if ((contours[i][0] != first) && (contours[i][1] != second) && (contours[i][2] != third) && (contours[i][3] != fourth)) //Í∞Ä??Î∞îÍπ•???àÎäî ?∏Í≥Ω???àÏû°Í≤?			cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
		else
			continue;
 
		if (fabs(cv::contourArea(cv::Mat(approx))) > 200)//Î©¥Ï†Å???ºÏ†ï?¨Í∏∞ ?¥ÏÉÅ?¥Ïñ¥???úÎã§.//isContourConvex(Mat(approx)) //convex?∏Ï? Í≤Ä?¨Ìïú??
		{
			draw_approx(img_result, approx, cv::Scalar(255, 255, 255));
		
		//Î™®Îì† ÏΩîÎÑà??Í∞ÅÎèÑÎ•?Íµ¨Ìï¥???îÌïú??
		int ang_sum = 0;
		int size = approx.size();
 
		for (int k = 0; k < size; k++) {
			int ang = GetAngleABC(approx[k], approx[(k + 1) % size], approx[(k + 2) % size]);
 
			ang_sum += abs(ang);
 
		}
 
		int ang_threshold = 8;
		int ang_sum_min = (180 - ang_threshold) * (size - 2);
		int ang_sum_max = (180 + ang_threshold) * (size - 2);
		ang_sum = abs(ang_sum);

		std::cout<< "angle" << ang_sum<<" area "<<fabs(cv::contourArea(cv::Mat(approx)))<<std::endl;
 			
 
		if (size == 4 && ang_sum >= ang_sum_min && ang_sum <= ang_sum_max)
		{
			if (fabs(cv::contourArea(cv::Mat(approx))) > 3000) {
				cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
				for (int z = 0; z < approx.size(); z++)
				{
					black_x += approx[z].x;
					black_y += approx[z].y;
				}
				black_x /= approx.size();
				black_y /= approx.size();
 
				int x[4] = { 0, }, y[4] = { 0, }, cnt = 0;;
				for (int h = 0; h < 4; h++) {
					x[h] = (black_x * 1 + approx[h].x * 5) / 6;
					y[h] = (black_y * 1 + approx[h].y * 5) / 6;
					if (COLOR(m_image, V_PURPLE, y[h], x[h])) { cnt++; }
				}
				if (cnt > 2) {
 
					REC++;
					cv::approxPolyDP(cv::Mat(contours[i]), rectangle_approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
 
					cv::Mat rec = m_image.clone();
					draw_approx(rec, rectangle_approx, cv::Scalar(255, 255, 255));
					cv::circle(rec, cv::Point2f(black_x, black_y), 3, cv::Scalar(255, 255, 255));
					ImShow("recc", rec);
                    std::cout<<"REC"<<std::endl;
				}
			}
		}
		else if (size == 7 && ang_sum >= 200 && ang_sum <= 1000)
		{
			if (fabs(cv::contourArea(cv::Mat(approx))) > 200) {
				cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
				for (int z = 0; z < approx.size(); z++)
				{
					X_7[cnt_7] += approx[z].x;
					Y_7[cnt_7] += approx[z].y;
				}
				X_7[cnt_7] /= approx.size();
				Y_7[cnt_7++] /= approx.size();
			}
 
		}
		else if (size == 9 && ang_sum >= 400 && ang_sum <= 1700)
		{
			if (fabs(cv::contourArea(cv::Mat(approx))) > 500 && fabs(cv::contourArea(cv::Mat(approx))) < 1400) {
 
				cv::approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);//?¨Í∞Å?ïÏùº?åÎßå Î¶¨ÌÑ¥??Î∞∞Ïó¥???Ä??				for (int z = 0; z < approx.size(); z++)
				{
					X_9[cnt_9] += approx[z].x;
					Y_9[cnt_9] += approx[z].y;
				}
				IDX_9[cnt_9] = i;
				X_9[cnt_9] /= approx.size();
				Y_9[cnt_9++] /= approx.size();
			}
			//else
			//std::cout << "else" << std::endl;
 
		}
		}
 
	}
 	std::cout<< REC <<"---------------------------------"<<std::endl;
	if (REC<5) {
		//CNTT++;
		int idx_7 = 100, idx_9 = 100;
		int isSeven = 0, isNine = 0;
 
		for (int k = 0; k < cnt_7; k++) {
			if (black_x + 5 > X_7[k] && black_x - 5 < X_7[k] && black_y + 10 > Y_7[k] && black_y - 10 < Y_7[k]) {
				idx_7 = k;
				isSeven = 1;
			}
		}
 
		cv::Mat b;
		for (int k = 0; k < cnt_9; k++) {
 
			if (black_x + 5 > X_9[k] && black_x - 5 < X_9[k] && black_y + 10 > Y_9[k] && black_y - 10 < Y_9[k]) {
				idx_9 = k;
				isNine = 1;
				cv::approxPolyDP(cv::Mat(contours[IDX_9[k]]), nine_approx, arcLength(cv::Mat(contours[IDX_9[k]]), true)*0.02, true);
				b = m_image.clone();
				draw_approx(b, nine_approx, cv::Scalar(255, 255, 255));
 
				ImShow("SELECTED", b);
			}
		}
 
		//
 
 	
		if (isSeven && isNine) {
			std::cout << "???" << std::endl;
			MODE = D_FIND;
		}
		else if (isSeven) {
			std::cout << "seven" << std::endl;
			MODE = D_FORWARD;
		}
		else if (isNine) {
			std::cout << "nine" << std::endl;
			NINE = 1;
		}
		else {
			std::cout << "?ú„Öú?ÜÏóâ" << std::endl;
			MODE = D_FIND;
		}
	}
	else{
		REC = 0;
		std::cout<< "?§Î™®?ÑÏóÜ??"<<std::endl;
		MODE = L_CUT_LINE;
	}
 
 
	//?àÎèÑ???ùÏÑ± 
	namedWindow("Shape Detection", cv::WINDOW_AUTOSIZE);
	//?àÎèÑ?∞Ïóê Ï∂úÎ†• 
	if (!current_Frame.empty())
	{
		ImShow("Shape Detection", img_result);
	}
	out_image = img_result;
	return nine_approx;
	//return 1;
}
 
 
void FindCenter(std::vector<cv::Point2f> approx) {
	int size = approx.size();
	int len[2] = { 0, };
	cv::Mat img = small_Binaryframe.clone();
 
	float slope[2] = { 0, };
	int idx[4] = { 0, }, idx_cnt = 0;
 
	int idx_up[2], idx_down[2], up_cnt = 0, down_cnt = 0;
 
	for (int i = 0; i < size; i++) {
		if (approx[i].y > UP - 5 && approx[i].y < UP + 5) { idx_up[up_cnt++] = i; idx[idx_cnt++] = i; }
		else if (approx[i].y > DOWN - 5 && approx[i].y < DOWN + 5) { idx_down[down_cnt++] = i; idx[idx_cnt++] = i; }
	}
 
	if (up_cnt == 1) { idx_up[1] = idx_up[0]; }
	if (down_cnt == 1) { idx_down[1] = idx_down[0]; }
 
	if (idx_cnt>4) {
		std::cout << "            idx     " << idx_cnt << std::endl;
		int centerX = 0, centerY = 0;
		for (int i = 0; i < 4; i++) {
			centerX += approx[idx[i]].x;
			centerY += approx[idx[i]].y;
		}
		centerX /= 4;
		centerY /= 4;
		//std::cout<<"center"<<centerX<<" " << centerY<<std::endl;	
		if (centerX < 100) {
			std::cout << "?ºÏ™Ω?ºÎ°ú Í≤åÍ±∏?? << std::endl;
			Motion(M_walk_left);
			
			return;
		}
		else if (centerX > 220) {
			std::cout << "?§Î•∏Ï™ΩÏúºÎ°?Í≤åÍ±∏?? << std::endl;
			Motion(M_walk_right); 
			
			return;
		}
		else {
			NEW = 1;
		}
 
	}
	if (1) {
 
		cv::circle(img, approx[idx[0]], 3, cv::Scalar(0, 0, 255));
		cv::circle(img, approx[idx[1]], 3, cv::Scalar(0, 255, 255));
		cv::circle(img, approx[idx[2]], 3, cv::Scalar(255, 0, 0));
		cv::circle(img, approx[idx[3]], 3, cv::Scalar(0, 255, 0));
		ImShow("long", img);
 
 
		for (int i = 0; i < 4; i++) {
			int j = i + 1;
			if (i == 3) j = 0;
			if ((approx[idx[i]].y < UP + 5 && approx[idx[i]].y > UP - 5) && (approx[idx[j]].y> DOWN - 5 && approx[idx[j]].y< DOWN + 5)) {
				len[0] = sqrt(pow(approx[idx[i]].x - approx[idx[j]].x, 2) + pow(approx[idx[i]].y - approx[idx[j]].y, 2));
				std::cout << i << ":" << 0 << "   " << len[0] << std::endl;
			}
			else if ((approx[idx[j]].y < UP + 5 && approx[idx[j]].y > UP - 5) && (approx[idx[i]].y> DOWN - 5 && approx[idx[i]].y< DOWN + 5)) {
				len[1] = sqrt(pow(approx[idx[i]].x - approx[idx[j]].x, 2) + pow(approx[idx[i]].y - approx[idx[j]].y, 2));
				std::cout << i << ":" << 1 << "   " << len[1] << std::endl;
			}
		}
 
 
		for (int i = 0; i < 4; i++) {
			int j = i + 1;
			if (i == 3) j = 0;
			if ((approx[idx[i]].y < UP + 5 && approx[idx[i]].y > UP - 5) && (approx[idx[j]].y> DOWN - 5 && approx[idx[j]].y< DOWN + 5)) {
				slope[0] = (float)(approx[idx[i]].y - approx[idx[j]].y) / (float)(approx[idx[i]].x - approx[idx[j]].x);
				std::cout << i << ":" << 0 << "   " << slope[0] << std::endl;
			}
			else if ((approx[idx[j]].y < UP + 5 && approx[idx[j]].y > UP - 5) && (approx[idx[i]].y> DOWN - 5 && approx[idx[i]].y< DOWN + 5)) {
				slope[1] = (float)(approx[idx[i]].y - approx[idx[j]].y) / (float)(approx[idx[i]].x - approx[idx[j]].x);
				std::cout << i << ":" << 1 << "   " << slope[1] << std::endl;
			}
		}
 		
		float slope_avg = (slope[0] + slope[1]) / 2;
 		/*if(idx_cnt<4){
			if(approx[idx_up[0]].x<150){
				slope_avg = (slope[0]>slope[1])
			}	
		}*/
 
		if (slope_avg <10 && slope_avg >0) { //?¨Í∏∞  ?∏ÌÅ¨??20??			std::cout << "?ºÏ™Ω" << std::endl;
			Motion2(M_turn_left);
		}
		else if (slope_avg > -10 && slope_avg<0) {
			std::cout << "?§Î•∏Ï™? << std::endl;
			Motion2(M_turn_right);
		}
		else {
			std::cout << "ÏßÅÏßÑ" << std::endl;
		}
	}
}
 
 
void LineTrace(std::vector<cv::Point2f> approx) {
	int size = approx.size();
	int len[2] = { 0, };
	cv::Mat img = small_Binaryframe.clone();
 
	float slope[2] = { 0, };
	int idx[10] = { 0, }, idx_cnt = 0;
 
	int idx_up[4], idx_down[4], up_cnt = 0, down_cnt = 0;
    
    int idx_left[4], idx_right[4] ,left_cnt =0, right_cnt =0;
 
	for (int i = 0; i < size; i++) {
		if (approx[i].y > UP - 10 && approx[i].y < UP + 10) { idx_up[up_cnt++] = i; idx[idx_cnt++] = i; }
		else if (approx[i].y > DOWN - 10 && approx[i].y < DOWN + 10) { idx_down[down_cnt++] = i; idx[idx_cnt++] = i; }
        else if (approx[i].y <= UP -10  && approx[i].y >= DOWN + 10) {
            if(approx[i].x<5){
                    idx_left[left_cnt++] = i; idx[idx_cnt++] = i; }
            else if(approx[i].x >315){
                idx_right[right_cnt++] =i; idx[idx_cnt++]=i;
            }
        }
    }
	if (up_cnt == 1) { idx_up[1] = idx_up[0]; }
	if (down_cnt == 1) { idx_down[1] = idx_down[0]; }
 
	if (1) {
		//std::cout << "                 " << idx_cnt << std::endl;
		int centerX = 0, centerY = 0;
		for (int i = 0; i < 4; i++) {
			centerX += approx[idx[i]].x;
			centerY += approx[idx[i]].y;
		}
		centerX /= 4;
		centerY /= 4;
		//std::cout<<"center"<<centerX<<" " << centerY<<std::endl;	
		if (centerX < 120) {
			std::cout << "?ºÏ™Ω?ºÎ°ú Í≤åÍ±∏?? << std::endl;
			//Motion(M_walk_left); return;
			Motion2(M_hoke_left); return;
		}
		if (centerX > 200) {
			std::cout << "?§Î•∏Ï™ΩÏúºÎ°?Í≤åÍ±∏?? << std::endl;
			//Motion(M_walk_right); return;
			Motion2(M_hoke_right); return;
 
		}
 
	}
	if (1) {
 
		cv::circle(img, approx[idx[0]], 3, cv::Scalar(0, 0, 255));
		cv::circle(img, approx[idx[1]], 3, cv::Scalar(0, 255, 255));
		cv::circle(img, approx[idx[2]], 3, cv::Scalar(255, 0, 0));
		cv::circle(img, approx[idx[3]], 3, cv::Scalar(0, 255, 0));
		ImShow("long", img);
 
 
		for (int i = 0; i < 4; i++) {
			int j = i + 1;
			if (i == 3) j = 0; //fix
			if ((approx[idx[i]].y < UP + 10 && approx[idx[i]].y > UP - 10) && (approx[idx[j]].y> DOWN - 10 && approx[idx[j]].y< DOWN + 10)) {
				len[0] = sqrt(pow(approx[idx[i]].x - approx[idx[j]].x, 2) + pow(approx[idx[i]].y - approx[idx[j]].y, 2));
				std::cout << i << ":" << 0 << "   " << len[0] << std::endl;
			}
			else if ((approx[idx[j]].y < UP + 10 && approx[idx[j]].y > UP - 10) && (approx[idx[i]].y> DOWN - 10 && approx[idx[i]].y< DOWN + 10)) {
				len[1] = sqrt(pow(approx[idx[i]].x - approx[idx[j]].x, 2) + pow(approx[idx[i]].y - approx[idx[j]].y, 2));
				std::cout << i << ":" << 1 << "   " << len[1] << std::endl;
			}
		}
 
 
		for (int i = 0; i < 4; i++) {
			int j = i + 1;
			if (i == 3) j = 0;
			if ((approx[idx[i]].y < UP + 5 && approx[idx[i]].y > UP - 5) && (approx[idx[j]].y> DOWN - 5 && approx[idx[j]].y< DOWN + 5)) {
				slope[0] = (float)(approx[idx[i]].y - approx[idx[j]].y) / (float)(approx[idx[i]].x - approx[idx[j]].x);
				std::cout << i << ":" << 0 << "   " << slope[0] << std::endl;
			}
			else if ((approx[idx[j]].y < UP + 5 && approx[idx[j]].y > UP - 5) && (approx[idx[i]].y> DOWN - 5 && approx[idx[i]].y< DOWN + 5)) {
				slope[1] = (float)(approx[idx[i]].y - approx[idx[j]].y) / (float)(approx[idx[i]].x - approx[idx[j]].x);
				std::cout << i << ":" << 1 << "   " << slope[1] << std::endl;
			}
		}
 
		float slope_avg = (slope[0] + slope[1]) / 2;
 
 
		if (slope_avg <4 && slope_avg >0) {
			std::cout << "?ºÏ™Ω" << std::endl;
			MODE = L_LEFT_LINE;
 
		}
		else if (slope_avg > -4 && slope_avg<0) {
			std::cout << "?§Î•∏Ï™? << std::endl;
			MODE = L_RIGHT_LINE;
		}
		else {
			std::cout << "ÏßÅÏßÑ" << std::endl;
			MODE = L_GOOD_LINE;
		}
	}
}
 
void find_direction(std::vector<cv::Point2f> approx) {
	int len[9] = { 0, };
	int idx[9] = { 0,1,2,3,4,5,6,7,8 };
 
	/*std::vector<cv::Scalar> col;
	col.push_back(cv::Scalar(0, 0, 255));
	col.push_back(cv::Scalar(0, 100, 255));
	col.push_back(cv::Scalar(0, 255, 255));
	col.push_back(cv::Scalar(0, 255, 0));
	col.push_back(cv::Scalar(255, 0, 0));
	col.push_back(cv::Scalar(155, 0, 0));
	col.push_back(cv::Scalar(255, 0, 255));
	col.push_back(cv::Scalar(200, 200, 200));
	col.push_back(cv::Scalar(100, 100, 100));
	cv::Mat image = small_Binaryframe.clone();*/
	for (int i = 0; i < 9; i++) {
		int j = i + 1;
		if (i == 8) j = 0;
		len[i] = sqrt(pow(approx[i].x - approx[j].x, 2) + pow(approx[i].y - approx[j].y, 2));
 
		//cv::circle(image, approx[i], 3, col[i]);
 
		//std::cout << "len  - " << i << " : " << len[i] << std::endl;
	}
	//	imshow("slope", image);
 
	cv::Mat img = small_Binaryframe.clone();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			if (len[j] < len[j + 1]) {
				int temp = len[j];
				len[j] = len[j + 1];
				len[j + 1] = temp;
				temp = idx[j];
				idx[j] = idx[j + 1];
				idx[j + 1] = temp;
			}
		}
	}
 
	cv::circle(img, approx[idx[0]], 3, cv::Scalar(0, 0, 255));
	cv::circle(img, approx[idx[1]], 3, cv::Scalar(255, 0, 0));
	ImShow("same", img);
 
	int IDX = idx[1];
	if (idx[0] > idx[1] && idx[1] != 8) IDX = idx[0];
 
	int FIND_IDX = (IDX + 3) % 9;
	if (approx[IDX].x > approx[FIND_IDX].x) {
		std::cout << "?ºÏ™Ω!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		MODE = D_LEFT;
	}
	else {
		std::cout << "?§Î•∏Ï™?!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		MODE = D_RIGHT;
	}
}
 
void GoToCenter( ){
	Motion2(H_turn_forward);
	Motion2(M_base);
	usleep(1000000);
	
	cv::Mat image;
	
	for(int i =0 ; i<4; i++){
		int cnt_L =-1, cnt_C=-1, cnt_R=-1;
 	
		pthread_mutex_lock(&frameLocker);
		current_Frame = Frame;
		pthread_mutex_unlock(&frameLocker);
 
		if (!current_Frame.empty())
		{
			usleep(1500000);
			cv::resize(current_Frame, image, cv::Size(320, 240), 0, 0, CV_INTER_NN);
 			BinaryColors(image, c_line, 1);
			ImShow("goto", small_Binaryframe);
			for (int y = 2; y < small_Binaryframe.rows; y++) {	
				for (int x = 2; x < small_Binaryframe.cols-1; x++) {
					if (COLOR(small_Binaryframe, V_GREEN, y, x)|| COLOR(small_Binaryframe, V_BLUE, y, x)) { // green
						if(x<130) cnt_L++;
						else if(x>190) cnt_R++;
						else cnt_C++;
					}	
				}
			}
			std::cout<< cnt_L<<" " << cnt_C << " " << cnt_R<<std::endl;
			if (cnt_L > 100 || cnt_C > 100 || cnt_R > 100){
				if(cnt_R>=cnt_L+50 && cnt_R>=cnt_C+50) {Motion2(M_walk_right);usleep(100000);}
				else if(cnt_L>=cnt_R + 50 && cnt_L>=cnt_C+50) {Motion2(M_walk_left);usleep(100000);}
				else { MODE = L_WAIT; std::cout<<"end"<<std::endl; gotocenter_cnt =0; return; }
			}
			else {
				MODE = L_NO_LINE; Motion2(199); return;
			}
		}
	}
} 
 
 
int CNT_COLOR(cv::Mat& image) {
	int cnt =0;
    C_LEFT=0; C_RIGHT=0;
 
	for (int y = 2; y < image.rows-1; y++) {
		for (int x = 2; x < image.cols-1; x++) {
			if (COLOR(image, V_GREEN, y, x)|| COLOR(image, V_BLUE, y, x)) { // green
				cnt++;
                if(x<100) C_LEFT++;
                else if (x>220) C_RIGHT++;
			}
		}
	}
	return cnt;
}


 
int CNT_BLACK(cv::Mat& image) {
	int cnt =0;
 
	for (int y = 2; y < image.rows-1; y++) {
		for (int x = 2; x < image.cols-1; x++) {
			if (COLOR(image, V_PURPLE, y, x)) { 
				cnt++;
			}
		}
	}
	return cnt;
}




 
 
void cut_bin(cv::Mat& image) {
	int up = UP, down = DOWN;
 
	for (int y = 0; y < image.rows; y++) {
		if (y == up) y = down;
 
		for (int x = 0; x < image.cols; x++) {
			if ((y > up - 3 && y < up + 3) || (y > down - 3 && y < down + 3)) {
				SetColor(1, image, y, x, 0, 0, 0);
			}
			else if (COLOR(image, V_GREEN, y, x)) { // green
				SetColor(1, image, y, x, 255, 0, 0);
			}
		}
	}
}
 


int GG = 0;
void GetColor(bool is_color, cv::Mat image, int j, int i, int* B, int* G, int* R)
{
	uchar *data = image.data;
	uchar *blue, *green, *red;
 
	int nRow = image.rows;
	int nCol = image.cols;
 
	if (i > nCol - 1 || j > nRow - 1) {
		GG++;
		if (GG % 100 == 0) {
			std::cout << nCol << " " << nRow << "/";
		}
		return;
	}
 
	blue = image.data + j*image.step + i*image.elemSize() + 0;
	if (is_color) {
		green = image.data + j*image.step + i*image.elemSize() + 1;
		red = image.data + j*image.step + i*image.elemSize() + 2;
	}
	*B = (int)*blue;
	if (is_color) { *G = (int)*green; *R = (int)*red; }
}
 
 
void SetColor(bool is_color, cv::Mat image, int j, int i, int B, int G, int R)
{
	uchar *data = image.data;
	uchar *blue, *green, *red;
 
	int nRow = image.rows;
	int nCol = image.cols;
 
	if (i > nCol - 1 || j > nRow - 1) {
		std::cout << "S";
		return;
	}
 
	blue = image.data + j*image.step + i*image.elemSize() + 0;
	if (is_color) {
		green = image.data + j*image.step + i*image.elemSize() + 1;
		red = image.data + j*image.step + i*image.elemSize() + 2;
	}
	*blue = B;
	if (is_color) { *green = G;	*red = R; }
 
}
 
 
bool COLOR(cv::Mat& image, cv::Vec3b color, int y, int x) {
	int R, G, B;
 
	GetColor(1, image, y, x, &B, &G, &R);
	//std::cout<<B<<" "<<G<<" "<<R<<std::endl;
 
	return (B == color[0] && G == color[1] && R == color[2]);
}
 
 
void BinaryColors(cv::Mat &frame, int color[], int num) {
	int cnt_BLACK=0;
	// hsvÎ°?Î∞îÍæ∏Í≥?	cv::Mat Temp, temp, temp2, F_red, F_black, F_blue, F_green, F_yellow, F_white, sum, x;
	cv::Mat a, b;
 
	cv::Mat small(2, 2, CV_8U, cv::Scalar(1));
	cv::Mat element(4, 4, CV_8U, cv::Scalar(1));
 
	cvtColor(frame, Temp, CV_BGR2HSV);
	cvtColor(frame, x, CV_BGR2HSV);
 
	for (int i = 0; i<num; i++) {
		switch (color[i]) {
		case 1: // RED 
 
				//std::cout<<"1"<<std::endl; 
		inRange(Temp, cv::Scalar(0, 80, 120), cv::Scalar(15, 255, 255), temp);
		inRange(Temp, cv::Scalar(170, 255, 255), cv::Scalar(180, 255, 255), temp2);
 
				
		//inRange(Temp, cv::Scalar(0, 50, 80), cv::Scalar(20, 255, 255), temp);
			//inRange(Temp, cv::Scalar(170, 255, 255), cv::Scalar(180, 255, 255), temp2);
 
			a = x; b = x;
 
			a.setTo(0); b.setTo(0);
 
			a.setTo(V_GREEN, temp);
			b.setTo(V_GREEN, temp2);
 
			F_red = a | b;
 
			cv::erode(F_red, F_red, small);
			cv::dilate(F_red, F_red, cv::Mat());
 
			sum = F_red;
 
			break;
 
		case 2: // BLUE // hsvÎ°?Î∞îÍæ∏Í≥?
				//std::cout<<"2"<<std::endl; 
			cvtColor(frame, Temp, CV_BGR2HSV);
			inRange(Temp, cv::Scalar(80, 65, 65), cv::Scalar(140, 255, 255), temp);
 
			a = x;
			a.setTo(0);
			a.setTo(V_BLUE, temp);
			F_blue = a;
			cv::erode(F_blue, F_blue, small);
			cv::dilate(F_blue, F_blue, cv::Mat());
 
			sum = F_blue;
			break;
 
 
		case 3: //GREEN
			cvtColor(frame, Temp, CV_BGR2HSV);
			inRange(Temp, cv::Scalar(70, 140, 20), cv::Scalar(95, 255, 200), temp);
			//inRange(Temp, cv::Scalar(150, 140, 0), cv::Scalar(190, 180, 20), temp);
			a = x;
			a.setTo(0);
			a.setTo(V_GREEN, temp);
			F_green = a;
			cv::erode(F_green, F_green, small);
			cv::dilate(F_green, F_green, cv::Mat());
 
			sum = F_green;
			break;
 
 
 
			/*	// YCBCR Î∞îÍæ∏Í≥?			//std::cout<<"3"<<std::endl;
			cvtColor(frame, F_green, CV_BGR2YCrCb);
 
			// Ï¥àÎ°ù?âÏúºÎ°??¥ÏßÑ??			inRange(F_green, cv::Scalar(1, 0, 0), cv::Scalar(160, 119, 119), temp);
			inRange(F_green, cv::Scalar(50, 0, 0), cv::Scalar(250, 115, 115), temp2);
 
			temp = temp | temp2;
 
			a = x;
			a.setTo(0);
			a.setTo(V_GREEN, temp);
 
			F_green = a;
 
			cv::erode(F_green, F_green, small); // default 3x3
			cv::dilate(F_green, F_green, element); // element
			cv::dilate(F_green, F_green, element);
 
			sum = F_green;
			break;
			*/
		case 4:
			//std::cout<<"4"<<std::endl;
 
			inRange(Temp, cv::Scalar(25, 110, 110), cv::Scalar(32, 255, 255), temp);
			a = x;
			a.setTo(0);
			a.setTo(V_YELLOW, temp);
 
			F_yellow = a;
 
			cv::erode(F_yellow, F_yellow, small);
			cv::dilate(F_yellow, F_yellow, cv::Mat());
 
			sum = F_yellow;
			break;
				case 5:
			inRange(frame, cv::Scalar(200, 200, 200), cv::Scalar(255, 255, 255), temp);
			a = x;
			a.setTo(0);
			a.setTo(V_WHITE, temp);
 
			F_white = a;
 
			cv::erode(F_white, F_white, small);
			cv::dilate(F_white, F_white, cv::Mat());
 
			sum = F_white;
			break;
		case 6:
			inRange(frame, cv::Scalar(10, 10, 10), cv::Scalar(120, 120, 120), temp);
			a = x;
			a.setTo(0);
			a.setTo(V_PURPLE, temp);
 
			F_black = a;
 
			cv::erode(F_black, F_black, small);
			cv::dilate(F_black, F_black, cv::Mat());
 
			sum = F_black;
			break;
		}//switch	
 
		if (i == 0) Binaryframe = sum;
		else Binaryframe = sum | Binaryframe;
 
	}//for
	for (int y = 0; y < Binaryframe.rows; y++) {
		for (int x = 0; x < Binaryframe.cols; x++) {
			if (y == 0 || y == Binaryframe.rows - 1 || x == 0 || x == Binaryframe.cols - 1
				|| y == 1 || y == Binaryframe.rows - 2 || x == 1 || x == Binaryframe.cols - 2) {
				SetColor(1, Binaryframe, y, x, 0, 0, 0);
			}
		}
	}
 
	cv::resize(Binaryframe, small_Binaryframe, cv::Size(320, 240), 0, 0, CV_INTER_NN);
 
 
}
 
 
int GetAngleABC(cv::Point a, cv::Point b, cv::Point c)
{
	cv::Point ab(b.x - a.x, b.y - a.y);
	cv::Point cb(b.x - c.x, b.y - c.y);
 
	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product
 
	float alpha = atan2(cross, dot);
 
	return (int)floor(alpha * 180.0 / CV_PI + 0.5);
}
 
 
void draw_approx(cv::Mat& image, std::vector<cv::Point2f> approx, cv::Scalar color) {
 
	int size = approx.size();
 
	//ContourÎ•?Í∑ºÏÇ¨?îÌïú ÏßÅÏÑ†??Í∑∏Î¶∞??
	if (size % 2 == 0) {
		cv::line(image, approx[0], approx[approx.size() - 1], color, 3);
 
		for (int k = 0; k < size - 1; k++)
			cv::line(image, approx[k], approx[k + 1], color, 3);
 
		for (int k = 0; k < size; k++)
			cv::circle(image, approx[k], 3, cv::Scalar(0, 0, 0));
	}
	else {
		cv::line(image, approx[0], approx[approx.size() - 1], color, 3);
 
		for (int k = 0; k < size - 1; k++)
			cv::line(image, approx[k], approx[k + 1], color, 3);
 
		for (int k = 0; k < size; k++)
			cv::circle(image, approx[k], 3, cv::Scalar(0, 0, 0));
	}
}
 
 
void *updateFrame(void* arg)
{
	while (1)
	{
		cv::Mat tempframe;
		capture >> tempframe;
		pthread_mutex_lock(&frameLocker);
		Frame = tempframe;
		//balance_white(Frame);
		pthread_mutex_unlock(&frameLocker);
	}
	pthread_exit((void *)0);
}
