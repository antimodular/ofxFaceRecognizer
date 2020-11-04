//
//  ofxFaceRecognizer.cpp
//  LOC_10
//
//  Created by Stephan Schulz on 2015-02-10.
//
//
#include "ofMain.h"
#include "ofxFaceRecognizer.h"


ofxFaceRecognizer::ofxFaceRecognizer()
{
}

ofxFaceRecognizer::~ofxFaceRecognizer(){
    //close();
}

void ofxFaceRecognizer::setup(int _maxFaces, bool bAlreadySavedModel, string folderName) {

    //eigen take much longer to load and longer to generate. also makes much larger yml file
    string method_names[3] = {"eigen","fisher","lbph"};
    
    // Create a FaceRecognizer and train it on the given images:

   
#if defined(USE_EIGEN)
    methodId = 0;
    model = EigenFaceRecognizer::create();
#elif defined(USE_FISHER)
    methodId = 1;
    model = FisherFaceRecognizer::create();
#elif defined(USE_LBPHF)
    methodId = 2;
    model = LBPHFaceRecognizer::create();
#else
#endif
     methodName = method_names[methodId];
    
    //if(_maxFaces > 0){
    
    maxFaces = _maxFaces;
   
 
    //by default my training images should be 150x150 pixels
    //will reset if you use loadTrainingImages()
    image_width = 150;
    image_height = 150;
    //load in training images
    loadTrainingImages(folderName,maxFaces);
    
    
    string compiledDatabasePath = ofToDataPath("model_"+methodName+"_"+ofToString(maxFaces)+"_"+folderName+".yml");
   
    if(bAlreadySavedModel){
        cout<<"model .yml supposedly existing"<<endl;
        cout<<"if it crashes here set bAlreadySavedModel = false which generates a new database model"<<endl;
        model->read(compiledDatabasePath);
        cout<<"loaded "<<maxFaces<<" faces with model "<<methodName<<endl;
    }else{
        cout<<"start training new model. this might take a very long time"<<endl;
        cout<<"more so for fisher than eigen"<<endl;
        model->train(allTrainingMats, allTrainingLabels);
        model->save(ofToDataPath(compiledDatabasePath));
        cout<<"trained and saved .yml with "<<maxFaces<<" faces with model "<<methodName<<endl;
    }
    
    // Quit if there are not enough images for this demo.
    if(allTrainingMats.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    

//    if(methodId == 0 || methodId == 1) generateEigenFishFaces();

    
}

void ofxFaceRecognizer::update(ofImage _img){
    
}
void ofxFaceRecognizer::update(Mat _mat){
    model->predict(_mat, prediction, confidence_double);
}


void ofxFaceRecognizer::loadTrainingImages(string _folderName, int _maxFaces){
    
    //folderName = "Tarrlab_rot+crop";
    string imagePath = ofToDataPath(_folderName);
    cout<<"imagePath "<<imagePath<<endl;
    ofDirectory dir(imagePath);
    //  dir.allowExt("pgm");
    dir.allowExt("tiff");
    dir.listDir();
    
    cout<<"dir.numFiles() "<<dir.size()<<endl;
    int lableCnt = 0;
    int imgCountPerPerson = 0;
    string lastLabel = "";
    
    
    for(int i = 0; i < _maxFaces; i++){
        
     //   cout<<"i "<<i;
        string fileName = dir.getName(i);
      //  cout<<" fileName "<<fileName<<endl;
        //file containts ID for each person
        //here you need to parse the file name to get this ID specific to each person
        string temp_label = fileName.substr(0,6);
        cout<<i<<" "<<lableCnt<<" , "<<temp_label<<endl;
        
        ofImage temp_img;
        temp_img.load(dir.getPath(i));
        allTrainingImages.push_back(temp_img); //stores all the images from database
        allTrainingLabels.push_back(lableCnt); //stores the label for each image. each person has a different label but all the images of the same person have the same label
        
        //since we have more then one pose/image of the same person
        //we only store the first image to reppresent that person
        if(imgCountPerPerson == 0) oneImagePerPerson.push_back(temp_img);
        imgCountPerPerson++;
        
        if(temp_label != lastLabel){
            lastLabel = temp_label;
            
            imgCountPerPerson = 0;
            lableCnt++;
        }

    }
    
    dir.close();
    
    //tried to read files right away in to Mat array but something did not work well.
    //so now first read into ofimage array then to mat array
    for(int i=0; i<allTrainingImages.size();i++){
        allTrainingMats.push_back(toCv(allTrainingImages[i]));
    }
    
    cout<<"image source "<<_folderName<<endl;
    cout<<"found "<<lableCnt<<" different people"<<endl;
    
    uniquePersonCount =  lableCnt; //231
    
    image_width = oneImagePerPerson[0].getWidth();
    image_height = oneImagePerPerson[0].getHeight();
    
    
}

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
        case 1:
            cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
            break;
        case 3:
            cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
            break;
        default:
            src.copyTo(dst);
            break;
    }
    return dst;
}

