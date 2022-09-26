#pragma once

#include "ofMain.h"

#include <cstdlib>

class Dot{
public:
    int x1,y1;
};

class LineSegment{
public:
    int x1,y1,x2,y2;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        LineSegment* lineseg;
        Dot* dot;
    
        int num_of_line;
        int num_of_dot;
        int draw_flag;
        int load_flag;
        
        int dot_now;
    
        string originalFileExtension;
    
        ofPoint maindot;
        float water_radius;
        float dot_diameter;
    

        void processOpenFileSelection(ofFileDialogResult openFileResult);
        void initializeWaterLines();

};

