#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15);
    
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
    dot_now = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);
    ofDrawRectangle(0, 0, 1024, 40);
    ofDrawRectangle(0, 728, 1024, 40);
    ofSetLineWidth(5);
    
    if( draw_flag){
        int i;
        if (num_of_line > 0)
            for( i=0 ; i< num_of_line ; i++){ //draw lines
                ofDrawLine(lineseg[i].x1, lineseg[i].y1, lineseg[i].x2, lineseg[i].y2);
            }
        if (num_of_dot > 0)
            for( i=0 ; i< num_of_dot ; i++){ // draw circles
                if( dot_now == i)
                    ofSetColor(255, 0, 0);
                else
                    ofSetColor(0);
                
                ofDrawCircle(dot[i].x1,dot[i].y1, dot_diameter);
            }
    }
    ofSetLineWidth(2);
}

//--------------------------------------------------------------x
void ofApp::keyPressed(int key){
    if (key == 'v') {

        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if( key == 'q'){
        draw_flag = 0;
        
        if( lineseg != NULL){
            free(lineseg);
            lineseg = NULL;
        }
        if( dot != NULL){
            free(dot);
            dot = NULL;
        }
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( load_flag != 0 ) {
            draw_flag = 1;
            if (dot != NULL) {
            maindot.x = dot[dot_now].x1;
            maindot.y = dot[dot_now].y1;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    if( key == OF_KEY_RIGHT ){
        dot_now++;
        if(dot_now >= num_of_dot)
            dot_now = 0;
        
        maindot.x = dot[dot_now].x1;
        maindot.y = dot[dot_now].y1;
        
        cout << "Selcted Dot Coordinate is (" << maindot.x << ", " << maindot.y << ")" << endl;
    }
    if( key == OF_KEY_LEFT){
        dot_now--;
        if (dot_now <= -1)
            dot_now = num_of_dot-1;
        
        maindot.x = dot[dot_now].x1;
        maindot.y = dot[dot_now].y1;
        
        cout << "Selcted Dot Coordinate is (" << maindot.x << ", " << maindot.y << ")" << endl;
    }
   
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    string fileName = openFileResult.getName();
    
    ofFile file(fileName);
    
    if( !file.exists())
        cout << "Target file does not exists." << endl;
    
    else
        cout << "We found the target file." << endl;
    
    
    ofBuffer buffer(file);
    int input_type=0;
    
    int idx=0;
    
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
       
        vector<string> words = ofSplitString(line, " ");
        
        if(words.size() == 1){
            if(input_type==0){
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                lineseg = (LineSegment*)malloc(sizeof(LineSegment) * num_of_line);
            }
            else{
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
                dot = (Dot*)malloc(sizeof(Dot) * num_of_dot);
            }
        }
        else if (words.size() >= 2){ //custom all
            int x1,y1,x2,y2;
            if(input_type==0){
                if (num_of_line > 0) {
                    
                    x1 = atoi(words[0].c_str());
                    y1 = atoi(words[1].c_str());
                    x2 = atoi(words[2].c_str());
                    y2 = atoi(words[3].c_str());
                    
                    lineseg[idx].x1 = x1;
                    lineseg[idx].y1 = y1;
                    lineseg[idx].x2 = x2;
                    lineseg[idx].y2 = y2;
                    
                    if( x1 < 0 || x1 > ofGetWidth()){
                        cout << "Out-of-range: " << x1 << endl;
                        return;
                    }
                    else if ( y1 < 0 || y1 > ofGetHeight()){
                        cout << "Out-of-range: " << y1 << endl;
                        return;
                    }
                    else if ( x2 < 0 || x2 > ofGetWidth()){
                        cout << "Out-of-range: " << x2 << endl;
                        return;
                    }
                    else if ( y2 < 0 || y2 > ofGetHeight()){
                        cout << "Out-of-range: " << y2 << endl;
                        return;
                    }
                
                    idx++;
                
                    if( idx >= num_of_line){
                        input_type = 1;
                        idx = 0;
                    }
                }
            }
            else{
                if (num_of_dot > 0) {
                    x1 = atoi(words[0].c_str());
                    y1 = atoi(words[1].c_str());
                    dot[idx].x1 = x1;
                    dot[idx].y1 = y1;
                    
                    if( x1 < 0 || x1 > ofGetWidth()){
                        cout << "Out-of-range: " << x1 << endl;
                        return;
                    }
                    else if ( y1 < 0 || y1 > ofGetHeight()){
                        cout << "Out-of-range: " << y1 << endl;
                        return;
                    }
                   
                    idx++;
                    
                    if( idx >= num_of_dot)
                        input_type = 0;
                }
            }
        }
    }
    
    //initializeWaterLines();
}

void ofApp::initializeWaterLines(){
    
}
