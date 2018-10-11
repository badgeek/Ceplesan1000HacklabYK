//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"
#include "ofxSceneManager.h"
#include "ofxPostGlitch.h"

#include "videomanager.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void listenerFunction(ofAbstractParameter& e);
    void guiSave();
    void guiLoad();
    void keyPressed(int key) override;
    void keyReleased(int key) override;

//    /// The buffer of images.
//    std::vector<ofTexture> frameBuffer;

    ofxOscSender sender;
    ofxOscReceiverSettings receiver;
    ofxOscParameterSync sync;

    ofxPanel gui;
    ofParameterGroup parameters;
    ofxButton btntest;

    ofxSceneManager sceneManager;

    videoManager jentikcam;

    ofFbo effectsFBO;
    ofxPostGlitch fxGlitch;

    bool bFullScreen;
};
