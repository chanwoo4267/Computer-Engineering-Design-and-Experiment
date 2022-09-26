
#pragma once

#include "ofMain.h"
#include <cstring> //문자열 처리 관련 헤더파일
#include <cstdlib> //문자열 처리 관련 헤더파일
#include <queue> //QUEUE STL
#include <stack> //STACK STL 
#include <random> //무작위 뽑기 관련 헤더파일

#define EMPTY 1 //미로의 벽
#define WALL 0 //미로의 통로,방

class position {  //위치를 나타내는 변수
public:
	int xpos; //x좌표
	int ypos; //y좌표
	void input(int, int); //x,y를 한번에 넣는 기능
};

class maze { //미로를 저장하는 자료구조
public:
	bool state; //현재좌표의 미로가 벽인지 공간인지
	position parent; //최단경로를 찾을때 이용하는 변수, 자신의 부모 방 좌표
	int visit_time; //방문여부를 확인. 0이면 방문하지않음, 1 이상이면 방문함. int변수인 이유는 RFS시 방문횟수에 따라 색을 다르게 그리기때문
}; 

class ofApp : public ofBaseApp {

	public:

		void setup(); //프로그램 시작시 실행되는기본함수
		void update(); //지속적으로 실행되는기본함수
		void draw(); //지속적으로 실행되는 기본함수2, 주로 그리기 관련기능만 넣어줌

		void keyPressed(int key); //키 입력에 대한 기본함수
		void keyReleased(int key); //키 입력후 뗄때에 대한 기본함수

		bool readFile(); //미로 파일(.maz)를 입력받으면, 미로를 저장한다.
		bool readHW(); //미로의 높이와 폭을 입력받으면(txt파일), 미로를 생성해 저장한다.
		void freeMemory(); //동적할당된 메모리 해제
		bool DFS(); //DFS를 통해 미로 탈출
		bool BFS(); //BFS를 통해 미로 탈출
		bool RFS(); //RFS를 통해 미로 탈출
		void findbfs(); //DFS의 기능함수, 실질적 계산
		void finddfs(); //BFS의 기능함수, 실질적 계산
		void findrfs(); //RFS의 기능함수, 실질적 계산
		void drawmaze(); //미로를 그려주는 함수
		void pathdraw(); //DFS, BFS시 탐색 경로를 그려줌. 최단경로는 다른색으로 표기
		void rfsdraw(); //RFS시 탐색 경로를 그려줌. 탐색 횟수가 적은 경로일수록 탁한색, 많을수록 선명한 빨간색

		void initialize_maze(); //미로를 초기화하는 함수
		void MakeMaze(); //readHW로 입력받은 데이터로 미로를 생성하는 함수
		int calcdir(int* dir); //인접한 미로의 방중 뚫을수 있는곳 탐색
		int calcdirr(int* dir); //인접한 미로의 방중 뚫려있는 곳 탐색
		int calcpath(int* dir); //인접한 미로의 통로중 방문 가능한 곳 탐색
		int excludepath(int count, int* dir, position* temp); //RFS시 방문 가능한 곳중 최근에 들렀던 경로를 제외하는함수
		int rand(int a, int b); //미로 생성 알고리즘에서 랜덤한 위치를 선택하게 도와주는 함수 
		void visitrfs(position* t, int k); // RFS 탐색시 방문하며 위치를 변경하는 함수

		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		int Mov; //RFS시 총 이동횟수

		maze** Maze; //미로의 정보를 저장하는 변수
		
		int x;//미로칸의 열의 인덱스를 가리킨다.
		int y;//미로칸의 행의 인덱스를 가리킨다.

		position start; //시작점
		position end; //종점

		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다. 2이면 h,w만의 txt파일을 열었다.
		int start_flag; // 시작점을 움직이는지 확인하는 플래그
		int end_flag; // 종점을 움직이는지 확인하는 플래그
		int size=20; //미로의 크기

		// Used by example app
		ofTrueTypeFont myFont; //폰트 입력받는 변수
		float windowWidth, windowHeight; //윈도우 창 크기를 조절하는 변수

		bool isdfs; //dfs flag
		bool isbfs; //bfs flag
		bool isrfs; //custom rfs flag
};
