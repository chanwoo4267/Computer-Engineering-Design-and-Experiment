/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS",false,false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		if (isOpen)
		{
			BFS();
			bShowInfo = bChecked;
		}

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	//input[][] : 1은 통로, 0은 벽 따라서 벽에 작은 사각형을 그리는방식으로 그려주기
	if (load_flag) {

		int window_h = 0;
		int window_w = 0;
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (input[i][j] == 0)
					ofDrawRectangle(window_w, window_h, size, size);
				window_w += size;
			}
			window_w = 0;
			window_h += size;
		}
	}


	if (isdfs)
	{
		ofSetColor(30,60,90);
		if (isOpen) {
			dfsdraw();
		}
			
		else
			cout << "You must open file first" << endl;
	}

	if (isbfs)
	{
		ofSetColor(30, 60, 90);
		if (isOpen) {
			bfsdraw();
		}

		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			freeMemory();
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	isbfs = 0;
	isdfs = 0;
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName.c_str());
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;
			int mazeidx = 0;

			// Read file line by line
			int cnt = 0;
			
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
			{
				string line = *it;
				WIDTH = line.size();
				input[idx] = (char*)malloc(sizeof(char) * WIDTH);
				for (mazeidx = 0; mazeidx < WIDTH; mazeidx++)
				{
					if (line[mazeidx] == '|' || line[mazeidx] == '+' || line[mazeidx] == '-') input[idx][mazeidx] = 0;
					else input[idx][mazeidx] = 1;
				}
				idx++;
			}
			HEIGHT = idx;
			load_flag = 1;

			visited = (int**)malloc(sizeof(int*) * HEIGHT);
			for (int k = 0; k < HEIGHT; k++)
				visited[k] = (int*)malloc(sizeof(int) * WIDTH);

			path = (bfspath**)malloc(sizeof(bfspath*) * HEIGHT);
			for (int k = 0; k < HEIGHT; k++)
				path[k] = (bfspath*)malloc(sizeof(bfspath) * WIDTH);

			bfs_path = (bfspath**)malloc(sizeof(bfspath*) * HEIGHT);
			for (int k = 0; k < HEIGHT; k++)
				bfs_path[k] = (bfspath*)malloc(sizeof(bfspath) * WIDTH);

			size = 20;
			while (1)
			{
				if (HEIGHT * size <= 480 && WIDTH * size <= 640) break;
				size--;
				if (size == 1) break;
			}

		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {
	for (int i = 0; i < HEIGHT; i++)
		free(input[i]);
	printf("Allocated Memory ' input ' has been freed.\n");
	for (int i = 0; i < HEIGHT; i++)
		free(visited[i]);
	free(visited);
	printf("Allocated Memory ' visited ' has been freed.\n");
	for (int i = 0; i < HEIGHT; i++)
			free(path[i]);
	free(path);
	printf("Allocated Memory ' path ' has been freed.");
	for (int i = 0; i < HEIGHT; i++)
		free(bfs_path[i]);
	free(bfs_path);
	printf("Allocated Memory ' bfs_path ' has been freed.");
	//TO DO
	// malloc한 memory를 free해주는 함수
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			visited[i][j] = 1;

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			path[i][j].visit = 1;
			path[i][j].parentx = -1;
			path[i][j].parenty = -1;
		}
	
	maze_col = 1;
	maze_row = 1;
	finddfs(maze_row, maze_col, HEIGHT, WIDTH);
	isdfs = 1;
	isbfs = 0;

	return 0;
}

bool ofApp::BFS()
{
	int i, j;
	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			visited[i][j] = 1;

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			bfs_path[i][j].visit = 1;
			bfs_path[i][j].parentx = -1;
			bfs_path[i][j].parenty = -1;
		}
	maze_col = 1;
	maze_row = 1;

	findbfs(maze_row, maze_col, HEIGHT, WIDTH);
	isbfs = 1;
	isdfs = 0;

	return 0;
}

