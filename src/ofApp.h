//  DRAWPOLYGONS_v02
//
//  Created by Anto+Creo on 06/01/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofHandle.h"


#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        vector <ofPoint> myPoints;
        ofPoint mousePos;
        ofColor colorPicked;
        ofRectangle colorPickerRect;

        float butRadius;
        float butAlpha;

    
        bool open, ableClick,bFullscreen,running, curved;
        float distance, xt,yt;
        float handlerRadius, colorSq;
        string curvedVal;
    
        ofImage img;
        vector <ofColor> selectedColor;
    
        ofFbo pickerFbo;
        ofFbo polyFbo;
    
    ///////MAIN FUNCTIONS////////
        void colorPicker();
        void polygons();
     
    
        void saveXml();
    
    //////////// draggable vertices ////////
    
    vector <draggableVertex> curveVertices;
    
    
    
    //child App
    void openChildApp ();
    
    
    ofxOscSender sender;

    
    
    
    
};
