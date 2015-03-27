# Introduction

ofxFaceRecognizer an addon for openframeworks that implements the face recognition tutorial from the opencv site:
http://docs.opencv.org/modules/contrib/doc/facerec/facerec_tutorial.html

It needs opencv 2 or higher.

You extract a face from a video or a static image and feed it in to the recognizer.

At the start the recognizer either loads an already exciting .yml file that contains the recognition model or it creates a new model from the images it finds in the faces folder.

You can select between 3 different recognition methods: eigenfaces, fisherfaces or LBPHFaces.

I am supplying some example faces created by the Tarrlab at Brown University.
http://wiki.cnbc.cmu.edu/Face_Place

You notice that the image file names (MF0907_1100_HA.tiff) contain unique IDs to label each person.
Each person has more then one image but always the same ID. This ID is used by ofxFaceRecognizer to label each person sepratly.

I used ofxCv align eyes and crop functions to prepare the images before placing them in the faces folder.


# Installation

Download the source from GitHub, unzip the folder, rename it from ofxFaceRecognizer-master to ofxFaceRecognizer and place it in your `openFrameworks/addons` folder.

Or use project generator to build project and then:

modify Project.xcconfig as described in ofxOpenCV2461

add to
project-> build settings -> other linker flags
../../../addons/ofxOpenCv/libs/opencv/lib/osx/opencv.a

add to
targets-> build settings ->library search paths
$(OPENCV_LIBS_PATH)


# Dependencies

ofxOpenCV2461
https://github.com/pkmital/ofxOpenCV2461
Requires your project to link against ZLib and OpenCL.

ofxCv
https://github.com/kylemcdonald/ofxCv

ofxYAML
https://github.com/satoruhiga/ofxYAML


# Operating systems

ofxFaceRecognizer has only been tested on osx 10.10 with OF 0.8.4

## Images
Screen shot:

![](https://raw.githubusercontent.com/antimodular/ofxFaceRecognizer/master/Screen_Shot.png)