/*
void ofxFaceRecognizer::generateEigenFishFaces(){
    int height = allTrainingMats[0].rows;
   
    
    // Here is how to get the eigenvalues of this Eigenfaces model:
    Mat eigenvalues = model->getMat("eigenvalues");
    cout<<"eigenvalues.row "<<eigenvalues.rows<<endl;
    cout<<"eigenvalues.cols "<<eigenvalues.cols<<endl;
    
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat W = model->getMat("eigenvectors");
    cout<<"W.row "<<W.rows<<endl;
    cout<<"W.cols "<<W.cols<<endl;
    
    // Get the sample mean from the training data
    Mat mean = model->getMat("mean");
    cout<<"mean.row "<<mean.rows<<endl;
    cout<<"mean.cols "<<mean.cols<<endl;
    
    // Display or save the first, at most 16 Fisherfaces:
    cgrayscaleJET_array.clear();
    cgrayscaleBONE_array.clear();
    
    for (int a = 0; a < min(16, W.cols); a++) {
        //for (int i = 0; i < 16; i++) {
        string msg = format("Eigenvalue #%d = %.5f", a, eigenvalues.at<double>(a));
        cout << msg << endl;
        // get eigenvector #a
        Mat ev = W.col(a).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Bone colormap for better sensing.
        Mat cgrayscale;
        applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
        
        // Display or save:
        ofImage ti;
        toOf(cgrayscale,ti);
        cgrayscaleJET_array.push_back(ti);
        
        applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
        toOf(cgrayscale,ti);
        cgrayscaleBONE_array.push_back(ti);
        
    }
    

    reconstruction_array.clear();
    // Display or save the image reconstruction at some predefined steps:
    if(methodId == 0){
        for(int num_component = min(W.cols, 10); num_component < min(W.cols, 300); num_component+=15) {
            Mat evs = Mat(W, Range::all(), Range(0, num_component));
            Mat projection = subspaceProject(evs, mean, allTrainingMats[0].reshape(1,1));
            Mat reconstruction = subspaceReconstruct(evs, mean, projection);
            // Normalize the result:
            reconstruction = norm_0_255(reconstruction.reshape(1, allTrainingMats[0].rows));
            
            // Normalize the result:
            reconstruction = norm_0_255(reconstruction.reshape(1, allTrainingMats[0].rows));
            // Display or save:
            
            ofImage ti;
            toOf(reconstruction,ti);
            //reconstruction_array[num_component] = ti;
            reconstruction_array.push_back(ti);
        }
    }
    if(methodId == 1){
        for(int num_component = 0; num_component < min(16, W.cols); num_component++) {
            // Slice the Fisherface from the model:
            Mat ev = W.col(num_component);
            Mat projection = subspaceProject(ev, mean, allTrainingMats[0].reshape(1,1));
            Mat reconstruction = subspaceReconstruct(ev, mean, projection);
            
            // Normalize the result:
            reconstruction = norm_0_255(reconstruction.reshape(1, allTrainingMats[0].rows));
          
            // Display or save:
            ofImage ti;
            toOf(reconstruction,ti);
            reconstruction_array.push_back(ti);
        }
    }
    
}
*/

int ofxFaceRecognizer::getEigenfaceSize(){
    return cgrayscaleJET_array.size();
}
int ofxFaceRecognizer::getReconstructionSize(){
    return reconstruction_array.size();
}
void ofxFaceRecognizer::drawEigefaceJET(int _id, float _x, float _y, float _w, float _h){
    cgrayscaleJET_array[_id].draw(_x, _y, _w, _h);
}
void ofxFaceRecognizer::drawEigenfaceBONE(int _id, float _x, float _y, float _w, float _h){
    cgrayscaleBONE_array[_id].draw(_x, _y, _w, _h);
}

void ofxFaceRecognizer::drawReconstructionImage(int _id, float _x, float _y, float _w, float _h){
   reconstruction_array[_id].draw(_x, _y, _w, _h);
}

int ofxFaceRecognizer::getPrediction(){
    return prediction;
}
double ofxFaceRecognizer::getConfidence(){
    return confidence_double;
}
float ofxFaceRecognizer::getImageWidth(){
    return image_width;
}
float ofxFaceRecognizer::getImageHeight(){
    return image_height;
}
ofImage ofxFaceRecognizer::getImageOfPerson(int _id){
    return oneImagePerPerson[_id];
}
void ofxFaceRecognizer::drawImageOfPerson(int _id, float _x, float _y, float _w, float _h){
    oneImagePerPerson[_id].draw(_x, _y, _w, _h);
}
void ofxFaceRecognizer::drawImageOfPerson(int _id, ofPoint _p, float _w, float _h){
    oneImagePerPerson[_id].draw(_p, _w, _h);
}
int ofxFaceRecognizer::getUniquePersonCount(){
    return uniquePersonCount;
}
int ofxFaceRecognizer::getMethodId(){
    return methodId;
}
string ofxFaceRecognizer::getMethodName(){
    return methodName;
}
