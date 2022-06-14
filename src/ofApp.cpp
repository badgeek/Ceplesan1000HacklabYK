//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

// Scene
#include "ImageScene.h"
#include "WebcamScene.h"

#define HOST "localhost"
#define PORT 12345

void ofApp::setup()
{
    ofEnableAlphaBlending();
//    ofSetFrameRate();
    ofSetFrameRate(29);
    ofSetVerticalSync(true);
    ofToggleFullscreen();

//    sender.setup(HOST, PORT);
    jentikcam.setup();

    gui.setHeaderBackgroundColor(ofColor::black);
    gui.setBackgroundColor(ofColor::black);
    gui.setup(jentikcam.parameters);
    gui.add(btntest.setup("DESTROY"));


    cout << ofGetWidth() << "\n";
    cout << ofGetHeight() << "\n";


    ofAddListener(gui.savePressedE, this, &ofApp::guiSave);
    ofAddListener(gui.loadPressedE, this, &ofApp::guiLoad);

//  sync.setup(jentikcam.parameters, 8888, "127.0.0.1", 9999);
    //Add scene
//    sceneManager.addScene(ofPtr<ofxScene>(new ImageScene("scene1.png")));
    sceneManager.addScene(ofPtr<ofxScene>(new WebcamScene(&jentikcam)));
//    sceneManager.addScene(ofPtr<ofxScene>(new ImageScene("scene2.png")));

    sceneManager.setTransitionDissolve();
    sceneManager.setSceneDuration(3,3);
    sceneManager.run();

    effectsFBO.allocate(ofGetWidth(), ofGetHeight());
    fxGlitch.setup(&effectsFBO);

    light.setup("192.168.43.66", 8888);

    bGui = false;
    bMask = true;

    warper.setup();
    warper.activate();

    cout << ofGetWidth();
    cout << ofGetHeight();



}

void ofApp::guiSave(){
    gui.saveToFile("settings.xml");
}


void ofApp::guiLoad(){
    gui.loadFromFile("settings.xml");
}


void ofApp::update()
{
    jentikcam.update();
    sceneManager.update();

    effectsFBO.begin();
        sceneManager.draw();
    effectsFBO.end();

//    sync.update();
}


void ofApp::draw()
{
//    warper.begin();	///all the things that are drawn AFTER ofxGLWarper's begin method are afected by it.
//    warper.draw();
    ofBackground(0);
    fxGlitch.generateFx();
    effectsFBO.draw(0,0);
//    warper.end();

    if(bGui) gui.draw();

}

void ofApp::keyPressed(int key)
{
    switch (key){
        case 'c':
            jentikcam.learnBackground();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'b':
            bGui = !bGui;
        break;
//        case ' ':
//            sceneManager.changeScene();
//            break;
        case 'l':
                ofxOscMessage m;
                m.setAddress("/relay");
                m.addFloatArg(1.0f);
                light.sendMessage(m, false);
        break;
    }


    if (key == '1') fxGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
    if (key == '2') fxGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
    if (key == '3') fxGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
    if (key == '4') fxGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
    if (key == '5') fxGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
    if (key == '6') fxGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, true);
    if (key == '7') fxGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
    if (key == '8') fxGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
    if (key == '9') fxGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
    if (key == '0') fxGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);
    if (key == 'q') fxGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') fxGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, true);
    if (key == 'e') fxGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, true);
    if (key == 'r') fxGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, true);
    if (key == 't') fxGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, true);
    if (key == 'y') fxGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, true);




}

void ofApp::keyReleased(int key)
{
    if (key == '1') fxGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
    if (key == '2') fxGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
    if (key == '3') fxGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
    if (key == '4') fxGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
    if (key == '5') fxGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
    if (key == '6') fxGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, false);
    if (key == '7') fxGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
    if (key == '8') fxGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
    if (key == '9') fxGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
    if (key == '0') fxGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);
    if (key == 'q') fxGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    if (key == 'w') fxGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, false);
    if (key == 'e') fxGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, false);
    if (key == 'r') fxGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, false);
    if (key == 't') fxGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, false);
    if (key == 'y') fxGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, false);
    if (key == 'u') fxGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
}
