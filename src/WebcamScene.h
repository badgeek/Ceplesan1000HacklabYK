#ifndef WEBCAMSCENE_H
#define WEBCAMSCENE_H

#include "ofxScene.h"
#include "videomanager.h"


struct struct_scene_jentik_assets {
    ofImage mask;
//    ofTexture mask;
};

class WebcamScene : public ofxScene {
public:

    WebcamScene(videoManager * videoman){
        videoMan = videoman;
    }

    void setup(){
        scene_jentik_assets.mask.load("jentik_mask.png");
    }

    void update(){

    }

    void draw(){
        ofClear(ofColor::black);
            ofPushMatrix();
                ofScale( 1.125 ,1.125);
                ofBackground(0);
                videoMan->draw();
                scene_jentik_assets.mask.draw(0,0);
            ofPopMatrix();
    }

private:
    videoManager * videoMan;
    struct_scene_jentik_assets scene_jentik_assets;
};

#endif // WEBCAMSCENE_H
