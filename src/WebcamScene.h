#ifndef WEBCAMSCENE_H
#define WEBCAMSCENE_H

#include "ofxScene.h"
#include "videomanager.h"
#include "framediffer.h"


struct struct_scene_jentik_assets {
    ofImage mask;
//    ofTexture mask;
};

class WebcamScene : public ofxScene {
public:
    bool bMask;
    FrameDiffer fd;
    WebcamScene(videoManager * videoman){
        videoMan = videoman;
        bMask = true;
    }

    void setup(){
        scene_jentik_assets.mask.load("jentik_mask.png");
        fd.set(300, 50, 100, 200);
        fd.setup();
    }

    void update(){

    }

    void draw(){
        ofClear(ofColor::black);
            ofPushMatrix();
                ofBackground(0);
                videoMan->draw();
                scene_jentik_assets.mask.draw(0,0);
            ofPopMatrix();
    }


//    void keyPressed(int key)
//    {

//        if (key == 'm') {
//            bMask = !bMask;
//        }
//    }

private:
    videoManager * videoMan;
    struct_scene_jentik_assets scene_jentik_assets;
};

#endif // WEBCAMSCENE_H
