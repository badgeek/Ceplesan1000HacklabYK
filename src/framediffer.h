/********  Test sample for ofxInteractiveObject									********/
/********  Make sure you open your console to see all the events being output	********/


#pragma once

#include "ofxMSAInteractiveObject.h"
#include "ofMain.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000
#define		DETECTED_COLOR  0xFF0000


class FrameDiffer : public ofxMSAInteractiveObject {
public:

    int posx, posy;
    int saveX, saveY;
    ofImage * sourceVideo;
    ofImage cropVideo;
    float differenceVideo;
    float differenceVideo2;
    int jelly_id;
    int widthRatio;
    int heightRatio;

    int videoQuadW;
    int videoQuadH;

    bool doubleSize;
    bool deleteMe;

    int offsetDrawX, offsetDrawY;

    bool beatDetected;

    unsigned char savedVideo[1440000];

    void setOffsetDraw(int x, int y) {
        offsetDrawX = x;
        offsetDrawY = y;
        this->offsetMouseX = -1*x;
        this->offsetMouseY = -1*y;
    }

    float getVideoMagniW()
    {
        return videoQuadW/sourceVideo->getWidth();
    }


    float getVideoMagniH()
    {
        return videoQuadH/sourceVideo->getHeight();
    }

    float getVideoRatio()
    {
        return sourceVideo->getWidth()/sourceVideo->getHeight();
    }

    float getWidthRatio()
    {
        return sourceVideo->getWidth()/videoQuadW;
    }

    float getHeightRatio()
    {
        return sourceVideo->getHeight()/videoQuadH;
    }


    void setup() {
        printf("MyTestObject::setup() - hello!\n");
        cropVideo.allocate(100,100, OF_IMAGE_COLOR_ALPHA);
        doubleSize = false;
        deleteMe = false;
        enableMouseEvents();
        enableKeyEvents();
        beatDetected = false;
    }


    void exit() {
        printf("MyTestObject::exit() - goodbye!\n");
    }


    void update() {

        float cropX = x*getWidthRatio();
        float cropY = y*getHeightRatio();
//cropY -= 70;
        float cropW = width/getVideoMagniW();
        float cropH = height/getVideoMagniH();

         cropVideo.cropFrom(*sourceVideo,cropX,cropY,cropW,cropH);
         cropVideo.setImageType(OF_IMAGE_GRAYSCALE);
           // cropVideo.update();
         float crop_width = cropVideo.getWidth();
         float crop_height = cropVideo.getHeight();

         float channel_crop = cropVideo.getPixelsRef().getBytesPerPixel();
         float arrpos_crop = (cropY*channel_crop*cropVideo.getWidth()) + x*channel_crop;
         ofPixels pix = cropVideo.getPixels();

         float diff;

         float diff2;

         for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
         {
             diff = diff + (abs(savedVideo[x] - pix[x]));
             diff2 = diff2 + ((savedVideo[x] - pix[x]));

         }

         differenceVideo = diff/(crop_width * crop_height * channel_crop);
         differenceVideo2 = diff2/(crop_width * crop_height * channel_crop);


    }


    void draw() {

        int finalX = x+offsetDrawX;
        int finalY = y+offsetDrawY;

        ofPushStyle();

                if(isMousePressed()) ofSetHexColor(DOWN_COLOR);
                else if(isMouseOver()) ofSetHexColor(OVER_COLOR);
                else ofSetHexColor(IDLE_COLOR);

                if (differenceVideo2*10 > 10.0){

                    float supersize = differenceVideo2 *0.000001;
                    float centersize = supersize/2;
                    beatDetected = true;

                    if (differenceVideo2 *10 > 100)
                    {
                        supersize = differenceVideo2*0.000001;
                    }else{
                        supersize = differenceVideo2 *0.000002;
                    }


                    centersize = supersize/2;



                            ofSetHexColor(DETECTED_COLOR);
                            ofRect(finalX-centersize-2, finalY-centersize-2, (width+supersize)+4, (width+supersize)+4);
                            cropVideo.draw(finalX-centersize,finalY-centersize,width+supersize,width+supersize);

                }else{
                    ofRect(finalX-2, finalY-2, (width)+4, (height)+4);
                    cropVideo.draw(finalX,finalY,width,height);
                    beatDetected = false;
                }


        //        ofLine(x-2,0,x+2,0);
        //        ofDrawAxis(3);
                ofDrawBitmapString("id: " + ofToString(jelly_id) + " diff: " + ofToString(differenceVideo2),finalX-40,finalY-20);
                ofPopStyle();
    }

    virtual void onRollOver(int x, int y) {
//        printf("MyTestObject::onRollOver(x: %i, y: %i)\n", x, y);
    }

    virtual void onRollOut() {
//        printf("MyTestObject::onRollOut()\n");
    }

    virtual void onMouseMove(int x, int y){
//        printf("MyTestObject::onMouseMove(x: %i, y: %i)\n", x, y);
    }

    virtual void onDragOver(int x, int y, int button) {
//        printf("MyTestObject::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
        this->x = (x - saveX)+offsetDrawX;    // update x position
        this->y = (y - saveY)+offsetDrawY;    // update mouse y position
    }

    virtual void onDragOutside(int x, int y, int button) {
//        printf("MyTestObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
//        saveX = x - this->x;
//        saveY = y - this->y;
    }

    virtual void onPress(int x, int y, int button) {
//        printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
        saveX = (x - this->x)+offsetDrawX;
        saveY = (y - this->y)+offsetDrawY;
    }

    virtual void onRelease(int x, int y, int button) {
        printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
    }

    virtual void grabFrame()
    {

//        int crop_width = cropVideo.getWidth();
//        int crop_height = cropVideo.getHeight();

//        int channel_crop = cropVideo.getPixelsRef().getBytesPerPixel();
//        int arrpos_crop = (y*channel_crop*cropVideo.getWidth()) + x*channel_crop;

//        ofPixels pix = cropVideo.getPixels();

//        for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
//        {
//            savedVideo[x] = pix[x];
//        }
    }

    virtual void onReleaseOutside(int x, int y, int button) {
        printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
    }

    virtual void keyPressed(int key) {
        printf("MyTestObject::keyPressed(key: %i)\n", key);
        if(isMouseOver()){

            if (key == 61){
                this->width += 10;
                this->height += 10;
                grabFrame();

            }

            if (key == 45){
                this->width -= 10;
                this->height -= 10;
                grabFrame();
            }

            if (key == 'c')
            {
              grabFrame();
            }

            if (key == 'm')
            {
                deleteMe = true;
                printf("deleteme\n");
            }

        }
    }

    virtual void keyReleased(int key) {
        printf("MyTestObject::keyReleased(key: %i)\n", key);
    }

};
