#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <math.h>
#include <leastsquares.h>
#include <miscelaneous.h>

using namespace cv;
using namespace std;


#define TEST_IMAGE "/home/stanke/test.png";


int main(int argc, char *argv[])
{
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed;
  start = chrono::high_resolution_clock::now();


  stringstream text;
  string image_name;
  auto colGen = Color().setup(100,200);
  if(argc>1){
      image_name = argv[1];
    }
  if(image_name.empty()){
      image_name = TEST_IMAGE;
    }

  auto iMat = new Mat();
  Mat original;
  original = imread(image_name, CV_8UC1);
  if(original.empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }
  //adaptiveThreshold(original, *iMat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25,5);
  threshold(original, *iMat, 127, 255, THRESH_BINARY);

  /*for(int i=0;i<original.cols;i+=100){
      line(*iMat, Point(i, 0), Point(i, original.rows), 255, 2);
  }*/

  copyMakeBorder(*iMat, *iMat, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  copyMakeBorder(original, original, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  // showCvMat("AT", *iMat, 1);


  vector<contour> contours;
  vector<Vec4i> hierarchy;
  findContours(*iMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

  vector<contour> largeContours;

  filterContours(contours, largeContours, 50);

  Mat arrLargeContours = Mat::zeros(original.rows, original.cols, CV_8UC3);

  Mat imatcolor = Mat(iMat->rows, iMat->cols, CV_8UC3);
  cvtColor(original, imatcolor,COLOR_GRAY2BGR);
  drawColorContours(imatcolor, largeContours, hierarchy);

  auto prvaKontura = *largeContours.begin();
  pointVector filled;
  fillContour(prvaKontura, filled);
  auto ls = LeastSquares(filled);
  auto rndclr = colGen.randomColor();
  drawPointVector(imatcolor, filled, rndclr, 3, LINE_AA);
  line(imatcolor, Point(0, ls.getY(0)), Point(imatcolor.cols, ls.getY(imatcolor.cols)), rndclr, 2, LINE_AA );
  text = stringstream();
  text << "R2: "<<ls.R2*100<<"%";
  int x=0, y=0;
  do{
    x+=50;
    y=ls.getY(x);

  }while(y<1 | y>imatcolor.rows);
  putText(imatcolor, text.str(), Point(x, y), FONT_ITALIC, 1, rndclr, 2, LINE_AA);
  showCvMat("Eh?", imatcolor, 0);


  // putText(imatcolor, text.str(), Point(10,100), FONT_HERSHEY_COMPLEX, 1, Scalar(125,125,0), 1, LINE_AA);

  for(contour cntr:largeContours){
      Scalar rndclr = colGen.randomColor();
      // fillConvexPoly(imatcolor, cntr,rndclr);
      // maybe use FILLED in drawContours instead?
      ls = LeastSquares(cntr);
      line(imatcolor, Point(0, ls.getY(0)), Point(imatcolor.cols, ls.getY(imatcolor.cols)), rndclr, 2, LINE_AA );
      text = stringstream();
      text << "R2: "<<ls.R2*100<<"%";
      int x=0, y=0;
      do{
        x+=50;
        y=ls.getY(x);

      }while(y<1 | y>imatcolor.rows);
      putText(imatcolor, text.str(), Point(x, y), FONT_ITALIC, 1, rndclr, 2, LINE_AA);
  }

  end = chrono::high_resolution_clock::now();
  elapsed = end-start;
  cout << "Got and calculated in " << elapsed.count() << " ms\n";

  showCvMat("Lines", imatcolor);
  return 0;
}
