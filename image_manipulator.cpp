/*
 * This is simply an example of utilizing the STIMP functionality
 */

#include "stimp.h"

int main(int argc, char** argv){
  cvImg testImg;
  //testImg.readImg("sedona2018.JPG");
  testImg.getFilename();
  testImg.resizeImg();
  testImg.writeImg();
  // int i=1;
  // while(1){
  //   testImg.hueShift(i*5);
  //   testImg.showImg(1600,900,"Window",1);
  //   i++;
  // }

  //testImg.equalize();
  //testImg.showImg(400,225,"testImg");

  return 0;
}
