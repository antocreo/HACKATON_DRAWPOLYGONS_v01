//  DRAWPOLYGONS_v02
//
//  Created by Anto+Creo on 06/01/2015.
//
//

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    sender.setup(HOST, PORT);

    
    ofSetFullscreen(true);  // set fullscreen
    
    open=true;              // set the polygon true at the beginning
    running=true;
    ableClick=true;
//    ofSetBackgroundAuto(false);
    ofBackground(15);      //background white
    handlerRadius = 5;     // radius of the handler of the polygon
    
    
    ///////////COLOR PICKER////////
    //setting up the color picker
    
    float w = ofGetWidth()/6;
    float h = ofGetHeight()/6;
    float cx = w/2;
    float cy = h/2;
    
  
    img.allocate(w,h,OF_IMAGE_COLOR);
    
    for (float y=0; y<h; y++) {
        for (float x=0; x<w; x++) {
            
            float angle = atan2(y-cy,x-cy)+PI;
            float dist = ofDist(x,y,cx,cy);
            float hue = angle/TWO_PI*255;
            float sat = ofMap(dist,0,w/4,0,255,true);
            float bri = ofMap(dist,w/4,w/2,255,0,true);
            
            img.setColor(x,y,ofColor::fromHsb(hue,sat,bri));
            
        }
    }
    
    img.reloadTexture();
    
    ofPoint mousePos(mouseX,mouseY);
    
    
    colorPickerRect.set((ofGetWidth()-ofGetWidth()/6),(ofGetHeight()-ofGetHeight()/6), ofGetWidth(), ofGetHeight());
    


    
//////////////////////////////
    
    curvedVal= "Curved";
    
    
    ////////////////// fbo color picker /////////////////
    
//        pickerFbo.allocate(w, h,GL_RGB);
//    
//        pickerFbo.begin();
//        ofClear(255,255,255);
//        pickerFbo.end();
    
    
    ofSetRectMode(OF_RECTMODE_CORNER);
//    img.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight()-ofGetHeight()/6);
    
    if (colorPickerRect.inside(mouseX, mouseY)) {
        
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255,255,255,200);
        ofRect(mouseX+10, mouseY, 30, 30);
        
        
        colorPicked.set(img.getColor(mouseX-(ofGetWidth()-ofGetWidth()/6), mouseY-(ofGetHeight()-ofGetHeight()/6)));
        ofSetColor(colorPicked);
        
        ofRect(mouseX+10, mouseY, 20, 20);
        ofFill();
        ofSetColor(255);
        ofPushMatrix();
        
        ofDrawBitmapString(ofToString( colorPicked), 50,500);
    }

    ////////////////// end fbo color picker /////////////////

    
 


}

