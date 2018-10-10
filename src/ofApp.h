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
#include "videomanager.h"
#include "ofxSceneManager.h"


struct struct_scene_jentik_assets {
    ofImage mask;
//    ofTexture mask;
};

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

//    /// The buffer of images.
//    std::vector<ofTexture> frameBuffer;

    ofxOscSender sender;
    ofxOscReceiverSettings receiver;

    ofxPanel gui;
    ofxOscParameterSync sync;
    ofParameterGroup parameters;

    ofxPanel btnpanel;
    ofxButton btntest;

    videoManager jentikcam;

    struct_scene_jentik_assets scene_jentik_assets;

};
