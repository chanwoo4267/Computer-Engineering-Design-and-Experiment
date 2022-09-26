
#include "ofApp.h"
#include <iostream>
using namespace std;


void ofApp::setup() {

	ofSetWindowTitle("Maze Project"); //â �������� Maze Project��� ���� ����
	ofSetFrameRate(15); //�ֻ��� ����
	ofBackground(255, 255, 255); //���� ����
	
	windowWidth = ofGetWidth(); //ȭ�� ũ�⿡ ���� ��������
	windowHeight = ofGetHeight(); //ȭ�� ũ�⿡ ���� ��������

	//���� flag �� ������ �ʱ�ȭ
	HEIGHT = 0;
	WIDTH = 0;
	Mov = 0;

	isdfs = false;
	isbfs = false;
	isrfs = false;
	start_flag = 0;
	end_flag = 0;
	isOpen = 0; 

	//������ ������ �������� â�� ������� ��ġ�� ȭ�� �߾����� ����
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	//verdana.ttf��� ��Ʈ�� �Է¹���
	myFont.loadFont("verdana.ttf", 12, true, true);

}


void ofApp::update() {

}


void ofApp::draw() {
	ofSetColor(100); //������ �� ����
	ofSetLineWidth(5); //���� 5�� ���� �׸���
	
	//input[][] : 1�� ���, 0�� �� ���� ���� ���� �簢���� �׸��¹������ �׷��ֱ�
	if (isOpen) {

		drawmaze();
	}

	if (start_flag) { //�������� �ű�°�?
		//���������� �׷��ִ� ��ġ�� �ű��
		int window_h = start.ypos * size;
		int window_w = start.xpos * size;
		//������
		ofSetColor(0);
		//�ش� ��ġ�� ũ�⿡ �´� �簢�� �׸���
		ofDrawRectangle(window_w, window_h, size, size);
	}

	if (end_flag) { //������ �ű�°�?
		//�������� �׷��ִ� ��ġ�� �ű��.
		int window_h = end.ypos * size;
		int window_w = end.xpos * size;
		//������
		ofSetColor(0);
		//�ش� ��ġ�� ũ�⿡ �´� �簢�� �׸���
		ofDrawRectangle(window_w, window_h, size, size);
	}

	if (isdfs) //dfsŽ���� �ߴٸ�
	{
		ofSetColor(0,255,0); //�� ����
		if (isOpen) { //������ �����ִٸ�
			pathdraw(); //Ž�� ��θ� �׷��ش�.
		}
			
		else //������ �����ʾҴٸ�
			cout << "You must open file first" << endl; 
	}

	if (isbfs) //bfsŽ���� �ߴٸ�
	{
		ofSetColor(0, 255, 0); //������
		if (isOpen) { //������ �����ִٸ�
			pathdraw(); //Ž����θ� �׷��ش�.
		}

		else //������ �����ʾҴٸ�
			cout << "You must open file first" << endl;
	}

	if (isrfs) //rfsŽ���� �ߴٸ�
	{
		if (isOpen) //������ �����ִٸ�
		{
			rfsdraw(); //Ž����θ� �׷��ش�. RFS�� BFS, DFS�� �ٸ� ������� �׷��ش�.
		}
		else //������ �����ʾҴٸ�
			cout << "You must open file first" << endl;
	}

} 