//--------------------------------------------------------------
void ofApp::update(){
    img.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    

    if (selectedColor.size()==0) {
        
        colorPicker();
    }
   

    //---------POLYGONS---------------------

    if (selectedColor.size()>0) {
        polygons();

    }
    
    
    //---------COLOR PICKER---------------------
    
//    pickerFbo.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight() - ofGetHeight()/6);
    
    if (colorPickerRect.inside(mouseX, mouseY)) {
        colorPicker();
        ableClick=false;
        
    }
  

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ///close path with spacebar
    if(key==' '){
        if(ofFill){
            ofNoFill();
        }else{
            ofFill;
        }
    }
    
    
    ///DELETE LAST POINT
    
    if(key=='b' && curveVertices.size()>0){
        open=true;
        curveVertices.pop_back();
        myPoints.pop_back();
        ableClick=true;
        saveXml(); //saving Xml file

    }
    
    //DELETE ALL
    
    if(key==OF_KEY_BACKSPACE){
        
        curveVertices.clear();
        myPoints.clear();
        distance=0;

        open=true;
        ableClick=true;
        saveXml(); //saving Xml file

    }
  
    ///MAKE IT CURVE
    
    if(key=='c' && curveVertices.size()>0){
        if (curved) {
            curved=false;
            curvedVal= "Curved";


        } else {
            curved=true;
            curvedVal= "Straight";


        }
            
        saveXml(); //saving Xml file
    }
    
    ///FULLSCREEN
    
    if(key=='f'){
        ofToggleFullscreen();  // set fullscreen
    }
        
     ///TEST RANDOM
    
    if(key=='k'){
        
        ofColor randomColor(ofRandom(255), ofRandom(255),ofRandom(255));
        selectedColor.clear();
        selectedColor.push_back(randomColor);
        
        if (curved) {
            
            for (int i = 0; i < curveVertices.size(); i++){
                
                curveVertices[i].x=ofRandom(ofGetWidth());
                curveVertices[i].y=ofRandom(ofGetHeight());
            
            }
            
        } else {
            for (int i = 0; i < curveVertices.size(); i++){

            curveVertices[i].x=mouseX+ofRandom(ofGetWidth()/2);
            curveVertices[i].y=mouseY+ofRandom(ofGetHeight()/2);
            }
            
        }
        
        saveXml(); //saving Xml file
        
        
    }
    

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    //draggable vertices
    
    for (int i = 0; i < curveVertices.size(); i++){
        float diffx = x - curveVertices[i].x;
        float diffy = y - curveVertices[i].y;
        float dist = sqrt(diffx*diffx + diffy*diffy);
        if (dist < curveVertices[i].radius){
            curveVertices[i].bOver = true;
        } else {
            curveVertices[i].bOver = false;
            
            saveXml(); //saving Xml file

        }
        
    }

    if (colorPickerRect.inside(mouseX, mouseX)) {
    
        img.reloadTexture();
    
    }

    
    
////    //osc
//    ofxOscMessage m;
//    m.setAddress("/mouse/position");
//    m.addIntArg(x);
//    m.addIntArg(y);
//    sender.sendMessage(m);


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    //draggable vertices

    
    for (int i = 0; i < curveVertices.size(); i++){
        if (curveVertices[i].bBeingDragged == true){
            curveVertices[i].x = x;
            curveVertices[i].y = y;
            saveXml(); //saving Xml file

        }
    }


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    

    
    if (colorPickerRect.inside(mouseX, mouseX) && selectedColor.size()>0) {
        
            selectedColor.clear();
            selectedColor.push_back(colorPicked);
        

    }
    
    
    
    ofColor newColor(int r, int g,int b, int a);
    selectedColor.push_back(colorPicked);
    
//    img.clear();
    
  
    
    // if the size of the vector color is > 1 then start drawing the shape
    

    
        if (selectedColor.size()>1 && ableClick==true) {
            
            printf("V %lu \n",myPoints.size() );
            printf("curvevertices %lu \n",curveVertices.size() );
            
            draggableVertex newDraggableVertex(x,y,0);
            curveVertices.push_back(newDraggableVertex);
            
            ofPoint newPoint(x,y,0);
            myPoints.push_back(newPoint);
            
//            saveXml(); //saving Xml file


            }
    
    
    //draggable vertices
    
    for (int i = 0; i < curveVertices.size(); i++){
        float diffx = x - curveVertices[i].x;
        float diffy = y - curveVertices[i].y;
        float dist = sqrt(diffx*diffx + diffy*diffy);
        
        
        if (curveVertices[i].bOver==true) {
            ofSetColor(0);
            ofDrawBitmapString(ofToString(curveVertices[i].bOver), 30,100);
            ableClick=false;

        }
        
        if (dist < curveVertices[i].radius){
            curveVertices[i].bBeingDragged = true;
            saveXml(); //saving Xml file

            
        } else {
            curveVertices[i].bBeingDragged = false;
        }
     
    }
    

    

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    //draggable vertices

    for (int i = 0; i < curveVertices.size(); i++){
        curveVertices[i].bBeingDragged = false;
        saveXml(); //saving Xml file

    }

    
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



//---------COLOR PICKER-----------------------------------------

void ofApp::colorPicker(){
    
    if (colorPickerRect.inside(mouseX, mouseY)) {

    ofSetRectMode(OF_RECTMODE_CORNER);
    img.draw(ofGetWidth()-ofGetWidth()/6,ofGetHeight()-ofGetHeight()/6);
    
   
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255,255,255,200);
    ofRect(mouseX+10, mouseY, 30, 30);
    
    
    colorPicked.set(img.getColor(mouseX-(ofGetWidth()-ofGetWidth()/6), mouseY-(ofGetHeight()-ofGetHeight()/6)));
    ofSetColor(colorPicked);

    ofRect(mouseX+10, mouseY, 20, 20);
    ofFill();
    ofSetColor(255);
    ofPushMatrix();
    
//    ofDrawBitmapString(ofToString( colorPicked), 50,500);

    }
    

}


//---------POLYGONS-----------------------------------------

