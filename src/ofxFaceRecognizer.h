//
//  ofxFaceRecognizer.h
//  LOC_10
//
//  Created by Stephan Schulz on 2015-02-10.
//
//

#pragma once


#include <stdio.h>
#include "ofxCv.h"


using namespace cv;
using namespace ofxCv;

class ofxFaceRecognizer {
public:
    ofxFaceRecognizer();
    virtual ~ofxFaceRecognizer();
    
    void setup(int method_used, int _maxFaces, bool bAlreadySavedModel, string folderName);
    void loadTrainingImages(string _folderName, int _maxFaces);
    
    float getImageWidth();
    float getImageHeight();
    
    ofImage getImageOfPerson(int _id);
    void drawImageOfPerson(int _id, float _x, float _y, float _w, float _h);
    void drawImageOfPerson(int _id, ofPoint _p, float _w, float _h);
    
    void update(ofImage _img);
    void update(Mat _mat);
    int getPrediction();
    double getConfidence();
    
    int getUniquePersonCount();
    int getMethodId();
    string getMethodName();
    
    void generateEigenFishFaces();
    void drawEigefaceJET(int _id, float _x, float _y, float _w, float _h);
    void drawEigenfaceBONE(int _id, float _x, float _y, float _w, float _h);
    void drawReconstructionImage(int _id, float _x, float _y, float _w, float _h);
    int getEigenfaceSize();
    int getReconstructionSize();
    
     vector<int> allTrainingLabels;
protected:
    
    int databaseImage_width,databaseImage_height;
    Ptr<FaceRecognizer> model;
    
    int maxFaces;

    
    vector<ofImage> allTrainingImages;
    vector<ofImage> oneImagePerPerson;
    vector<Mat> allTrainingMats;
   
    

    int uniquePersonCount;
    int image_width,image_height;
    
    int prediction;
    double confidence_double;
    
    string methodName;
    int methodId;
    
    vector<ofImage>cgrayscaleJET_array;
    vector<ofImage>cgrayscaleBONE_array;
    vector<ofImage>reconstruction_array;
    
    
};