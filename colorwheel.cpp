/*
Goal is to make this an image processing code. Make a class (Image or something)
that inherits from opencv's Mat class, so that functions can be added,
like the colorwheel, the image resizing, code, brightness changing, etc
*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
  Mat frame = imread("test.jpg");

  Mat hsv;

  const unsigned char hue_shift = 20;

  cvtColor(frame, hsv, COLOR_BGR2HSV);

  while(1){
    for (int j = 0; j < frame.rows; j++)
    {
        for (int i = 0; i < frame.cols; i++)
        {
            // Get hue.
            // Saturation is hsv.at<Vec3b>(j, i)[1], and
            // Value is hsv.at<Vec3b>(j, i)[2].
            unsigned char h = hsv.at<Vec3b>(j, i)[0];

            if (h + hue_shift > 180)
                h = (h + hue_shift) - 180;
            else
                h = h + hue_shift;

            // Set hue.
            hsv.at<Vec3b>(j, i)[0] = h;
        }
    }

    cvtColor(hsv, frame, COLOR_HSV2BGR);

    //imwrite("out.png",frame);
    namedWindow("Window",WINDOW_NORMAL);
    resizeWindow("Window",1600,900);
    imshow("Window",frame);
    waitKey(0);
}

  return 0;
}
