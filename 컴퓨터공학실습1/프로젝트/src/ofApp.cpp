
#include "ofApp.h"
#include <iostream>
using namespace std;


void ofApp::setup() {

	ofSetWindowTitle("Maze Project"); //창 가장위에 Maze Project라는 글자 띄우기
	ofSetFrameRate(15); //주사율 설정
	ofBackground(255, 255, 255); //배경색 설정
	
	windowWidth = ofGetWidth(); //화면 크기에 맞춰 변수설정
	windowHeight = ofGetHeight(); //화면 크기에 맞춰 변수설정

	//각종 flag 및 변수들 초기화
	HEIGHT = 0;
	WIDTH = 0;
	Mov = 0;

	isdfs = false;
	isbfs = false;
	isrfs = false;
	start_flag = 0;
	end_flag = 0;
	isOpen = 0; 

	//설정한 변수를 바탕으로 창이 띄워지는 위치를 화면 중앙으로 조절
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	//verdana.ttf라는 폰트를 입력받음
	myFont.loadFont("verdana.ttf", 12, true, true);

}


void ofApp::update() {

}


void ofApp::draw() {
	ofSetColor(100); //선분의 색 조정
	ofSetLineWidth(5); //굵이 5의 선분 그리기
	
	//input[][] : 1은 통로, 0은 벽 따라서 벽에 작은 사각형을 그리는방식으로 그려주기
	if (isOpen) {

		drawmaze();
	}

	if (start_flag) { //시작점을 옮기는가?
		//시작점으로 그려주는 위치를 옮긴다
		int window_h = start.ypos * size;
		int window_w = start.xpos * size;
		//검은색
		ofSetColor(0);
		//해당 위치에 크기에 맞는 사각형 그리기
		ofDrawRectangle(window_w, window_h, size, size);
	}

	if (end_flag) { //종점을 옮기는가?
		//종점으로 그려주는 위치를 옮긴다.
		int window_h = end.ypos * size;
		int window_w = end.xpos * size;
		//검은색
		ofSetColor(0);
		//해당 위치에 크기에 맞는 사각형 그리기
		ofDrawRectangle(window_w, window_h, size, size);
	}

	if (isdfs) //dfs탐색을 했다면
	{
		ofSetColor(0,255,0); //색 조정
		if (isOpen) { //파일이 열려있다면
			pathdraw(); //탐색 경로를 그려준다.
		}
			
		else //파일을 열지않았다면
			cout << "You must open file first" << endl; 
	}

	if (isbfs) //bfs탐색을 했다면
	{
		ofSetColor(0, 255, 0); //색조정
		if (isOpen) { //파일이 열려있다면
			pathdraw(); //탐색경로를 그려준다.
		}

		else //파일을 열지않았다면
			cout << "You must open file first" << endl;
	}

	if (isrfs) //rfs탐색을 했다면
	{
		if (isOpen) //파일이 열려있다면
		{
			rfsdraw(); //탐색경로를 그려준다. RFS는 BFS, DFS와 다른 방식으로 그려준다.
		}
		else //파일을 열지않았다면
			cout << "You must open file first" << endl;
	}

} 

void ofApp::keyPressed(int key) { //키 입력에 대한 함수
	
	if(key == VK_ESCAPE) { //esc를 입력받으면
			freeMemory(); //메모리 해제
			ofExit(); //종료
	}

	if (key == 'l') { //'l'키를 입력받으면 ----- 'LOAD' 파일입력
		readFile(); //미로 파일(.maz)을 입력받고 미로를 저장한다.
	}

	if (key == 's') { //'s'키를 입력받으면 ------ 'START' 시작점
		//flag 초기화
		isdfs = 0;
		isbfs = 0;
		isrfs = 0;
		if (start_flag)
			start_flag = 0; //이미 start_flag가 1이면 다시 0으로 돌려주고
		else {
			start_flag = 1; //아닐경우 start_flag를 1로, end_flag를 0으로한다.
			end_flag = 0;
		}
	}

	if (key == 'e') { //'e'키를 입력받으면 ----- 'END' 종점
		//flag 초기화
		isdfs = 0;
		isbfs = 0;
		isrfs = 0;
		if (end_flag)
			end_flag = 0; //이미 end_flag가 1이면 다시 0으로 돌려주고
		else {
			end_flag = 1; //아닐경우 end_flag를 1로, start_flag를 0으로한다.
			start_flag = 0;
		}
	}

	if (key == 'g') { //'g'키를 입력받으면 ----- 'GENERATE' 미로 생성
		readHW(); // 높이와 폭으로 된 텍스트파일을 입력받고, 미로를 생성해 저장한다.
	}

	if (key == 'r') { //'r'키를 입력받으면 ----- 'RFS' Random First Search
		if (isOpen) //파일이 열렸으면
		{
			RFS(); //RFS탐색을 해 경로를 저장한다.
		}
		else
			cout << "you must open file first" << endl;
	}

	if (key == 'b') { //'b'키를 입력받으면 ----- 'BFS' Breadth First Search
		if (isOpen) //파일이 열렸으면
		{
			BFS(); //BFS탐색을 해 경로를 저장한다.
		}
		else
			cout << "you must open file first" << endl;
	}

	if (key == 'd') { //'d'키를 입력받으면 ----- 'DFS' Depth First Search
		if (isOpen) //파일이 열렸으면
		{
			DFS(); //DFS탐색을 해 경로를 저장한다.
		}
		else
			cout << "you must open file first" << endl;
	}


}

