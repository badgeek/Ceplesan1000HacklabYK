//
//  ImageScene.cpp
//  example-Simple
//
//  Created by Koki Nomura on 2014/03/12.
//
//

#include "ImageScene.h"

ImageScene::ImageScene(string filename){
    _background.load(filename);
    _width = _background.getWidth() * (ofGetWidth()/_background.getWidth()) ;
    _height = _background.getHeight() * (ofGetHeight()/_background.getHeight()) ;
}

void ImageScene::setup() {
}

void ImageScene::update() {
    
}

void ImageScene::draw() {
    ofClear(ofColor::black);
    _background.draw(0,0);
}

void ImageScene::willFadeIn() {

}

void ImageScene::willDraw() {

}

void ImageScene::willFadeOut() {

}

void ImageScene::willExit() {
    
}
