/*
Simple Terminal Image Manipulation Program

Written by Jackson Banbury, 2018

Based heavily on the OpenCV Library
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <exiv2/exiv2.hpp>

using namespace cv;
using namespace std;

class cvImg {
  public:
    //cvImg(); //constructor
    string fileName; //filename of image
    Mat baseImage;

    void readImg(string newFilename){
      baseImage = imread(newFilename);
      fileName = newFilename;
    }

    void showImg(int width=1600, int height=900, string windowName = "Window", int wait=1){
      namedWindow(windowName,WINDOW_NORMAL);
      resizeWindow(windowName,width,height);
      imshow(windowName,baseImage);
      if(wait){
        waitKey(0);
      }
    }

    void writeImg(string writeName="out.png"){
      imwrite(writeName,baseImage);
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
      cvtColor(baseImage, hsv, COLOR_BGR2HSV);
      for (int j = 0; j < baseImage.rows; j++)
      {
          for (int i = 0; i < baseImage.cols; i++)
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
      //convert baseImage back to BGR
      cvtColor(hsv, baseImage, COLOR_HSV2BGR);
    }

    void invColor(){
      Mat tempImg;
      bitwise_not(baseImage,tempImg);
      baseImage = tempImg;
    }

    void cropImg(int targetWidth=500, int targetHeight=500, int offsetX=0, int offsetY=0)
    {

      //Note that the offset is defined by pixels away from center
      offsetX = baseImage.size().width/2 + offsetX;
      offsetY = baseImage.size().height/2 + offsetY;

      Rect roi;
      roi.x = offsetX;
      roi.y = offsetY;
      roi.width = targetWidth;
      roi.height = targetHeight;
      try{
        baseImage = baseImage(roi);
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
      for(int y=0; y < baseImage.rows; y++){
        for(int x=0; x < baseImage.cols; x++){
          for(int color=0; color<3; color++){
            baseImage.at<Vec3b>(y,x)[color] = saturate_cast<uchar>( alpha*( baseImage.at<Vec3b>(y,x)[color] ) + beta );
          }
        }
      }
    }

    void changeSaturation(double saturation=0){
      int scale=1;
      baseImage.convertTo(baseImage, CV_8UC1, scale, saturation);
    }

    void flipHorizontal(){
      Mat tempImg;
      flip(baseImage, tempImg, 1);
      baseImage = tempImg;
    }

    void flipVertical(){
      Mat tempImg;
      flip(baseImage, tempImg, 0);
      baseImage = tempImg;
    }

    void resizeImg(int height=0, int width=0){
      if(height==0 || width==0){
        cout << "Resized width: "; cin >> width;
        cout << "Resized height: "; cin >> height;
      }
      Size size(height,width);
      Mat tempImg;
      resize(baseImage,tempImg,size);
      baseImage = tempImg;
    }

    void rotateImg(double angle=90){
      //Get the rotation matrix
      Point2f center((baseImage.cols-1)/2.0, (baseImage.rows-1)/2.0);
      Mat rot = getRotationMatrix2D(center,angle,1.0);
      //Determine the bounding rectangle
      Rect2f bbox = RotatedRect(Point2f(), baseImage.size(), angle).boundingRect2f();
      //Adjust transformation matrix
      rot.at<double>(0,2) += bbox.width/2.0 - baseImage.cols/2.0;
      rot.at<double>(1,2) += bbox.height/2.0 - baseImage.rows/2.0;

      Mat tempImg;
      warpAffine(baseImage, tempImg, rot, bbox.size());
      baseImage = tempImg;
    }

    void colorEnhance(){
      //Automatic: increases the saturation range of the colors in the layer,
      //without altering brightness or hue. See https://docs.gimp.org/en/plug-in-color-enhance.html

      int a=0;

    }

};

int main(int argc, char** argv){
  cvImg testImg;
  testImg.readImg("Lenna.png");
  testImg.changeSaturation(80);
  testImg.readExif();
  testImg.showImg(400,225,"testImg");

  return 0;
}