void ofApp::keyReleased(int key){ //키 입력후 떼는것과 관련된 함수
	if (key == OF_KEY_LEFT) { //왼쪽 방향키를 누르고떼면
		if (start_flag) //start_flag가 1이면
			if (start.xpos -2 > 0 && Maze[start.ypos][start.xpos - 2].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				start.xpos-=2; //시점의 위치 옮김
		if (end_flag) //end_flag가 1이면
			if (end.xpos - 2 > 0 && Maze[end.ypos][end.xpos - 2].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				end.xpos-=2; //종점의 위치 옮김
	}

	if (key == OF_KEY_RIGHT) {
		if (start_flag) //start_flag가 1이면
			if (start.xpos + 2 < WIDTH && Maze[start.ypos][start.xpos + 2].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				start.xpos+=2; //시점의 위치 옮김

		if (end_flag) //end_flag가 1이면
			if (end.xpos + 2 < WIDTH && Maze[end.ypos][end.xpos + 2].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				end.xpos+=2; //종점의 위치 옮김
	}

	if (key == OF_KEY_UP) {
		if (start_flag) //start_flag가 1이면
			if (start.ypos - 2 > 0 && Maze[start.ypos-2][start.xpos].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				start.ypos-=2; //시점의 위치 옮김

		if (end_flag) //end_flag가 1이면
			if (end.ypos - 2 > 0 && Maze[end.ypos-2][end.xpos].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				end.ypos-=2; //종점의 위치 옮김
	}

	if (key == OF_KEY_DOWN) {
		if (start_flag) //start_flag가 1이면
			if (start.ypos + 2 < HEIGHT && Maze[start.ypos + 2][start.xpos].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				start.ypos+=2; //시점의 위치 옮김

		if (end_flag) //end_flag가 1이면
			if (end.ypos + 2 < HEIGHT && Maze[end.ypos + 2][end.xpos].state == EMPTY) //해당 방향으로 이동한 위치가 비어있다면
				end.ypos+=2; //종점의 위치 옮김
	}
}

void ofApp::drawmaze()
{
	int i = 0, j = 0;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (Maze[i][j].state == 0)  //높이와 폭에 따라, 해당 위치의 미로가 벽이 아니라면
				ofDrawRectangle(window_w, window_h, size, size); //해당 위치에 크기에 맞는 사각형을 그려준다.
			window_w += size; //크기에 따라 그려주는 위치변경
		}
		window_w = 0;
		window_h += size; //크기에 따라 그려주는 위치 변경
	}
}

bool ofApp::readHW() // 높이, 폭을 입력받으면 미로를 생성해서 저장하는 함수
{
	freeMemory(); //이전에 할당된 메모리를 해제한다.
	//flag를 초기화한다.
	isbfs = 0;
	isdfs = 0;
	isrfs = 0;
	isOpen = 0;
	start_flag = 0;
	end_flag = 0;
	
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .txt file"); //시스템 메세지로 "Select .txt file" 문자열을 출력
	string filePath;
	size_t pos;

	if (openFileResult.bSuccess) { //정상적으로 출력했다면
		ofLogVerbose("User selected a file"); // 해당 문자열 출력

		//파일 형식을 검사한다.
		string fileName = openFileResult.getName(); //파일의 이름을 입력받아서
		printf("file name is %s\n", fileName.c_str()); //"file name is 파일이름\n"을 출력한다.
		filePath = openFileResult.getPath(); //파일 경로를 해당 파일에 이르는 경로로 저장한다.
		printf("Open\n"); 
		pos = filePath.find_last_of("."); //파일이름에서 마지막 .을 찾는다.
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "txt") { //마지막 . 이후의 문자열이 "txt"인지, 즉 .txt 파일인가?

			ofFile file(fileName); //해당 .txt파일을 연다

			if (!file.exists()) { //해당 파일이 없다면
				cout << "Target file does not exists." << endl;
				return false;
			}
			else { //있다면
				cout << "We found the target file." << endl;
				isOpen = 2;     //******************************************** 높이, 폭만을 입력받으면 isOpen = 2, 미로를 통째로 입력받으면 isOpen = 1 
			}

			ofBuffer buffer(file); //파일 내용을 buffer에 저장

			ofBuffer::Line it = buffer.getLines().begin(); // it에 buffer의 시작 줄을 입력받는다.
		
			string line = *it; //line이 it를 가리키게 하고
			vector<string> words = ofSplitString(line, " "); //공백을 기준으로 문자열을 쪼갠다
			if (words.size() == 2) //쪼갠 문자열이 2개로 나눠질경우
			{
					WIDTH = atoi(words[0].c_str())*2 + 1; //첫번째는 가로(폭)
					HEIGHT = atoi(words[1].c_str())*2 + 1; //두번째는 세리(높이)로 입력받는다.
			}
			else //아닐경우 예외처리
			{
				cout << "File Open Process Error! " << endl;
				cout << "Please Input Correct File" << endl;
				isOpen = 0;
				return 0;
			}
			//입력받은 HEIGHT, WIDTH만큼 동적할당
			Maze = (maze**)malloc(sizeof(maze*) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				Maze[i] = (maze*)malloc(sizeof(maze) * WIDTH);

			MakeMaze(); //미로를 생성한다.

			//시작점과 종점을 입력한다.
			start.input(1, 1);
			end.input(WIDTH - 2, HEIGHT - 2);

			size = 20;
			while (1) //윈도우 창에 알맞게 크기 조절할 size변수의 적절한 크기를 조절한다.
			{
				if (HEIGHT * size <= 480 && WIDTH * size <= 640) break;
				size--; //미로의 전체길이가 창의 높이나 폭보다 크다면 size를 감소시키는걸 반복
				if (size == 1) break; //최소길이 1
			}
		}

		else { //아닐경우 예외처리
			printf("  Needs a '.txt' extension\n");
			return false;
		}
	}
}

void ofApp::MakeMaze() //미로를 생성한다. 알고리즘은 Hunt And Kill Algorithm
{
	initialize_maze(); //미로를 초기화, 이번경우 미로의 모든칸을 WALL로 한다.
	//미로의 무작위 위치 설정
	isOpen = 1;
	y = rand(1, (HEIGHT - 1) / 2);
	x = rand(1, (WIDTH - 1) / 2);
	//0~3의 랜덤난수 형성
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);				
															 
	int dir_count; //움직일수있는 방향 수
	int dir[4] = { 0,0,0,0 }; //움직일수있는 방향, 1이면 가능 0이면 불가능
	int t,k,i,j;

	while (1) //반복
	{
		Maze[y][x].state = EMPTY; //현재위치를 WALL -> EMPTY로 뚫어준다.
		dir_count = 0;
		dir_count = calcdir(dir); //인접한 뚫을수 있는 방향 계산
		if (dir_count == 0) //더이상 인접한 뚫을수있는 방이 없다면
		{
			//미로의 왼쪽 위부터 오른쪽 아래까지 순차적으로 탐색하며 해당 방은 WALL이고, 인접한 방중 EMPTY인 방이 있는 위치를 탐색
			for (i = 0; i < (HEIGHT - 1) / 2; i++) {
				for (j = 0; j < (WIDTH - 1) / 2; j++)
				{
					y = i * 2 + 1;
					x = j * 2 + 1;
					if (Maze[y][x].state == EMPTY) continue; //해당위치가 EMPTY면 for문 반복

					dir_count = calcdirr(dir); //미로와 맞닿는 방 있는지 찾기
					if (dir_count != 0) //미로와 맞닿는 방이 있다면
						break; //반복문 빠져나오기
				}
				if (dir_count != 0) break; //반복문 빠져나오기
			}

			if (i == (HEIGHT - 1) / 2 && j == (WIDTH - 1) / 2) return; //모든 방에대해 탐색을 마쳤다면 알고리즘 종료

			if (dir_count != 0) //뚫을수 있는 방이 있다면
			{
				for (k = 0; k < 4; k++)
					if (dir[k] == 1) break; //방향찾기

				//방향에 따라 미로와 인접한 중간 통로를 뚫어준다. (방과 방 사이의 공간)

				if (k == 0)
				{
					Maze[y - 1][x].state = EMPTY;
					continue;
				}

				else if (k == 1)
				{
					Maze[y][x+1].state = EMPTY;
					continue;
				}

				else if (k == 2)
				{
					Maze[y + 1][x].state = EMPTY;
					continue;
				}

				else if (k == 3)
				{
					Maze[y][x-1].state = EMPTY;
					continue;
				}
			}
			return;
		}

		while (1) {
			//인접한 뚫을수 있는 공간 중 무작위로 선정
			t = dis(gen);
			if (dir[t] == 1)
				break;
		}
		//방향에 따라 다음 방까지 이동하는 통로를 뚫고, 위치를 다음방으로 이동한다.
		if (t == 0) {
			Maze[y - 1][x].state = EMPTY;
			y -= 2;
		}
		else if (t == 1) {
			Maze[y][x + 1].state = EMPTY;
			x += 2;
		}
		else if (t == 2) {
			Maze[y + 1][x].state = EMPTY;
			y += 2;
		}
		else if (t == 3) {
			Maze[y][x - 1].state = EMPTY;
			x -= 2;
		}
	}
}

int ofApp::rand(int a,int b) //랜덤 난수 형성 함수
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(a, b);
	//a~b 사이의 무작위 난수를 설정하고 2를 곱하고 1을 뺀 값을 반환한다.
	return dis(gen) * 2 - 1;
}

int ofApp::calcdir(int* dirarray) //미로를 뚫을때, 뚫을수 있는 위치를 찾는 함수
{
	int dir = 0;
	
	if (y - 2 > 0) { //미로의 범위를 벗어나지 않고
		if (Maze[y - 2][x].state == WALL) { // 해당 방향의 다음방이 벽이라면
			dir++; //가능한 방향 수 ++
			dirarray[0] = 1; //해당 방향을 1로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
		}
		else 
			dirarray[0] = 0; //해당 방향을 0으로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
	}

	else //미로의 범위를 벗어나면
		dirarray[0] = 0; 
	
	if (x + 2 < WIDTH) { //미로의 범위를 벗어나지 않고
		if (Maze[y][x + 2].state == WALL) { // 해당 방향의 다음방이 벽이라면
			dir++;
			dirarray[1] = 1; //해당 방향을 1로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
		}
		else
			dirarray[1] = 0; //해당 방향을 0으로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
	}
	else
		dirarray[1] = 0;

	if (y + 2 < HEIGHT) { //미로의 범위를 벗어나지 않고
		if (Maze[y + 2][x].state == WALL) { // 해당 방향의 다음방이 벽이라면
			dir++;
			dirarray[2] = 1;  //해당 방향을 1로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
		}
		else
			dirarray[2] = 0;  //해당 방향을 0으로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
	}
	else
		dirarray[2] = 0;

	if (x - 2 > 0) { //미로의 범위를 벗어나지 않고
		if (Maze[y][x - 2].state == WALL) { // 해당 방향의 다음방이 벽이라면
			dir++;
			dirarray[3] = 1;  //해당 방향을 1로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
		}
		else
			dirarray[3] = 0;  //해당 방향을 0으로 설정 (1 = 뚫을수있음, 0 = 뚫을수없음)
	}
	else
		dirarray[3] = 0;

	return dir;
}

int ofApp::calcdirr(int* dirarray) //미로와 맞닿아있는 다음 방들 중 빈 방을 찾을때
{
	int dir=0;
	if (y - 2 > 0) { //미로의 범위를 벗어나지 않고
		if (Maze[y - 2][x].state == EMPTY) { // 해당 방향의 다음방이 빈 방이라면
			dir++; //가능한 방향 수 ++
			dirarray[0] = 1;  //해당 방향을 1로 설정 (1 = 비어있음, 0 = 막혀있음)
		}
		else
			dirarray[0] = 0;  //해당 방향을 0로 설정 (1 = 비어있음, 0 = 막혀있음)
	}
	else
		dirarray[0] = 0;
	//아래도 모두 동일
	if (x + 2 < WIDTH) {
		if (Maze[y][x + 2].state == EMPTY) {
			dir++;
			dirarray[1] = 1;
		}
		else
			dirarray[1] = 0;
	}
	else
		dirarray[1] = 0;

	if (y + 2 < HEIGHT) {
		if (Maze[y + 2][x].state == EMPTY) {
			dir++;
			dirarray[2] = 1;
		}
		else
			dirarray[2] = 0;
	}
	else
		dirarray[2] = 0;

	if (x - 2 > 0) {
		if (Maze[y][x - 2].state == EMPTY) {
			dir++;
			dirarray[3] = 1;
		}
		else
			dirarray[3] = 0;
	}
	else
		dirarray[3] = 0;

	return dir;
}

bool ofApp::readFile() //
{
	freeMemory(); //이전에 할당된 메모리를 해제한다.
	//flag를 초기화한다.
	isbfs = 0;
	isdfs = 0;
	isrfs = 0;
	HEIGHT = 0;
	WIDTH = 0;
	start_flag = 0;
	end_flag = 0;
	
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file"); //시스템 메세지로 " "문자열 출력
	string filePath;
	size_t pos;
	if (openFileResult.bSuccess) { //정상적으로 출력했다면
		ofLogVerbose("User selected a file"); //" "문자열 출력

		string fileName = openFileResult.getName(); //파일의 이름을 입력받고
		printf("file name is %s\n", fileName.c_str()); //" "출력
		filePath = openFileResult.getPath(); //파일 경로를 해당 파일 경로로 저장
		printf("Open\n");
		pos = filePath.find_last_of("."); //파일이름에서 마지막 .을 찾는다
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") { //마지막 .이후의 문자열이 "maz"인지, 즉 .maz파일인가?

			ofFile file(fileName); //해당 .maz파일 열기

			if (!file.exists()) { //해당 파일이 없다면
				cout << "Target file does not exists." << endl;
				return false;
			}
			else { //있다면
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file); //파일 내용을 buffer에 저장

			int idx = 0;
			int mazeidx = 0;
			//buffer의 시작줄부터 끝줄까지 한줄씩 입력받는다.
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) 
			{
				HEIGHT++;
			}
			//모든 줄에 대해 끝마쳤다면, 그 횟수가 미로의 높이
			//미로의 높이만큼 Maze변수 동적할당
			Maze = (maze**)malloc(sizeof(maze*) * HEIGHT);
			//buffer의 시작줄부터 끝줄까지 한줄씩 입력받는다.
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
			{
				string line = *it;
				WIDTH = line.size(); //한줄의 문자열 크기가 곧 미로의 폭(가로)

				Maze[idx] = (maze*)malloc(sizeof(maze) * WIDTH); //WIDTH만큼 미로를 동적할당

				for (mazeidx = 0; mazeidx < WIDTH; mazeidx++) //문자열의 모든 글자에 대해
				{
					if (line[mazeidx] == '|' || line[mazeidx] == '+' || line[mazeidx] == '-') //'|' '+' '-'이면 벽을 나타내므로
						Maze[idx][mazeidx].state = WALL; //벽
					else //아니면
						Maze[idx][mazeidx].state = EMPTY; //공간
				}
				idx++;
			}

			start.input(1, 1); //시점 입력
			end.input(WIDTH - 2, HEIGHT - 2); //종점 입력
			size = 20;

			while (1) //윈도우 창에 알맞게 크기 조절할 size변수
			{
				if (HEIGHT * size <= 480 && WIDTH * size <= 640) break;
				size--; //미로의 전체길이가 창의 높이나 폭보다 크다면 size를 감소시키는걸 반복
				if (size == 1) break; //최소길이 1
			}
		}

		else { //아니라면 예외처리
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() { //동적할당된 메모리 해제
	if (isOpen == 1) { //파일을 열었다면
		for (int i = 0; i < HEIGHT; i++)
			if (Maze[i] != NULL)
				free(Maze[i]);
		if (Maze != NULL)
			free(Maze); //2차원 배열 Maze를 동적할당
		printf("Allocated Memory has been freed.\n");
	}
	else
		printf("File is Not Opened.\n");
}

void position::input(int x, int y) //position 변수에 x,y값을 각각 넣어주는 함수
{
	this->xpos = x;
	this->ypos = y;
}

void ofApp::initialize_maze() { //미로의 초기화
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		{
			Maze[i][j].parent.input(-1, -1); //모든칸의 부모 미로를 (-1,-1)로 설정
			Maze[i][j].visit_time = 0; //방문횟수를 0으로 설정
			if (isOpen == 2) //높이, 폭을 입력받아 미로를 생성하는 알고리즘이라면
				Maze[i][j].state = WALL; //모든 미로를 벽으로 바꿔줌
		}
}

bool ofApp::DFS() //DFS탐색
{
	initialize_maze(); //초기화
	x = start.xpos;
	y = start.ypos; //x,y를 시점에 맞춰준다
	finddfs(); //종점에 도달할때까지 dfs탐색한다.
	//flag를 알맞게 설정한다.
	isdfs = 1;
	isbfs = 0;
	isrfs = 0;
	start_flag = 0;
	end_flag = 0;
	return 0;
}

void ofApp::finddfs()
{
	stack<position> S; //스택 S를 선언한다.
	position t;
	t.input(x, y); 
	S.push(t); //현재 위치를 S에 넣는다.

	while (!S.empty()) //S가 비었다면 반복종료
	{
		if (Maze[end.ypos][end.xpos].visit_time != 0) //만약 종점을 방문했다면
			break; //탐색종료

		x = S.top().xpos;
		y = S.top().ypos;
		S.pop(); //x,y를 S에서 POP한 값으로 바꿔준다.

		Maze[y][x].visit_time++; //해당 위치를 방문

		if (x - 1 >= 0) //미로의 범위를 벗어나지 않고
			if (Maze[y][x-1].state == EMPTY && Maze[y][x-1].visit_time == 0) { //해당 방향을 방문한적 없고, 비어있다면
				x--;
				t.input(x, y);
				S.push(t);
				//해당 위치를 S에 push한다.
				Maze[y][x].parent.ypos = y;
				Maze[y][x].parent.xpos = x + 1;
				//해당 위치의 부모를 현재 위치로 한다.
				x++;//원래대로 되돌림
			}
		//아래 3개 모두 동일한 방식
		if (y - 1 >= 0)
			if (Maze[y-1][x].state == EMPTY && Maze[y-1][x].visit_time == 0) {
				y--;
				t.input(x, y);
				S.push(t);

				Maze[y][x].parent.ypos = y+1;
				Maze[y][x].parent.xpos = x;
				y++;
			}

		if (y + 1 < HEIGHT)
			if (Maze[y + 1][x].state == EMPTY && Maze[y+1][x].visit_time == 0) {
				y++;
				t.input(x, y);
				S.push(t);

				Maze[y][x].parent.ypos = y - 1;
				Maze[y][x].parent.xpos = x;
				y--;
			}

		if (x + 1 < WIDTH)
			if (Maze[y][x + 1].state == EMPTY && Maze[y][x + 1].visit_time == 0) {
				x++;
				t.input(x, y);
				S.push(t);

				Maze[y][x].parent.ypos = y;
				Maze[y][x].parent.xpos = x - 1;
				x--;
			}
	}
}

bool ofApp::BFS() //BFS 탐색 알고리즘
{
	initialize_maze(); //초기화

	x = start.xpos;
	y = start.ypos; //시점설정
	findbfs(); //종점에 도달할때까지 BFS탐색
	isbfs = 1;
	isdfs = 0;
	isrfs = 0;
	start_flag = 0;
	end_flag = 0;
	return 0;
}

void ofApp::findbfs()
{
	position t;
	t.input(x, y);
	queue<position> Q; //Queue Q를 선언
	Q.push(t); //Q에 현재위치 삽입

	while (!Q.empty()) //Q가 비어있으면 반복종료
	{
		x = Q.front().xpos;
		y = Q.front().ypos;
		Q.pop(); //Q에서 pop한 값을 x,y에 넣는다
		Maze[y][x].visit_time++; //현재 위치를 방문
		if (Maze[end.ypos][end.xpos].visit_time != 0) //종점을 방문했다면
			break; //알고리즘 종료

		if (y + 1 < HEIGHT) //미로 범위를 벗어나지 않으면서
			if (Maze[y + 1][x].state == EMPTY && Maze[y + 1][x].visit_time == 0) { //해당 방향이 방문한적 없고, 비어있다면
				y++;
				t.input(x, y);
				Q.push(t); //해당 위치를 Q에 push
				Maze[y][x].parent.ypos = y - 1;
				Maze[y][x].parent.xpos = x; //해당 위치의 부모를 현재위치로 설정
				y--; //원래대로
			}
		//아래 3개 모두 동일한 방식
		if (x + 1 < WIDTH)
			if (Maze[y][x + 1].state == EMPTY && Maze[y][x + 1].visit_time == 0) {
				x++;
				t.input(x, y);
				Q.push(t);
				Maze[y][x].parent.ypos = y;
				Maze[y][x].parent.xpos = x - 1;
				x--;
			}

		if (x - 1 >= 0)
			if (Maze[y][x - 1].state == EMPTY && Maze[y][x - 1].visit_time == 0) {
				x--;
				t.input(x, y);
				Q.push(t);
				
				Maze[y][x].parent.ypos = y;
				Maze[y][x].parent.xpos = x + 1;
				x++;
			}

		if (y - 1 >= 0)
			if (Maze[y - 1][x].state == EMPTY && Maze[y - 1][x].visit_time == 0) {
				y--;
				t.input(x, y);
				Q.push(t);
				
				Maze[y][x].parent.ypos = y + 1;
				Maze[y][x].parent.xpos = x;
				y++;
			}
	}
}

bool ofApp::RFS()
{
	initialize_maze(); //초기화
	x = start.xpos;
	y = start.ypos; //시작점 설정
	findrfs(); //RFS 탐색
	//flag를 알맞게 설정
	isbfs = 0;
	isdfs = 0;
	isrfs = 1;
	start_flag = 0;
	end_flag = 0;
	cout << "Movement : " << Mov << endl; //이동 횟수 출력
	return 0;
}

void ofApp::findrfs()
{
	position t[3]; //가장 최근까지 이동했던 위치정보 3개를 저장하는 배열 ----- t[0]이 바로 전, t[1]이 2번 전, t[2]가 3번 전
	position temp[3]; //임시로 위치를 저장하는 배열
	t[0].input(-1, -1);
	t[1].input(-1, -1);
	t[2].input(-1, -1); //기억하는 경로 초기화
	Mov = -1; //이동횟수 초기화

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3); //랜덤 방향 생성

	int dir_count; //이동 가능한 방향 수
	int dir[4] = { 0,0,0,0 }; //이동 가능한 방향
	int k,i=0;
	int idx = 0;
	int tempx, tempy;
	while (1)
	{
		Mov++; //이동횟수++
		Maze[y][x].visit_time++; //현재위치 방문
		Maze[y][x].parent.input(t[0].xpos, t[0].ypos); //현재위치의 부모를 가장 최근에 방문한 t[0]으로 설정

		if (y == end.ypos && x == end.xpos) //현위치가 종점이라면
			return; //알고리즘 종료

		dir_count = calcpath(dir); //방문 가능한 방향 확인
		dir_count = excludepath(dir_count, dir, t); //방문 가능한 방향중 최근3회 이내에 방문했던 방향 제외

		if (dir_count <= 0) //최근3회 이내 방문했던 방향을 제외하면, 갈수있는 경로가 없다면
		{

			while (1)
			{
				for (int l = 0; l < 3; l++)
					temp[l].input(t[l].xpos, t[l].ypos); //임시 배열에 최근3회 방문 위치 저장
				idx = 0;
				
				tempx = x;
				tempy = y;
				x = temp[idx].xpos;
				y = temp[idx].ypos;
				idx++; // 바로 전에 방문했던 점으로 이동

				t[2].input(t[1].xpos, t[1].ypos);
				t[1].input(t[0].xpos, t[0].ypos);
				t[0].input(tempx, tempy); //최근3회 방문위치 재설정

				dir_count = calcpath(dir); //방문 가능한 방향 확인
				dir_count = excludepath(dir_count, dir, t); //방문 가능한 방향중 최근3회 이내에 방문했던 방향 제외

				if (dir_count == 0) continue; //여전히 방문가능한 점이 없다면 반복
				else
				{
					while (1) {
						//방문가능한 무작위 방향 설정
						k = dis(gen);
						if (dir[k] == 1)
							break;
					}
					visitrfs(t, k); //알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
					break;
				}
			}
		}

		else
		{
			while (1) {
				//방문 가능한 무작위 방향 설정
				k = dis(gen);
				if (dir[k] == 1)
					break;
			}
			visitrfs(t, k); //알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
			continue;
		}
	}
}

void ofApp::visitrfs(position* t, int k) { // RFS 탐색시 방문하며 위치를 변경하는 함수

	//알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
	if (k == 0) {
		Maze[y - 1][x].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		y -= 2;
	}
	//알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
	else if (k == 1) {
		Maze[y][x + 1].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		x += 2;
	}
	//알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
	else if (k == 2) {
		Maze[y + 1][x].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		y += 2;
	}
	//알맞은 방향에 따라 미로 방문, 최근3회 방문위치 재설정, 좌표변경
	else if (k == 3) {
		Maze[y][x - 1].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		x -= 2;
	}
}

int ofApp::calcpath(int* dir) //RFS 탐색중 이동 가능한 방향을 탐색하는 함수
{
	int dir_count = 0;
	if (y - 1 > 0) { //미로 범위를 벗어나지 않고
		if (Maze[y - 1][x].state == EMPTY) { //해당 방향의 미로가 비어있다면
			dir_count++; //이동 가능 방향의 수 ++
			dir[0] = 1; //갈수 있다고 표시( 1 = 갈수있음, 0 = 못감)
		}
		else
			dir[0] = 0; //갈수 없다고 표시( 1 = 갈수있음, 0 = 못감)
	}
	else
		dir[0] = 0; //갈수 없다고 표시( 1 = 갈수있음, 0 = 못감)
	//아래도 모두 동일
	if (x + 1 < WIDTH) {
		if (Maze[y][x + 1].state == EMPTY) {
			dir_count++;
			dir[1] = 1;
		}
		else
			dir[1] = 0;
	}
	else
		dir[1] = 0;

	if (y + 1 < HEIGHT) {
		if (Maze[y + 1][x].state == EMPTY) {
			dir_count++;
			dir[2] = 1;
		}
		else
			dir[2] = 0;
	}
	else
		dir[2] = 0;

	if (x - 1 > 0) {
		if (Maze[y][x - 1].state == EMPTY) {
			dir_count++;
			dir[3] = 1;
		}
		else
			dir[3] = 0;
	}
	else
		dir[3] = 0;

	return dir_count; //이동 가능한 방향의 수를 반환
}

int ofApp::excludepath(int count, int* dir, position* temp) //RFS탐색시, 최근3회 이내에 거쳤던 위치로 가는 방향을 제외하는 함수
{
	if (dir[0] == 1) //방향에 대해
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[j].xpos == x && temp[j].ypos == y - 2) { //최근 3회 방문을 저장하는 temp[]와 비교해서, 같다면
				count--;
				dir[0] = 0; //방문가능여부를 0으로, 접근 방향 갯수를 1줄인다
				break;
			}
		}
	}
	//아래 3개 모두 동일하다
	if (dir[1] == 1)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[j].xpos == x + 2 && temp[j].ypos == y) {
				count--;
				dir[1] = 0;
				break;
			}
		}
	}

	if (dir[2] == 1)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[j].xpos == x && temp[j].ypos == y + 2) {
				count--;
				dir[2] = 0;
				break;
			}
		}
	}

	if (dir[3] == 1)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[j].xpos == x - 2 && temp[j].ypos == y) {
				count--;
				dir[3] = 0;
				break;
			}
		}
	}

	return count; //접근 가능한 방향 갯수를 반환
}