void ofApp::finddfs(int maze_row, int maze_col, int HEIGHT, int WIDTH)
{
	int i, j;
	int x, y;
	stack<int> S;

	visited[maze_row][maze_col] = 0;
	path[maze_row][maze_col].visit = 0;
	S.push(maze_row);
	S.push(maze_col);
	while (!S.empty())
	{
		maze_col = S.top();
		S.pop();
		maze_row = S.top();
		S.pop();
		visited[maze_row][maze_col] = 0;

		if (visited[HEIGHT - 2][WIDTH - 2] == 0)
			break;

		if (maze_col - 1 >= 0)
			if (input[maze_row][maze_col - 1] == 1 && visited[maze_row][maze_col - 1] == 1) {
				maze_col--;
				S.push(maze_row);
				S.push(maze_col);
				
				path[maze_row][maze_col].visit = 0;
				path[maze_row][maze_col].parenty = maze_row;
				path[maze_row][maze_col].parentx = maze_col + 1;
				maze_col++;
			}

		if (maze_row - 1 >= 0)
			if (input[maze_row - 1][maze_col] == 1 && visited[maze_row - 1][maze_col] == 1) {
				maze_row--;
				S.push(maze_row);
				S.push(maze_col);

				path[maze_row][maze_col].visit = 0;
				path[maze_row][maze_col].parenty = maze_row + 1;
				path[maze_row][maze_col].parentx = maze_col;
				maze_row++;
			}

		if (maze_row + 1 < HEIGHT)
			if (input[maze_row + 1][maze_col] == 1 && visited[maze_row + 1][maze_col] == 1) {
				maze_row++;
				S.push(maze_row);
				S.push(maze_col);
		
				path[maze_row][maze_col].visit = 0;
				path[maze_row][maze_col].parenty = maze_row - 1;
				path[maze_row][maze_col].parentx = maze_col;
				maze_row--;
			}

		if (maze_col + 1 < WIDTH)
			if (input[maze_row][maze_col + 1] == 1 && visited[maze_row][maze_col + 1] == 1) {
				maze_col++;
				S.push(maze_row);
				S.push(maze_col);
				
				path[maze_row][maze_col].visit = 0;
				path[maze_row][maze_col].parenty = maze_row;
				path[maze_row][maze_col].parentx = maze_col - 1;
				maze_col--;
			}

		
	}


	/*if (maze_row < 0 || maze_col < 0 || maze_row >= HEIGHT || maze_col >= WIDTH) //outofrange
		return;
	
	visited[maze_row][maze_col] = 0; //1 방문안함, 0 방문함
	path[maze_row][maze_col].visit = 0;
	
	if (visited[HEIGHT-2][WIDTH-2] == 0)
		return;

	if (maze_row + 1 < HEIGHT)
		if (input[maze_row + 1][maze_col] == 1 && visited[maze_row + 1][maze_col] == 1)
			finddfs(maze_row + 1, maze_col, HEIGHT, WIDTH);

	if (visited[HEIGHT - 2][WIDTH - 2] == 0)
		return;

	if (maze_col + 1 < WIDTH)
		if (input[maze_row][maze_col + 1] == 1 && visited[maze_row][maze_col + 1] == 1)
			finddfs(maze_row, maze_col + 1, HEIGHT, WIDTH);

	if (visited[HEIGHT - 2][WIDTH - 2] == 0)
			return;

	if (maze_col - 1 >= 0)
		if (input[maze_row][maze_col-1] == 1 && visited[maze_row][maze_col-1] == 1)
			finddfs(maze_row, maze_col-1, HEIGHT, WIDTH);

	if (visited[HEIGHT - 2][WIDTH - 2] == 0)
		return;

	if (maze_row - 1 >= 0)
		if (input[maze_row-1][maze_col] == 1 && visited[maze_row-1][maze_col] == 1)
			finddfs(maze_row-1, maze_col, HEIGHT, WIDTH);

	if (visited[HEIGHT - 2][WIDTH - 2] == 0)
		return;

	path[maze_row][maze_col] = 1;
	*/ //recursive
}

