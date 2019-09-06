#include "kobot.h"

//extern cv::Mat Frame ;
//extern bool busywait ;

int main(int argc, char *argv[])
{
	Uart_open() ; // 통신 준비
	Init_console() ; 

	usleep(1000000) ;
	Check_remote() ;


	
	std::cout << " start " << std::endl ;
//	testRed();
	//ddd();//ImageFromCamera 복사본, 쓰레드 쓰다가 문제(카메라 멈추는 거) 생기는 거 같아서 사용 중
	//	basketball();
	//int thr_id[2] ;
	//int a = 1 ;
	//int b = 2 ;
	//pthread_t p_threads[2] ;

	/*
	thr_id[0] = pthread_create(&p_threads[0], NULL, ImageFromCamera , (void *)&a ) ;
	if ( thr_id[0] < 0 ){
		perror("ImageFromCamer() create error : \n") ; 
		exit(0) ;
	}
	*/
//	Motion(212);
	usleep(1000000);
//	Motion(200);	
//	Motion(200);
//	usleep(1000000);


///////각 경기 실행 코드///////////////----------------------------------------------------------

	//basketball();
	//sprint();
	marathon();
	//obstacle();
//////////////////////////////////////





/*
	usleep(1000000) ;
	
	while(1){
		Motion(212);
		usleep(1000000) ;
		Motion(213);
		if(cv::waitKey(20)==27)
			break; // ESC키 누르 면 종료
	}
	*/

	/*
	thr_id[1] = pthread_create(&p_threads[1], NULL, ImageProcess , (void *)&b ) ;
	if ( thr_id[1] < 0 ){
		perror("ImageProcess() create error : \n") ; 
		exit(0) ;
	}
	*/
	int status ;
	//pthread_join(p_threads[0], (void **)&status) ;
	//pthread_join(p_threads[1], (void **)&status) ;

	Uart_close() ;
	return 0;
}

	/*		99 - 머리숙이기
		100 - 머리들기
		101 - 전진종종걸음
		102 - 후진종종걸음
		103 - 좌로걷기
		104 - 우로걷기
		105 - 좌회전
		106 - 우회전
		107 - 좌 90도
		108 - 우 90도
		109 - 180
		110 - 왼발 차기
		111 - 오른발 차기
		112 - 우유곽 잡기
		113 - 내려놓기		
		115 - 우유잡기 연속 동작 	
		116 - 4 run
		117 - 6 run
		118 - left 6
		119 - left 4 run 2
		120 - left 4 run 3
		121 - right 6 ...
...
		124 - left 10
		125 - right 10
		126 - near kick
	*/