void ofApp::pathdraw() //DFS, BFS시 방문 여부 및 최단경로를 그려주는 함수
{
	//방문한 모든노드 그려주기
	int i, j;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) { //높이, 폭에 따른 모든 위치에 대하여
			if (Maze[i][j].visit_time != 0) //해당 위치의 미로를 방문한적이 있다면
				ofDrawRectangle(window_w, window_h, size, size); //사각형을 그려준다
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
	//최단경로 그려주기
	y = end.ypos, x = end.xpos; //종점으로 설정
	int tempy, tempx;
	window_h = y * size;
	window_w = x * size; //종점에 해당하는 위치로 변경
	ofSetColor(0, 0, 255); //색깔 변경
	ofDrawRectangle(window_w, window_h, size, size);  //종점을 그려준다

	while (1)
	{
		if (Maze[y][x].parent.xpos == -1) break; //부모가 없는점, 즉 시점일 때 반복종료
		tempy = Maze[y][x].parent.ypos;
		tempx = Maze[y][x].parent.xpos; 
		y = tempy;
		x = tempx; //부모의 위치로 점을 옮긴다
		window_h = y * size;
		window_w = x * size; //점이 해당하는 위치로 변경
		ofDrawRectangle(window_w, window_h, size, size); //경로를 그려준다
	}
}

void ofApp::rfsdraw() //RFS방문시, 방문 횟수에 따라 색을 다르게 해서 그려준다
{
	int i, j;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (Maze[i][j].visit_time > 0) //방문한 적이 있다면
			{
				int k = 102 + Maze[i][j].visit_time * 10; //많이 방문한 방일수록 색이 선명한 빨강 , 적을수록 탁한 적색으로
				if (k > 255) k = 255; //out-of-range
				ofSetColor(k,0,0); //색설정
				ofDrawRectangle(window_w, window_h, size, size); //그려주기
			}
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
}
