/*
Simple Terminal Image Manipulation Program
Potential features to add:
* Stenography
* Panorama stitching
* Gif creation
* Exif stripping/customization

Written by Jackson Banbury, 2018

Based heavily on the OpenCV Library
*/

//Use the below when it is eventually a library (header)
#ifndef STIMP_H
#define STIMP_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

class cvImg {
  public:
    //cvImg(); //constructor
    string fileName; //filename of image
    Mat baseImg;

    void readImg(string newFilename){
      baseImg = imread(newFilename);
      fileName = newFilename;
    }

    void getFilename(){
      cout << "Enter the image path: ";
      cin >> fileName;
      readImg(fileName);
    }

    void showImg(int width=1600, int height=900, string windowName = "Window", int wait=1){
      namedWindow(windowName,WINDOW_NORMAL);
      resizeWindow(windowName,width,height);
      imshow(windowName,baseImg);
      if(wait){
        waitKey(0);
      }
    }

    void writeImg(string writeName="out.png"){
      imwrite(writeName,baseImg);
    }

    void convertType(int filetype){
      //convert to specified file type
      //Do this with if(filetype == 1) then convert to jpg, etc
      //use regex to get rid of the extension, then append it to new extension
      //and do writeImg(writeName)

    }

    void readExif(){
      //Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fileName);
      //image->readMetadata();
      //Exiv2::ExifData &exifData = image->exifData();
      //cout << "Exif data: ";
      //cout << exifData;
    }

    void hueShift(int shift){
      Mat hsv;
      cvtColor(baseImg, hsv, COLOR_BGR2HSV);
      for (int j = 0; j < baseImg.rows; j++)
      {
          for (int i = 0; i < baseImg.cols; i++)
          {
              // Get hue.
              // Saturation is hsv.at<Vec3b>(j, i)[1], and
              // Value is hsv.at<Vec3b>(j, i)[2].
              unsigned char h = hsv.at<Vec3b>(j, i)[0];
              if (h + shift > 180)
                  h = (h + shift) - 180;
              else
                  h = h + shift;
              // Set hue.
              hsv.at<Vec3b>(j, i)[0] = h;
          }
      }
      //convert baseImg back to BGR
      cvtColor(hsv, baseImg, COLOR_HSV2BGR);
    }

    void invColor(){
      Mat tempImg;
      bitwise_not(baseImg,tempImg);
      baseImg = tempImg;
    }

    void cropImg(int targetWidth=500, int targetHeight=500, int offsetX=0, int offsetY=0)
    {

      //Note that the offset is defined by pixels away from center
      offsetX = baseImg.size().width/2 + offsetX;
      offsetY = baseImg.size().height/2 + offsetY;

      Rect roi;
      roi.x = offsetX;
      roi.y = offsetY;
      roi.width = targetWidth;
      roi.height = targetHeight;
      try{
        baseImg = baseImg(roi);
      }
      catch(...){
        cout << "Error: Ensure your offsets are within the boundary of the image,"
          " and that your target dimensions are not larger than the original. \n";
      }
    }

    void changeBrightness(double alpha=-1, int beta=-1){
      if(alpha==-1 | beta==-1){
        cout << "Change the brightness of an image using basic linear transforms:\n";
        cout << "new(i,j) = a * source(i,j) + b\n";
        cout << "Enter gain value, alpha: "; cin >> alpha;
        cout << "\nEnter bias value, beta: "; cin >> beta;
      }
      for(int y=0; y < baseImg.rows; y++){
        for(int x=0; x < baseImg.cols; x++){
          for(int color=0; color<3; color++){
            baseImg.at<Vec3b>(y,x)[color] = saturate_cast<uchar>( alpha*( baseImg.at<Vec3b>(y,x)[color] ) + beta );
          }
        }
      }
    }

    void changeSaturation(double saturation=0){
      int scale=1;
      baseImg.convertTo(baseImg, CV_8UC1, scale, saturation);
    }

    void flipHorizontal(){
      Mat tempImg;
      flip(baseImg, tempImg, 1);
      baseImg = tempImg;
    }