void ofApp::findbfs(int maze_row, int maze_col, int HEIGHT, int WIDTH)
{
	int i, j;
	int x, y;
	
	//1 방문안함, 0 방문함

	visited[maze_row][maze_col] = 0;
	bfs_path[maze_row][maze_col].visit = 0;

	queue<int> Q;
	Q.push(maze_row);
	Q.push(maze_col);

	while (!Q.empty())
	{
		maze_row = Q.front();
		Q.pop();
		maze_col = Q.front();
		Q.pop();

		if (visited[HEIGHT - 2][WIDTH - 2] == 0)
			break;

		if (maze_row + 1 < HEIGHT)
			if (input[maze_row + 1][maze_col] == 1 && visited[maze_row + 1][maze_col] == 1) {
				maze_row++;
				Q.push(maze_row);
				Q.push(maze_col);
				visited[maze_row][maze_col] = 0;
				bfs_path[maze_row][maze_col].visit = 0;
				bfs_path[maze_row][maze_col].parenty = maze_row-1;
				bfs_path[maze_row][maze_col].parentx = maze_col;
				maze_row--;
			}

		if (maze_col + 1 < WIDTH)
			if (input[maze_row][maze_col + 1] == 1 && visited[maze_row][maze_col + 1] == 1) {
				maze_col++;
				Q.push(maze_row);
				Q.push(maze_col);
				visited[maze_row][maze_col] = 0;
				bfs_path[maze_row][maze_col].visit = 0;
				bfs_path[maze_row][maze_col].parenty = maze_row;
				bfs_path[maze_row][maze_col].parentx = maze_col-1;
				maze_col--;
			}

		if (maze_col - 1 >= 0)
			if (input[maze_row][maze_col - 1] == 1 && visited[maze_row][maze_col - 1] == 1) {
				maze_col--;
				Q.push(maze_row);
				Q.push(maze_col);
				visited[maze_row][maze_col] = 0;
				bfs_path[maze_row][maze_col].visit = 0;
				bfs_path[maze_row][maze_col].parenty = maze_row;
				bfs_path[maze_row][maze_col].parentx = maze_col+1;
				maze_col++;
			}

		if (maze_row - 1 >= 0)
			if (input[maze_row - 1][maze_col] == 1 && visited[maze_row - 1][maze_col] == 1) {
				maze_row--;
				Q.push(maze_row);
				Q.push(maze_col);
				visited[maze_row][maze_col] = 0;
				bfs_path[maze_row][maze_col].visit = 0;
				bfs_path[maze_row][maze_col].parenty = maze_row+1;
				bfs_path[maze_row][maze_col].parentx = maze_col;
				maze_row++;
			}
	}

}

void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)


	int i, j;
	int window_h = 0;
	int window_w = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (visited[i][j] == 0)
				ofDrawRectangle(window_w, window_h, size, size);
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
	
	int y = HEIGHT - 2, x = WIDTH - 2;
	int tempy, tempx;
	window_h = y * size;
	window_w = x * size;
	ofSetColor(200, 255, 200);
	ofDrawRectangle(window_w, window_h, size, size);  //draw start point : end of maze

	while (1)
	{
		if (path[y][x].parentx == -1) break;
		tempy = path[y][x].parenty;
		tempx = path[y][x].parentx;
		y = tempy;
		x = tempx;
		window_h = y * size;
		window_w = x * size;
		ofDrawRectangle(window_w, window_h, size, size);
	}
	
	/*
	window_h = 0;
	window_w = 0;
	ofSetColor(200, 255, 200);
	
	
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (path[i][j] == 0)
				ofDrawRectangle(window_w, window_h, 5, 5);
			window_w += 5;
		}
		window_w = 0;
		window_h += 5;
	}
	*/ //dfs recursive
}

void ofApp::bfsdraw()
{
	int i, j;
	int window_h = 0;
	int window_w = 0;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (visited[i][j] == 0)
				ofDrawRectangle(window_w, window_h, size, size);
			window_w += size;
		}
		window_w = 0;
		window_h += size;
	}
	int y = HEIGHT -2, x = WIDTH - 2;
	int tempy, tempx;
	window_h = y * size;
	window_w = x * size;
	ofSetColor(200, 255, 200);
	ofDrawRectangle(window_w, window_h, size, size);  //draw start point : end of maze

	while (1)
	{
		if (bfs_path[y][x].parentx == -1) break;
		tempy = bfs_path[y][x].parenty;
		tempx = bfs_path[y][x].parentx;
		y = tempy;
		x = tempx;
		window_h = y * size;
		window_w = x * size;
		ofDrawRectangle(window_w, window_h, size, size);
	}
	
}

