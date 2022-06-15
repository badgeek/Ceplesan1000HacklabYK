#include "videomanager.h"


#define HOST "localhost"
#define PORT 12345

#define CAM_W 640
#define CAM_H 480

videoManager::videoManager()
{
    bLearnBakground = true;
    threshold = 80;
    countframediff = 0;

}

void videoManager::addAnalyzer(int mouseX, int mouseY)
{
    FrameDiffer* new_frame_diff = new FrameDiffer();
    float ratio =  videoSource.getWidth()/videoSource.getHeight();

    new_frame_diff->set(mouseX-50-param.ps3_draw_x, mouseY-50-param.ps3_draw_y, 100, 100);
    new_frame_diff->sourceVideo = &videoSource;
    new_frame_diff->videoQuadW = ofGetWidth();
    new_frame_diff->videoQuadH = ofGetWidth()/ratio;
    new_frame_diff->jelly_id = countframediff++;
    new_frame_diff->setup();
    _frameDifferCol.push_back(new_frame_diff);
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
    videoSource.allocate(CAM_W, CAM_H, OF_IMAGE_COLOR);

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



    timer_interval = 29;
    timer_current =0;

}


void videoManager::listenerFunction(ofAbstractParameter& e){

    string param_name;

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
    grabber.update();

    if (grabber.isFrameNew())
    {
        colorImg.setFromPixels(grabber.getPixels());
        videoSource.setFromPixels(grabber.getPixelsRef());

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
                    m.setAddress("/trowacv/jentik" + ofToString(i));
                    m.addFloatArg(contourFinder.blobs[i].centroid.x);
                    m.addFloatArg(contourFinder.blobs[i].centroid.y);
                    sender.sendMessage(m, false);
        }
    }


    for (int x = 0; x < _frameDifferCol.size(); x++)
    {
        _frameDifferCol.at(x)->setOffsetDraw(param.ps3_draw_x, param.ps3_draw_y);
        if(_frameDifferCol.at(x)->deleteMe)
        {
            delete _frameDifferCol.at(x);
            _frameDifferCol.erase(_frameDifferCol.begin()+x);
        }
    }

    float timer_now = ofGetElapsedTimeMillis();

    if((timer_now - timer_current) > 100   )
    {
        timer_current = timer_now;

        ofxOscMessage m;
        m.setAddress("/differ/magnitude");

        for (int x = 0; x < _frameDifferCol.size(); x++)
        {
            m.addFloatArg(_frameDifferCol.at(x)->differenceVideo);
        }

        sender.sendMessage(m);

        ofxOscMessage b;
        b.setAddress("/differ/beat");

        for (int x = 0; x < _frameDifferCol.size(); x++)
        {
            b.addFloatArg(_frameDifferCol.at(x)->beatDetected);
        }

        sender.sendMessage(b);

    }
}

void videoManager::draw()
{
    float ratio =  videoSource.getWidth()/videoSource.getHeight();

    ofSetColor(255);
    ofPushMatrix();
      ofTranslate(param.ps3_draw_x, param.ps3_draw_y);
//      ofScale(param.ps3_draw_scale);
//      ofScale(ofGetWidth(), ofGetWidth()/ratio)
        colorImg.draw(0,0,ofGetWidth(), ofGetWidth()/ratio);
        ofPushStyle();
            ofNoFill();
            ofSetLineWidth(3);
            ofSetColor(ofColor::white);
            for (int i = 0; i < contourFinder.nBlobs; i++){
                ofRectangle test =  contourFinder.blobs[i].boundingRect;
                test.setHeight(test.height/ratio);
                    ofDrawRectangle(test);
             }
         ofPopStyle();
    ofPopMatrix();
//    videoSource.draw(0,0,300,400);

}
