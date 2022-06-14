    #ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"
#include "ofxPS3EyeGrabber.h"
#include "ofxOsc.h"

struct jentik_param{
    ofParameter<bool> ps3_autobalance;
    ofParameter<bool> ps3_autogain;
    ofParameter<int> ps3_brightness;
    ofParameter<int> ps3_contrast;
    ofParameter<int> ps3_green;
    ofParameter<int> ps3_red;
    ofParameter<int> ps3_blue;
    ofParameter<int> ps3_threshold;
    ofParameter<int> ps3_draw_x;
    ofParameter<int> ps3_draw_y;
    ofParameter<float> ps3_draw_scale;
} ;



class videoManager
{
public:

    std::shared_ptr<ofxPS3EyeGrabber> ps3eye;
    ofParameterGroup parameters;

    videoManager();
    void setup();
    void update();
    void draw();
    void listenerFunction(ofAbstractParameter& e);
    void learnBackground();
//    void setLearnBackground();

private:

    /// The video grabber.
    ofVideoGrabber grabber;

    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;
    ofxCvContourFinder      contourFinder;
    ofImage                 videoSource;

    ofxOscSender sender;

    jentik_param param;

    int                 threshold;
    bool                bLearnBakground;
    bool                bEnableGui;

    /// True if we are recording into the buffer.
    bool isRecording = false;

    /// The current frame in the buffer.
    std::size_t currentFrame = 0;
};

#endif // VIDEOMANAGER_H