    void flipVertical(){
      Mat tempImg;
      flip(baseImg, tempImg, 0);
      baseImg = tempImg;
    }

    void resizeImg(int height=0, int width=0){
      if(height==0 || width==0){
        cout << "Resized width: "; cin >> width;
        cout << "Resized height: "; cin >> height;
      }
      Size size(height,width);
      Mat tempImg;
      resize(baseImg,tempImg,size);
      baseImg = tempImg;
    }

    void rotateImg(double angle=90){
      //Get the rotation matrix
      Point2f center((baseImg.cols-1)/2.0, (baseImg.rows-1)/2.0);
      Mat rot = getRotationMatrix2D(center,angle,1.0);
      //Determine the bounding rectangle
      Rect2f bbox = RotatedRect(Point2f(), baseImg.size(), angle).boundingRect2f();
      //Adjust transformation matrix
      rot.at<double>(0,2) += bbox.width/2.0 - baseImg.cols/2.0;
      rot.at<double>(1,2) += bbox.height/2.0 - baseImg.rows/2.0;

      Mat tempImg;
      warpAffine(baseImg, tempImg, rot, bbox.size());
      baseImg = tempImg;
    }

    void colorEnhance(){
      //Automatic: increases the saturation range of the colors in the layer,
      //without altering brightness or hue. See https://docs.gimp.org/en/plug-in-color-enhance.html

      int a=0;

    }

    void equalize(){
      //TODO: Get this to work with non-gray images
      Mat hsvImg, tempImg;
      string source_window = "Source image";
      string equalized_window = "Equalized Image";

      cvtColor(baseImg, hsvImg, CV_BGR2GRAY);
      /// Apply Histogram Equalization
      equalizeHist(hsvImg, tempImg);
      /// Display results
      namedWindow(source_window, CV_WINDOW_NORMAL);
      namedWindow(equalized_window, CV_WINDOW_AUTOSIZE);
      imwrite("test_hist.jpg",tempImg);
      imshow(source_window, baseImg);
      imshow(equalized_window, tempImg);
      /// Wait until user exits the program
      waitKey(0);

    }

    void histogram(int hbins=30, int sbins=32, int scale=10){
      //Calculate and show the histogram for an image
      Mat hsvImg;
      cvtColor(baseImg, hsvImg, CV_BGR2HSV);

      int histSize[] = {hbins, sbins};
      float hranges[] = {0, 180};
      float sranges[] = {0, 256};
      const float* ranges[] = {hranges, sranges};
      MatND hist;
      int channels[] = {0, 1};

      calcHist(&hsvImg, 1, channels, Mat(),hist,2,histSize,ranges,true,false);

      double maxVal = 0;
      minMaxLoc(hist, 0, &maxVal, 0, 0);
      Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

      for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
          float binVal = hist.at<float>(h, s);
          int intensity = cvRound(binVal*255/maxVal);
          rectangle( histImg, Point(h*scale, s*scale),
                      Point( (h+1)*scale - 1, (s+1)*scale - 1),
                      Scalar::all(intensity),
                      CV_FILLED );
        }
        showImg();
        namedWindow("Histogram", 1);
        imshow("Histogram", histImg);
        waitKey(0);
    }

    void stenoWrite(string text){
      //using regex, if text contants a file extension, interpret
      //it as a file and read the contents of the file
      //Otherwise interpret it as text to hide in the image

      //Cypher the text

      //Convert text to bits

      //convert image to byte array

      //Allocate the first x bytes to define the length of the actual text (in bytes)
      //This is important so only the actual text is read, and not extra image bits

      //Iterate across the bytes of the image, along with the bits of the text
      //If text bit ==1, then last bit of image ==1, and vice versa

      //Convert back to standard Mat format, and save the image

    }

    void stenoRead(){
      //Read an image and recover the text from the stenography
      //First read the region that defines the length of the text
      //Then read the actual text in bitwise form
      //Convert bitwise form to string
      //decypher the string
      //Print the string
      //Optionally save it to a file


    }

};

#endif