void ofApp::polygons(){
//////////////////////////////LAYOUT//////////////////////////////////

//    ofSetColor(0);
    ////instructions
//    ofDrawBitmapString("open "+ ofToString(open), 30,100);
//    ofDrawBitmapString("click " + ofToString(ableClick), 30,120);

   /*
    //rectangle base
    ofSetColor(0, 0, 0,20);
    ofRect(150, ofGetHeight(), 250, 280);
    //rectangle title
    ofSetColor(200, 0, 0,200);
    ofRect(150, ofGetHeight()-155, 250, 30);
    
    ofSetColor(250);
    ofDrawBitmapString("INSTRUCTIONS", 30,ofGetHeight()-150);
    
    ofSetColor(0);
    ofDrawBitmapString("now draw a shape", 30,ofGetHeight()-120);
    ofDrawBitmapString("click anywhere", 30,ofGetHeight()-100);
    ofDrawBitmapString("num of Points " + ofToString(myPoints.size()), 30,ofGetHeight()-80);
    ofDrawBitmapString("f: toggle Fullscreen", 30,ofGetHeight()-60);
    ofDrawBitmapString("b: delete last line", 30,ofGetHeight()-40);
    ofDrawBitmapString("backspace: delete everything", 30,ofGetHeight()-20);
    
    */
    
  
///////////////////////////////////////////////////////////////////////////////////
    


    // if drawing the lines the polygon is open
    if(open){
        ableClick=true;        // if the polygone it's open activate the click function
        for(int k=0;k<curveVertices.size();k++){
            distance= ofDist(curveVertices[k].x, curveVertices[k].y, curveVertices[0].x, curveVertices[0].y);
            
            if(k>=0){
                ofSetColor(selectedColor[0]);
                ofSetLineWidth(3);
                ofNoFill();
                ofBeginShape();
                //shape
                for (int i = 0; i < curveVertices.size(); i++){
                    ofVertex(curveVertices[i].x, curveVertices[i].y);
                }
                ofEndShape(false);
                //handle circle
                for (int i = 0; i < curveVertices.size(); i++){
                    if (curveVertices[i].bOver == true) ofFill();
                    else ofNoFill();
                    ofCircle(curveVertices[i].x, curveVertices[i].y,4);
                }
            }
            
            

            //checking the distance mousePos and first point
            if (   mouseX - curveVertices[0].x<=handlerRadius*5
                && mouseY - curveVertices[0].y<=handlerRadius*5
                && curveVertices[0].x - mouseX<=handlerRadius*5
                && curveVertices[0].y - mouseY<=handlerRadius*5
                ) {
                ofSetColor(0);
                string mouseMessage;
                
                //CLOSE MESSAGE
//                mouseMessage = "close the pattern";
//                if (curveVertices.size()>1) {
//                    ofDrawBitmapString(mouseMessage, mouseX-mouseMessage.length()*10,mouseY+10);
//                    
//                }

                // handle blink
                for (int i=1; i<ofGetFrameNum()%40; i++) {
                    ofNoFill();
                    ofSetColor(selectedColor[0],200/i+10);
                    ofSetLineWidth(5/i);
                    ofCircle(curveVertices[0].x,curveVertices[0].y,(i*(i+1)/100));
                
                        }
                    }
      
                }
//
    }
        if (distance>0 && distance<=handlerRadius*2) {
    
//        ofSetColor(selectedColor[0]);
//        ofDrawBitmapString("Press C to make " + curvedVal + " shape", 30,ofGetHeight()/2);
        
            //handles
            for (int i = 0; i < curveVertices.size(); i++){
                if (curveVertices[i].bOver == true) ofFill();
                else ofNoFill();
                ofCircle(curveVertices[i].x, curveVertices[i].y,5);
            }

        ////////THIS IS THE OLD BEGIN SHAPE

        ofBeginShape();
        for(int i=0;i<curveVertices.size();i++){

            ofSetColor(selectedColor[0]);
            ofFill();
          
            //curved shape
            if (curved) {
                
                if (i == 0){
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
                } else if (i == curveVertices.size()-1){
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
                    ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
                } else {
                    ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
                }
                


            }else{
                
            //straight shape
            ofVertex(curveVertices[i].x,curveVertices[i].y);
            curved=false;
            }

        }
            
        ofEndShape();
            open=false;
            ableClick=false;        // if the polygone it's closed deactivate the click function

        
           
            
            
        ///END/////
        
        
    }


}





//---------XML---------------------

void ofApp::saveXml(){
    
    //////////////////XML//////////////
    ofxXmlSettings positions;
    
    ofxOscMessage m;
    
    ///color
    positions.addTag("color");
    positions.pushTag("color");

    positions.addValue("R", selectedColor[0].r);
    positions.addValue("G", selectedColor[0].g);
    positions.addValue("B", selectedColor[0].b);
    
    //osc
    m.setAddress("/color");

    m.addIntArg(selectedColor[0].r);
    m.addIntArg(selectedColor[0].g);
    m.addIntArg(selectedColor[0].b);
    sender.sendMessage(m);



    positions.popTag();//pop color
    ///
    
    ///curved
    positions.addTag("curved");
    positions.pushTag("curved");
    positions.addValue("value", curved);
    positions.popTag();//pop curved
    ///
    
    ///point coord
    positions.addTag("positions");
    positions.pushTag("positions");
    //positions is a vector<draggableVerteces> that we want to save to a file
    for(int i = 0; i < curveVertices.size(); i++){
        //each position tag represents one point
        positions.addTag("position");
        positions.pushTag("position",i);
        //so set the three values in the file
        positions.addValue("X", curveVertices[i].x);
        positions.addValue("Y", curveVertices[i].y);
        positions.addValue("Z", curveVertices[i].z);

//        //osc
        m.setAddress("/coordinate");

        m.addIntArg(curveVertices[i].x);
        m.addIntArg(curveVertices[i].y);
        sender.sendMessage(m);
        positions.popTag();//pop position
    }
    positions.popTag(); //pop position
    positions.saveFile("positions.xml");
    
    //////////////////XML//////////////
    
    ///OSC message
    
    
    
}












