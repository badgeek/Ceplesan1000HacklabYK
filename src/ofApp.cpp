//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

#define HOST "localhost"
#define PORT 12345

void ofApp::setup()
{
    sender.setup(HOST, PORT);
    jentikcam.setup();

    gui.setHeaderBackgroundColor(ofColor::black);
    gui.setBackgroundColor(ofColor::black);

    ofAddListener(gui.savePressedE, this, &ofApp::guiSave);
    ofAddListener(gui.loadPressedE, this, &ofApp::guiLoad);

    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    gui.setup(jentikcam.parameters);
    gui.add(btntest.setup("DESTROY"));

    sync.setup(jentikcam.parameters, 8888, "127.0.0.1", 9999);

    scene_jentik_assets.mask.load("jentik_mask.png");

}

void ofApp::guiSave(){
    gui.saveToFile("settings.xml");
}


void ofApp::guiLoad(){
    gui.loadFromFile("settings.xml");
}


void ofApp::update()
{
    sync.update();
    jentikcam.update();
}


void ofApp::draw()
{
    ofBackground(0);
    jentikcam.draw();
    scene_jentik_assets.mask.draw(0,0);
    gui.draw();
}

void ofApp::keyPressed(int key)
{
//    auto eye = grabber.getGrabber<ofxPS3EyeGrabber>();
    
    switch (key){
        case 'c':
            jentikcam.learnBackground();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
//        case '+':
//            threshold ++;
//            if (threshold > 255) threshold = 255;
//            break;
//        case '-':
//            threshold --;
//            if (threshold < 0) threshold = 0;
//            break;
    }


//    if (key == ' ')
//    {
////        if (isRecording) currentFrame = 0;
////        else frameBuffer.clear();
        
////        isRecording = !isRecording;
//    }
//    else if (key == 'v' && eye)
//    {
//        eye->setFlipVertical(!eye->getFlipVertical());
//    }
//    else if (key == 'h' && eye)
//    {
//        eye->setFlipHorizontal(!eye->getFlipHorizontal());
//    }
}