void ofApp::keyPressed(int key) { //Ű �Է¿� ���� �Լ�
	
	if(key == VK_ESCAPE) { //esc�� �Է¹�����
			freeMemory(); //�޸� ����
			ofExit(); //����
	}

	if (key == 'l') { //'l'Ű�� �Է¹����� ----- 'LOAD' �����Է�
		readFile(); //�̷� ����(.maz)�� �Է¹ް� �̷θ� �����Ѵ�.
	}

	if (key == 's') { //'s'Ű�� �Է¹����� ------ 'START' ������
		//flag �ʱ�ȭ
		isdfs = 0;
		isbfs = 0;
		isrfs = 0;
		if (start_flag)
			start_flag = 0; //�̹� start_flag�� 1�̸� �ٽ� 0���� �����ְ�
		else {
			start_flag = 1; //�ƴҰ�� start_flag�� 1��, end_flag�� 0�����Ѵ�.
			end_flag = 0;
		}
	}

	if (key == 'e') { //'e'Ű�� �Է¹����� ----- 'END' ����
		//flag �ʱ�ȭ
		isdfs = 0;
		isbfs = 0;
		isrfs = 0;
		if (end_flag)
			end_flag = 0; //�̹� end_flag�� 1�̸� �ٽ� 0���� �����ְ�
		else {
			end_flag = 1; //�ƴҰ�� end_flag�� 1��, start_flag�� 0�����Ѵ�.
			start_flag = 0;
		}
	}

	if (key == 'g') { //'g'Ű�� �Է¹����� ----- 'GENERATE' �̷� ����
		readHW(); // ���̿� ������ �� �ؽ�Ʈ������ �Է¹ް�, �̷θ� ������ �����Ѵ�.
	}

	if (key == 'r') { //'r'Ű�� �Է¹����� ----- 'RFS' Random First Search
		if (isOpen) //������ ��������
		{
			RFS(); //RFSŽ���� �� ��θ� �����Ѵ�.
		}
		else
			cout << "you must open file first" << endl;
	}

	if (key == 'b') { //'b'Ű�� �Է¹����� ----- 'BFS' Breadth First Search
		if (isOpen) //������ ��������
		{
			BFS(); //BFSŽ���� �� ��θ� �����Ѵ�.
		}
		else
			cout << "you must open file first" << endl;
	}

	if (key == 'd') { //'d'Ű�� �Է¹����� ----- 'DFS' Depth First Search
		if (isOpen) //������ ��������
		{
			DFS(); //DFSŽ���� �� ��θ� �����Ѵ�.
		}
		else
			cout << "you must open file first" << endl;
	}


}

