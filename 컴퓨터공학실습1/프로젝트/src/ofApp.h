
#pragma once

#include "ofMain.h"
#include <cstring> //���ڿ� ó�� ���� �������
#include <cstdlib> //���ڿ� ó�� ���� �������
#include <queue> //QUEUE STL
#include <stack> //STACK STL 
#include <random> //������ �̱� ���� �������

#define EMPTY 1 //�̷��� ��
#define WALL 0 //�̷��� ���,��

class position {  //��ġ�� ��Ÿ���� ����
public:
	int xpos; //x��ǥ
	int ypos; //y��ǥ
	void input(int, int); //x,y�� �ѹ��� �ִ� ���
};

class maze { //�̷θ� �����ϴ� �ڷᱸ��
public:
	bool state; //������ǥ�� �̷ΰ� ������ ��������
	position parent; //�ִܰ�θ� ã���� �̿��ϴ� ����, �ڽ��� �θ� �� ��ǥ
	int visit_time; //�湮���θ� Ȯ��. 0�̸� �湮��������, 1 �̻��̸� �湮��. int������ ������ RFS�� �湮Ƚ���� ���� ���� �ٸ��� �׸��⶧��
}; 

class ofApp : public ofBaseApp {

	public:

		void setup(); //���α׷� ���۽� ����Ǵ±⺻�Լ�
		void update(); //���������� ����Ǵ±⺻�Լ�
		void draw(); //���������� ����Ǵ� �⺻�Լ�2, �ַ� �׸��� ���ñ�ɸ� �־���

		void keyPressed(int key); //Ű �Է¿� ���� �⺻�Լ�
		void keyReleased(int key); //Ű �Է��� ������ ���� �⺻�Լ�

		bool readFile(); //�̷� ����(.maz)�� �Է¹�����, �̷θ� �����Ѵ�.
		bool readHW(); //�̷��� ���̿� ���� �Է¹�����(txt����), �̷θ� ������ �����Ѵ�.
		void freeMemory(); //�����Ҵ�� �޸� ����
		bool DFS(); //DFS�� ���� �̷� Ż��
		bool BFS(); //BFS�� ���� �̷� Ż��
		bool RFS(); //RFS�� ���� �̷� Ż��
		void findbfs(); //DFS�� ����Լ�, ������ ���
		void finddfs(); //BFS�� ����Լ�, ������ ���
		void findrfs(); //RFS�� ����Լ�, ������ ���
		void drawmaze(); //�̷θ� �׷��ִ� �Լ�
		void pathdraw(); //DFS, BFS�� Ž�� ��θ� �׷���. �ִܰ�δ� �ٸ������� ǥ��
		void rfsdraw(); //RFS�� Ž�� ��θ� �׷���. Ž�� Ƚ���� ���� ����ϼ��� Ź�ѻ�, �������� ������ ������

		void initialize_maze(); //�̷θ� �ʱ�ȭ�ϴ� �Լ�
		void MakeMaze(); //readHW�� �Է¹��� �����ͷ� �̷θ� �����ϴ� �Լ�
		int calcdir(int* dir); //������ �̷��� ���� ������ �ִ°� Ž��
		int calcdirr(int* dir); //������ �̷��� ���� �շ��ִ� �� Ž��
		int calcpath(int* dir); //������ �̷��� ����� �湮 ������ �� Ž��
		int excludepath(int count, int* dir, position* temp); //RFS�� �湮 ������ ���� �ֱٿ� �鷶�� ��θ� �����ϴ��Լ�
		int rand(int a, int b); //�̷� ���� �˰��򿡼� ������ ��ġ�� �����ϰ� �����ִ� �Լ� 
		void visitrfs(position* t, int k); // RFS Ž���� �湮�ϸ� ��ġ�� �����ϴ� �Լ�

		int HEIGHT;//�̷��� ����
		int WIDTH;//�̷��� �ʺ�
		int Mov; //RFS�� �� �̵�Ƚ��

		maze** Maze; //�̷��� ������ �����ϴ� ����
		
		int x;//�̷�ĭ�� ���� �ε����� ����Ų��.
		int y;//�̷�ĭ�� ���� �ε����� ����Ų��.

		position start; //������
		position end; //����

		int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�. 2�̸� h,w���� txt������ ������.
		int start_flag; // �������� �����̴��� Ȯ���ϴ� �÷���
		int end_flag; // ������ �����̴��� Ȯ���ϴ� �÷���
		int size=20; //�̷��� ũ��

		// Used by example app
		ofTrueTypeFont myFont; //��Ʈ �Է¹޴� ����
		float windowWidth, windowHeight; //������ â ũ�⸦ �����ϴ� ����

		bool isdfs; //dfs flag
		bool isbfs; //bfs flag
		bool isrfs; //custom rfs flag
};
