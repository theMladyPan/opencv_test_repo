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

using namespace cv;
using namespace std;
typedef vector<Point> contour;


RNG rng(12345);

#define TEST_IMAGE "/home/stanke/test.png";

double average(vector<float> &array){
    double average(0);
    int num(0);
    for(auto it : array){
        average+=static_cast<double>(it);
        num++;
    }
    return average/static_cast<double>(num);
}

int showMat(string name, Mat &inpArr, int delay_ms=0){
    namedWindow(name, WINDOW_NORMAL);// Create a window for display.
    imshow(name, inpArr);                   // Show our image inside it.
     return waitKey(delay_ms);
}

void drawColorContours(Mat &destArray, vector<contour> &contours, vector<Vec4i> &hierarchy){
    for( size_t i = 0; i< contours.size(); i++ )
    {
        auto color = Scalar( 0,0,125 );
        drawContours( destArray, contours, (int)i, color, 1, LINE_AA, hierarchy, 0);
    }
}

int main(int argc, char *argv[])
{
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed;


  string image_name;
  if(argc>1){
      image_name = argv[1];
    }
  if(image_name.empty()){
      image_name = TEST_IMAGE;
    }

  start = chrono::high_resolution_clock::now();
  auto iMat = new Mat();
  Mat original;
  original = imread(image_name, CV_8UC1);
  adaptiveThreshold(original, *iMat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 55,25);
  copyMakeBorder(*iMat, *iMat, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  copyMakeBorder(original, original, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  showMat("AT", *iMat, 1);
  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  vector<contour> contours;
  vector<Vec4i> hierarchy;
  findContours(*iMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

  vector<vector<Point>> largeContours;

  for(auto vec=contours.begin(); vec!=contours.end(); vec++){
      if(vec->size() > 200){
            largeContours.push_back(*vec);
            break;
        }
  }

  Mat arrLargeContours = Mat::zeros(original.rows, original.cols, CV_8UC3);

  Mat imatcolor = Mat(iMat->rows, iMat->cols, CV_8UC3);
  cvtColor(original, imatcolor,COLOR_GRAY2BGR);
  drawColorContours(arrLargeContours, largeContours, hierarchy);
  drawColorContours(imatcolor, largeContours, hierarchy);
  cvtColor(arrLargeContours, arrLargeContours, COLOR_BGR2GRAY);

  //putText(imatcolor, text.str(), Point(10,100), FONT_HERSHEY_COMPLEX, 1, Scalar(125,125,0), 1, LINE_AA);
  LeastSquares ls(arrLargeContours);

  line(imatcolor, Point(0,ls.getIntercept()), Point(300, ls.getSlope()*300+ls.getIntercept()), Scalar(255,127,0), 2, LINE_AA );

  end = chrono::high_resolution_clock::now();
  elapsed = end-start;
  cout << "Got and calculated in " << elapsed.count() << " ms\n";
  showMat("Lines", imatcolor);
  return 0;
}