void ofApp::keyReleased(int key){ //Ű �Է��� ���°Ͱ� ���õ� �Լ�
	if (key == OF_KEY_LEFT) { //���� ����Ű�� ��������
		if (start_flag) //start_flag�� 1�̸�
			if (start.xpos -2 > 0 && Maze[start.ypos][start.xpos - 2].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				start.xpos-=2; //������ ��ġ �ű�
		if (end_flag) //end_flag�� 1�̸�
			if (end.xpos - 2 > 0 && Maze[end.ypos][end.xpos - 2].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				end.xpos-=2; //������ ��ġ �ű�
	}

	if (key == OF_KEY_RIGHT) {
		if (start_flag) //start_flag�� 1�̸�
			if (start.xpos + 2 < WIDTH && Maze[start.ypos][start.xpos + 2].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				start.xpos+=2; //������ ��ġ �ű�

		if (end_flag) //end_flag�� 1�̸�
			if (end.xpos + 2 < WIDTH && Maze[end.ypos][end.xpos + 2].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				end.xpos+=2; //������ ��ġ �ű�
	}

	if (key == OF_KEY_UP) {
		if (start_flag) //start_flag�� 1�̸�
			if (start.ypos - 2 > 0 && Maze[start.ypos-2][start.xpos].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				start.ypos-=2; //������ ��ġ �ű�

		if (end_flag) //end_flag�� 1�̸�
			if (end.ypos - 2 > 0 && Maze[end.ypos-2][end.xpos].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				end.ypos-=2; //������ ��ġ �ű�
	}

	if (key == OF_KEY_DOWN) {
		if (start_flag) //start_flag�� 1�̸�
			if (start.ypos + 2 < HEIGHT && Maze[start.ypos + 2][start.xpos].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				start.ypos+=2; //������ ��ġ �ű�

		if (end_flag) //end_flag�� 1�̸�
			if (end.ypos + 2 < HEIGHT && Maze[end.ypos + 2][end.xpos].state == EMPTY) //�ش� �������� �̵��� ��ġ�� ����ִٸ�
				end.ypos+=2; //������ ��ġ �ű�
	}
}

void ofApp::drawmaze()
{
	int i = 0, j = 0;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (Maze[i][j].state == 0)  //���̿� ���� ����, �ش� ��ġ�� �̷ΰ� ���� �ƴ϶��
				ofDrawRectangle(window_w, window_h, size, size); //�ش� ��ġ�� ũ�⿡ �´� �簢���� �׷��ش�.
			window_w += size; //ũ�⿡ ���� �׷��ִ� ��ġ����
		}
		window_w = 0;
		window_h += size; //ũ�⿡ ���� �׷��ִ� ��ġ ����
	}
}

bool ofApp::readHW() // ����, ���� �Է¹����� �̷θ� �����ؼ� �����ϴ� �Լ�
{
	freeMemory(); //������ �Ҵ�� �޸𸮸� �����Ѵ�.
	//flag�� �ʱ�ȭ�Ѵ�.
	isbfs = 0;
	isdfs = 0;
	isrfs = 0;
	isOpen = 0;
	start_flag = 0;
	end_flag = 0;
	
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .txt file"); //�ý��� �޼����� "Select .txt file" ���ڿ��� ���
	string filePath;
	size_t pos;

	if (openFileResult.bSuccess) { //���������� ����ߴٸ�
		ofLogVerbose("User selected a file"); // �ش� ���ڿ� ���

		//���� ������ �˻��Ѵ�.
		string fileName = openFileResult.getName(); //������ �̸��� �Է¹޾Ƽ�
		printf("file name is %s\n", fileName.c_str()); //"file name is �����̸�\n"�� ����Ѵ�.
		filePath = openFileResult.getPath(); //���� ��θ� �ش� ���Ͽ� �̸��� ��η� �����Ѵ�.
		printf("Open\n"); 
		pos = filePath.find_last_of("."); //�����̸����� ������ .�� ã�´�.
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "txt") { //������ . ������ ���ڿ��� "txt"����, �� .txt �����ΰ�?

			ofFile file(fileName); //�ش� .txt������ ����

			if (!file.exists()) { //�ش� ������ ���ٸ�
				cout << "Target file does not exists." << endl;
				return false;
			}
			else { //�ִٸ�
				cout << "We found the target file." << endl;
				isOpen = 2;     //******************************************** ����, ������ �Է¹����� isOpen = 2, �̷θ� ��°�� �Է¹����� isOpen = 1 
			}

			ofBuffer buffer(file); //���� ������ buffer�� ����

			ofBuffer::Line it = buffer.getLines().begin(); // it�� buffer�� ���� ���� �Է¹޴´�.
		
			string line = *it; //line�� it�� ����Ű�� �ϰ�
			vector<string> words = ofSplitString(line, " "); //������ �������� ���ڿ��� �ɰ���
			if (words.size() == 2) //�ɰ� ���ڿ��� 2���� ���������
			{
					WIDTH = atoi(words[0].c_str())*2 + 1; //ù��°�� ����(��)
					HEIGHT = atoi(words[1].c_str())*2 + 1; //�ι�°�� ����(����)�� �Է¹޴´�.
			}
			else //�ƴҰ�� ����ó��
			{
				cout << "File Open Process Error! " << endl;
				cout << "Please Input Correct File" << endl;
				isOpen = 0;
				return 0;
			}
			//�Է¹��� HEIGHT, WIDTH��ŭ �����Ҵ�
			Maze = (maze**)malloc(sizeof(maze*) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				Maze[i] = (maze*)malloc(sizeof(maze) * WIDTH);

			MakeMaze(); //�̷θ� �����Ѵ�.

			//�������� ������ �Է��Ѵ�.
			start.input(1, 1);
			end.input(WIDTH - 2, HEIGHT - 2);

			size = 20;
			while (1) //������ â�� �˸°� ũ�� ������ size������ ������ ũ�⸦ �����Ѵ�.
			{
				if (HEIGHT * size <= 480 && WIDTH * size <= 640) break;
				size--; //�̷��� ��ü���̰� â�� ���̳� ������ ũ�ٸ� size�� ���ҽ�Ű�°� �ݺ�
				if (size == 1) break; //�ּұ��� 1
			}
		}

		else { //�ƴҰ�� ����ó��
			printf("  Needs a '.txt' extension\n");
			return false;
		}
	}
}

void ofApp::MakeMaze() //�̷θ� �����Ѵ�. �˰����� Hunt And Kill Algorithm
{
	initialize_maze(); //�̷θ� �ʱ�ȭ, �̹���� �̷��� ���ĭ�� WALL�� �Ѵ�.
	//�̷��� ������ ��ġ ����
	isOpen = 1;
	y = rand(1, (HEIGHT - 1) / 2);
	x = rand(1, (WIDTH - 1) / 2);
	//0~3�� �������� ����
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);				
															 
	int dir_count; //�����ϼ��ִ� ���� ��
	int dir[4] = { 0,0,0,0 }; //�����ϼ��ִ� ����, 1�̸� ���� 0�̸� �Ұ���
	int t,k,i,j;

	while (1) //�ݺ�
	{
		Maze[y][x].state = EMPTY; //������ġ�� WALL -> EMPTY�� �վ��ش�.
		dir_count = 0;
		dir_count = calcdir(dir); //������ ������ �ִ� ���� ���
		if (dir_count == 0) //���̻� ������ �������ִ� ���� ���ٸ�
		{
			//�̷��� ���� ������ ������ �Ʒ����� ���������� Ž���ϸ� �ش� ���� WALL�̰�, ������ ���� EMPTY�� ���� �ִ� ��ġ�� Ž��
			for (i = 0; i < (HEIGHT - 1) / 2; i++) {
				for (j = 0; j < (WIDTH - 1) / 2; j++)
				{
					y = i * 2 + 1;
					x = j * 2 + 1;
					if (Maze[y][x].state == EMPTY) continue; //�ش���ġ�� EMPTY�� for�� �ݺ�

					dir_count = calcdirr(dir); //�̷ο� �´�� �� �ִ��� ã��
					if (dir_count != 0) //�̷ο� �´�� ���� �ִٸ�
						break; //�ݺ��� ����������
				}
				if (dir_count != 0) break; //�ݺ��� ����������
			}

			if (i == (HEIGHT - 1) / 2 && j == (WIDTH - 1) / 2) return; //��� �濡���� Ž���� ���ƴٸ� �˰��� ����

			if (dir_count != 0) //������ �ִ� ���� �ִٸ�
			{
				for (k = 0; k < 4; k++)
					if (dir[k] == 1) break; //����ã��

				//���⿡ ���� �̷ο� ������ �߰� ��θ� �վ��ش�. (��� �� ������ ����)

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
			//������ ������ �ִ� ���� �� �������� ����
			t = dis(gen);
			if (dir[t] == 1)
				break;
		}
		//���⿡ ���� ���� ����� �̵��ϴ� ��θ� �հ�, ��ġ�� ���������� �̵��Ѵ�.
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

int ofApp::rand(int a,int b) //���� ���� ���� �Լ�
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(a, b);
	//a~b ������ ������ ������ �����ϰ� 2�� ���ϰ� 1�� �� ���� ��ȯ�Ѵ�.
	return dis(gen) * 2 - 1;
}

int ofApp::calcdir(int* dirarray) //�̷θ� ������, ������ �ִ� ��ġ�� ã�� �Լ�
{
	int dir = 0;
	
	if (y - 2 > 0) { //�̷��� ������ ����� �ʰ�
		if (Maze[y - 2][x].state == WALL) { // �ش� ������ �������� ���̶��
			dir++; //������ ���� �� ++
			dirarray[0] = 1; //�ش� ������ 1�� ���� (1 = ����������, 0 = ����������)
		}
		else 
			dirarray[0] = 0; //�ش� ������ 0���� ���� (1 = ����������, 0 = ����������)
	}

	else //�̷��� ������ �����
		dirarray[0] = 0; 
	
	if (x + 2 < WIDTH) { //�̷��� ������ ����� �ʰ�
		if (Maze[y][x + 2].state == WALL) { // �ش� ������ �������� ���̶��
			dir++;
			dirarray[1] = 1; //�ش� ������ 1�� ���� (1 = ����������, 0 = ����������)
		}
		else
			dirarray[1] = 0; //�ش� ������ 0���� ���� (1 = ����������, 0 = ����������)
	}
	else
		dirarray[1] = 0;

	if (y + 2 < HEIGHT) { //�̷��� ������ ����� �ʰ�
		if (Maze[y + 2][x].state == WALL) { // �ش� ������ �������� ���̶��
			dir++;
			dirarray[2] = 1;  //�ش� ������ 1�� ���� (1 = ����������, 0 = ����������)
		}
		else
			dirarray[2] = 0;  //�ش� ������ 0���� ���� (1 = ����������, 0 = ����������)
	}
	else
		dirarray[2] = 0;

	if (x - 2 > 0) { //�̷��� ������ ����� �ʰ�
		if (Maze[y][x - 2].state == WALL) { // �ش� ������ �������� ���̶��
			dir++;
			dirarray[3] = 1;  //�ش� ������ 1�� ���� (1 = ����������, 0 = ����������)
		}
		else
			dirarray[3] = 0;  //�ش� ������ 0���� ���� (1 = ����������, 0 = ����������)
	}
	else
		dirarray[3] = 0;

	return dir;
}

int ofApp::calcdirr(int* dirarray) //�̷ο� �´���ִ� ���� ��� �� �� ���� ã����
{
	int dir=0;
	if (y - 2 > 0) { //�̷��� ������ ����� �ʰ�
		if (Maze[y - 2][x].state == EMPTY) { // �ش� ������ �������� �� ���̶��
			dir++; //������ ���� �� ++
			dirarray[0] = 1;  //�ش� ������ 1�� ���� (1 = �������, 0 = ��������)
		}
		else
			dirarray[0] = 0;  //�ش� ������ 0�� ���� (1 = �������, 0 = ��������)
	}
	else
		dirarray[0] = 0;
	//�Ʒ��� ��� ����
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
	freeMemory(); //������ �Ҵ�� �޸𸮸� �����Ѵ�.
	//flag�� �ʱ�ȭ�Ѵ�.
	isbfs = 0;
	isdfs = 0;
	isrfs = 0;
	HEIGHT = 0;
	WIDTH = 0;
	start_flag = 0;
	end_flag = 0;
	
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file"); //�ý��� �޼����� " "���ڿ� ���
	string filePath;
	size_t pos;
	if (openFileResult.bSuccess) { //���������� ����ߴٸ�
		ofLogVerbose("User selected a file"); //" "���ڿ� ���

		string fileName = openFileResult.getName(); //������ �̸��� �Է¹ް�
		printf("file name is %s\n", fileName.c_str()); //" "���
		filePath = openFileResult.getPath(); //���� ��θ� �ش� ���� ��η� ����
		printf("Open\n");
		pos = filePath.find_last_of("."); //�����̸����� ������ .�� ã�´�
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") { //������ .������ ���ڿ��� "maz"����, �� .maz�����ΰ�?

			ofFile file(fileName); //�ش� .maz���� ����

			if (!file.exists()) { //�ش� ������ ���ٸ�
				cout << "Target file does not exists." << endl;
				return false;
			}
			else { //�ִٸ�
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file); //���� ������ buffer�� ����

			int idx = 0;
			int mazeidx = 0;
			//buffer�� �����ٺ��� ���ٱ��� ���پ� �Է¹޴´�.
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) 
			{
				HEIGHT++;
			}
			//��� �ٿ� ���� �����ƴٸ�, �� Ƚ���� �̷��� ����
			//�̷��� ���̸�ŭ Maze���� �����Ҵ�
			Maze = (maze**)malloc(sizeof(maze*) * HEIGHT);
			//buffer�� �����ٺ��� ���ٱ��� ���پ� �Է¹޴´�.
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
			{
				string line = *it;
				WIDTH = line.size(); //������ ���ڿ� ũ�Ⱑ �� �̷��� ��(����)

				Maze[idx] = (maze*)malloc(sizeof(maze) * WIDTH); //WIDTH��ŭ �̷θ� �����Ҵ�

				for (mazeidx = 0; mazeidx < WIDTH; mazeidx++) //���ڿ��� ��� ���ڿ� ����
				{
					if (line[mazeidx] == '|' || line[mazeidx] == '+' || line[mazeidx] == '-') //'|' '+' '-'�̸� ���� ��Ÿ���Ƿ�
						Maze[idx][mazeidx].state = WALL; //��
					else //�ƴϸ�
						Maze[idx][mazeidx].state = EMPTY; //����
				}
				idx++;
			}

			start.input(1, 1); //���� �Է�
			end.input(WIDTH - 2, HEIGHT - 2); //���� �Է�
			size = 20;

			while (1) //������ â�� �˸°� ũ�� ������ size����
			{
				if (HEIGHT * size <= 480 && WIDTH * size <= 640) break;
				size--; //�̷��� ��ü���̰� â�� ���̳� ������ ũ�ٸ� size�� ���ҽ�Ű�°� �ݺ�
				if (size == 1) break; //�ּұ��� 1
			}
		}

		else { //�ƴ϶�� ����ó��
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() { //�����Ҵ�� �޸� ����
	if (isOpen == 1) { //������ �����ٸ�
		for (int i = 0; i < HEIGHT; i++)
			if (Maze[i] != NULL)
				free(Maze[i]);
		if (Maze != NULL)
			free(Maze); //2���� �迭 Maze�� �����Ҵ�
		printf("Allocated Memory has been freed.\n");
	}
	else
		printf("File is Not Opened.\n");
}

void position::input(int x, int y) //position ������ x,y���� ���� �־��ִ� �Լ�
{
	this->xpos = x;
	this->ypos = y;
}

void ofApp::initialize_maze() { //�̷��� �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		{
			Maze[i][j].parent.input(-1, -1); //���ĭ�� �θ� �̷θ� (-1,-1)�� ����
			Maze[i][j].visit_time = 0; //�湮Ƚ���� 0���� ����
			if (isOpen == 2) //����, ���� �Է¹޾� �̷θ� �����ϴ� �˰����̶��
				Maze[i][j].state = WALL; //��� �̷θ� ������ �ٲ���
		}
}

bool ofApp::DFS() //DFSŽ��
{
	initialize_maze(); //�ʱ�ȭ
	x = start.xpos;
	y = start.ypos; //x,y�� ������ �����ش�
	finddfs(); //������ �����Ҷ����� dfsŽ���Ѵ�.
	//flag�� �˸°� �����Ѵ�.
	isdfs = 1;
	isbfs = 0;
	isrfs = 0;
	start_flag = 0;
	end_flag = 0;
	return 0;
}

void ofApp::finddfs()
{
	stack<position> S; //���� S�� �����Ѵ�.
	position t;
	t.input(x, y); 
	S.push(t); //���� ��ġ�� S�� �ִ´�.

	while (!S.empty()) //S�� ����ٸ� �ݺ�����
	{
		if (Maze[end.ypos][end.xpos].visit_time != 0) //���� ������ �湮�ߴٸ�
			break; //Ž������

		x = S.top().xpos;
		y = S.top().ypos;
		S.pop(); //x,y�� S���� POP�� ������ �ٲ��ش�.

		Maze[y][x].visit_time++; //�ش� ��ġ�� �湮

		if (x - 1 >= 0) //�̷��� ������ ����� �ʰ�
			if (Maze[y][x-1].state == EMPTY && Maze[y][x-1].visit_time == 0) { //�ش� ������ �湮���� ����, ����ִٸ�
				x--;
				t.input(x, y);
				S.push(t);
				//�ش� ��ġ�� S�� push�Ѵ�.
				Maze[y][x].parent.ypos = y;
				Maze[y][x].parent.xpos = x + 1;
				//�ش� ��ġ�� �θ� ���� ��ġ�� �Ѵ�.
				x++;//������� �ǵ���
			}
		//�Ʒ� 3�� ��� ������ ���
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

bool ofApp::BFS() //BFS Ž�� �˰���
{
	initialize_maze(); //�ʱ�ȭ

	x = start.xpos;
	y = start.ypos; //��������
	findbfs(); //������ �����Ҷ����� BFSŽ��
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
	queue<position> Q; //Queue Q�� ����
	Q.push(t); //Q�� ������ġ ����

	while (!Q.empty()) //Q�� ��������� �ݺ�����
	{
		x = Q.front().xpos;
		y = Q.front().ypos;
		Q.pop(); //Q���� pop�� ���� x,y�� �ִ´�
		Maze[y][x].visit_time++; //���� ��ġ�� �湮
		if (Maze[end.ypos][end.xpos].visit_time != 0) //������ �湮�ߴٸ�
			break; //�˰��� ����

		if (y + 1 < HEIGHT) //�̷� ������ ����� �����鼭
			if (Maze[y + 1][x].state == EMPTY && Maze[y + 1][x].visit_time == 0) { //�ش� ������ �湮���� ����, ����ִٸ�
				y++;
				t.input(x, y);
				Q.push(t); //�ش� ��ġ�� Q�� push
				Maze[y][x].parent.ypos = y - 1;
				Maze[y][x].parent.xpos = x; //�ش� ��ġ�� �θ� ������ġ�� ����
				y--; //�������
			}
		//�Ʒ� 3�� ��� ������ ���
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
	initialize_maze(); //�ʱ�ȭ
	x = start.xpos;
	y = start.ypos; //������ ����
	findrfs(); //RFS Ž��
	//flag�� �˸°� ����
	isbfs = 0;
	isdfs = 0;
	isrfs = 1;
	start_flag = 0;
	end_flag = 0;
	cout << "Movement : " << Mov << endl; //�̵� Ƚ�� ���
	return 0;
}

void ofApp::findrfs()
{
	position t[3]; //���� �ֱٱ��� �̵��ߴ� ��ġ���� 3���� �����ϴ� �迭 ----- t[0]�� �ٷ� ��, t[1]�� 2�� ��, t[2]�� 3�� ��
	position temp[3]; //�ӽ÷� ��ġ�� �����ϴ� �迭
	t[0].input(-1, -1);
	t[1].input(-1, -1);
	t[2].input(-1, -1); //����ϴ� ��� �ʱ�ȭ
	Mov = -1; //�̵�Ƚ�� �ʱ�ȭ

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3); //���� ���� ����

	int dir_count; //�̵� ������ ���� ��
	int dir[4] = { 0,0,0,0 }; //�̵� ������ ����
	int k,i=0;
	int idx = 0;
	int tempx, tempy;
	while (1)
	{
		Mov++; //�̵�Ƚ��++
		Maze[y][x].visit_time++; //������ġ �湮
		Maze[y][x].parent.input(t[0].xpos, t[0].ypos); //������ġ�� �θ� ���� �ֱٿ� �湮�� t[0]���� ����

		if (y == end.ypos && x == end.xpos) //����ġ�� �����̶��
			return; //�˰��� ����

		dir_count = calcpath(dir); //�湮 ������ ���� Ȯ��
		dir_count = excludepath(dir_count, dir, t); //�湮 ������ ������ �ֱ�3ȸ �̳��� �湮�ߴ� ���� ����

		if (dir_count <= 0) //�ֱ�3ȸ �̳� �湮�ߴ� ������ �����ϸ�, �����ִ� ��ΰ� ���ٸ�
		{

			while (1)
			{
				for (int l = 0; l < 3; l++)
					temp[l].input(t[l].xpos, t[l].ypos); //�ӽ� �迭�� �ֱ�3ȸ �湮 ��ġ ����
				idx = 0;
				
				tempx = x;
				tempy = y;
				x = temp[idx].xpos;
				y = temp[idx].ypos;
				idx++; // �ٷ� ���� �湮�ߴ� ������ �̵�

				t[2].input(t[1].xpos, t[1].ypos);
				t[1].input(t[0].xpos, t[0].ypos);
				t[0].input(tempx, tempy); //�ֱ�3ȸ �湮��ġ �缳��

				dir_count = calcpath(dir); //�湮 ������ ���� Ȯ��
				dir_count = excludepath(dir_count, dir, t); //�湮 ������ ������ �ֱ�3ȸ �̳��� �湮�ߴ� ���� ����

				if (dir_count == 0) continue; //������ �湮������ ���� ���ٸ� �ݺ�
				else
				{
					while (1) {
						//�湮������ ������ ���� ����
						k = dis(gen);
						if (dir[k] == 1)
							break;
					}
					visitrfs(t, k); //�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
					break;
				}
			}
		}

		else
		{
			while (1) {
				//�湮 ������ ������ ���� ����
				k = dis(gen);
				if (dir[k] == 1)
					break;
			}
			visitrfs(t, k); //�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
			continue;
		}
	}
}

void ofApp::visitrfs(position* t, int k) { // RFS Ž���� �湮�ϸ� ��ġ�� �����ϴ� �Լ�

	//�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
	if (k == 0) {
		Maze[y - 1][x].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		y -= 2;
	}
	//�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
	else if (k == 1) {
		Maze[y][x + 1].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		x += 2;
	}
	//�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
	else if (k == 2) {
		Maze[y + 1][x].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		y += 2;
	}
	//�˸��� ���⿡ ���� �̷� �湮, �ֱ�3ȸ �湮��ġ �缳��, ��ǥ����
	else if (k == 3) {
		Maze[y][x - 1].visit_time++;
		t[2].input(t[1].xpos, t[1].ypos);
		t[1].input(t[0].xpos, t[0].ypos);
		t[0].input(x, y);
		x -= 2;
	}
}

int ofApp::calcpath(int* dir) //RFS Ž���� �̵� ������ ������ Ž���ϴ� �Լ�
{
	int dir_count = 0;
	if (y - 1 > 0) { //�̷� ������ ����� �ʰ�
		if (Maze[y - 1][x].state == EMPTY) { //�ش� ������ �̷ΰ� ����ִٸ�
			dir_count++; //�̵� ���� ������ �� ++
			dir[0] = 1; //���� �ִٰ� ǥ��( 1 = ��������, 0 = ����)
		}
		else
			dir[0] = 0; //���� ���ٰ� ǥ��( 1 = ��������, 0 = ����)
	}
	else
		dir[0] = 0; //���� ���ٰ� ǥ��( 1 = ��������, 0 = ����)
	//�Ʒ��� ��� ����
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

	return dir_count; //�̵� ������ ������ ���� ��ȯ
}

int ofApp::excludepath(int count, int* dir, position* temp) //RFSŽ����, �ֱ�3ȸ �̳��� ���ƴ� ��ġ�� ���� ������ �����ϴ� �Լ�
{
	if (dir[0] == 1) //���⿡ ����
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[j].xpos == x && temp[j].ypos == y - 2) { //�ֱ� 3ȸ �湮�� �����ϴ� temp[]�� ���ؼ�, ���ٸ�
				count--;
				dir[0] = 0; //�湮���ɿ��θ� 0����, ���� ���� ������ 1���δ�
				break;
			}
		}
	}
	//�Ʒ� 3�� ��� �����ϴ�
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

	return count; //���� ������ ���� ������ ��ȯ
}

void ofApp::pathdraw() //DFS, BFS�� �湮 ���� �� �ִܰ�θ� �׷��ִ� �Լ�
{
	//�湮�� ����� �׷��ֱ�
	int i, j;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) { //����, ���� ���� ��� ��ġ�� ���Ͽ�
			if (Maze[i][j].visit_time != 0) //�ش� ��ġ�� �̷θ� �湮������ �ִٸ�
				ofDrawRectangle(window_w, window_h, size, size); //�簢���� �׷��ش�
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
	//�ִܰ�� �׷��ֱ�
	y = end.ypos, x = end.xpos; //�������� ����
	int tempy, tempx;
	window_h = y * size;
	window_w = x * size; //������ �ش��ϴ� ��ġ�� ����
	ofSetColor(0, 0, 255); //���� ����
	ofDrawRectangle(window_w, window_h, size, size);  //������ �׷��ش�

	while (1)
	{
		if (Maze[y][x].parent.xpos == -1) break; //�θ� ������, �� ������ �� �ݺ�����
		tempy = Maze[y][x].parent.ypos;
		tempx = Maze[y][x].parent.xpos; 
		y = tempy;
		x = tempx; //�θ��� ��ġ�� ���� �ű��
		window_h = y * size;
		window_w = x * size; //���� �ش��ϴ� ��ġ�� ����
		ofDrawRectangle(window_w, window_h, size, size); //��θ� �׷��ش�
	}
}

void ofApp::rfsdraw() //RFS�湮��, �湮 Ƚ���� ���� ���� �ٸ��� �ؼ� �׷��ش�
{
	int i, j;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (Maze[i][j].visit_time > 0) //�湮�� ���� �ִٸ�
			{
				int k = 102 + Maze[i][j].visit_time * 10; //���� �湮�� ���ϼ��� ���� ������ ���� , �������� Ź�� ��������
				if (k > 255) k = 255; //out-of-range
				ofSetColor(k,0,0); //������
				ofDrawRectangle(window_w, window_h, size, size); //�׷��ֱ�
			}
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
}
