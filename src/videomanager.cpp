#include "videomanager.h"


#define HOST "localhost"
#define PORT 12345

#define CAM_W 640
#define CAM_H 480

videoManager::videoManager()
{
    bLearnBakground = true;
    threshold = 80;
}

void videoManager::setup()
{

    grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    grabber.setDesiredFrameRate(75);
    grabber.setup(CAM_W, CAM_H);

    ps3eye = grabber.getGrabber<ofxPS3EyeGrabber>();
    ps3eye->setAutogain(true);
    ps3eye->setAutoWhiteBalance(true);

    colorImg.allocate(CAM_W,CAM_H);
    grayImage.allocate(CAM_W,CAM_H);
    grayBg.allocate(CAM_W,CAM_H);
    grayDiff.allocate(CAM_W,CAM_H);

    parameters.setName("IMGANALYZER");
    parameters.add(param.ps3_autogain.set("Autogain", ps3eye->getAutogain()));
    parameters.add(param.ps3_autobalance.set("Autobalance", ps3eye->getAutoWhiteBalance()));
    parameters.add(param.ps3_brightness.set("Brightness",ps3eye->getBrightness(),0,255));
    parameters.add(param.ps3_contrast.set("Contrast",ps3eye->getContrast(),0,255));
    parameters.add(param.ps3_red.set("R",ps3eye->getRedBalance(),0,255));
    parameters.add(param.ps3_green.set("G",ps3eye->getGreenBalance(),0,255));
    parameters.add(param.ps3_blue.set("B",ps3eye->getBlueBalance(),0,255));
    parameters.add(param.ps3_threshold.set("Threshold",threshold,0,255));
    parameters.add(param.ps3_draw_x.set("DrawX",0,-2000,2000));
    parameters.add(param.ps3_draw_y.set("DrawY",0,-2000,2000));
    parameters.add(param.ps3_draw_scale.set("DrawScale",2,-100.0,100.0));

    ofAddListener(parameters.parameterChangedE(), this, &videoManager::listenerFunction);

    sender.setup(HOST, PORT);

}


void videoManager::listenerFunction(ofAbstractParameter& e){
//do something
    string param_name;

//    auto ps3eye = jentikcam.ps3eye;

    param_name = e.getName();

    if (param_name == "R")
    {
        ps3eye->setRedBalance(param.ps3_red);
    }

    if (param_name == "G")
    {
        ps3eye->setGreenBalance(param.ps3_green);
    }

    if (param_name == "B")
    {
        ps3eye->setBlueBalance(param.ps3_blue);
    }

    if (param_name == "Brightness")
    {
        ps3eye->setBrightness(param.ps3_brightness);
    }

    if (param_name == "Contrast")
    {
        ps3eye->setContrast(param.ps3_contrast);
    }

    if (param_name == "Autogain")
    {
        ps3eye->setAutogain(param.ps3_autogain);
    }

    if (param_name == "Autobalance")
    {
        ps3eye->setAutoWhiteBalance(param.ps3_autobalance);
    }

    if (param_name == "Threshold")
    {
        threshold = (param.ps3_threshold);
    }


    if (param_name == "DrawX")
    {

    }


    if (param_name == "DrawY")
    {

    }


    if (param_name == "DrawScale")
    {

    }

}

void videoManager::learnBackground()
{
    bLearnBakground = true;
}



void videoManager::update()
{
//    sync.update();
    grabber.update();

    if (grabber.isFrameNew())
    {

        colorImg.setFromPixels(grabber.getPixels());
        grayImage = colorImg;

        if (bLearnBakground == true){
            grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
            bLearnBakground = false;
        }

        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayDiff, 100, (CAM_W*CAM_H)/3, 10, false);	// find holes


        for (int i = 0; i < contourFinder.nBlobs; i++){
            ofxOscMessage m;
                    m.setAddress("/jentik" + ofToString(i));
                    m.addFloatArg(contourFinder.blobs[i].centroid.x);
                    m.addFloatArg(contourFinder.blobs[i].centroid.y);
                    sender.sendMessage(m, false);
        }
//        if (isRecording)
//        {
//            auto frames = grabber.getGrabber<ofxPS3EyeGrabber>()->getAllFrames();

//            for (auto& frame: frames)
//            {
//                ofTexture tex;
//                tex.loadData(frame.pixels);
//                frameBuffer.push_back(tex);
//            }
//        }


    }

}

void videoManager::draw()
{
    ofSetColor(255);
    ofPushMatrix();
        ofTranslate(param.ps3_draw_x, param.ps3_draw_y);
        ofScale(param.ps3_draw_scale);
        colorImg.draw(0,0);
        ofPushStyle();
            ofNoFill();
            ofSetHexColor(0xffffff);
            for (int i = 0; i < contourFinder.nBlobs; i++){
                    ofDrawRectangle(contourFinder.blobs[i].boundingRect);
             }
         ofPopStyle();
    ofPopMatrix();
}
