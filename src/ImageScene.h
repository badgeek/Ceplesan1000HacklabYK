//
//  ImageScene.h
//  example-Simple
//
//  Created by Koki Nomura on 2014/03/12.
//
//

#ifndef __example_Simple__ImageScene__
#define __example_Simple__ImageScene__

#include "ofxScene.h"

class ImageScene : public ofxScene {
public:
    ImageScene(string filename);
    void setup();
    void update();
    void draw();
    
    void willFadeIn();
    void willDraw();
    void willFadeOut();
    void willExit();
    
    int y=0;

    ofImage _background;

    int _width;
    int _height;
};

#endif /* defined(__example_Simple__ImageScene__) */
