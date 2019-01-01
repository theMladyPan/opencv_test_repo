#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/opengl.hpp>
#include <chrono>
#include <thread>
#include <GL/gl.h>

#include <math.h>
#include <cstdlib>
//#include "noise_filter.h"

using namespace cv;
using namespace std;


#define MAX_32BIT 4294967295
#define TEST_IMAGE "/home/stanke/test.jpg"


// segmentation PoC
class Segmentation{
private:
  Mat *_originalArray = new Mat();
  Mat *_segmentedArray = new Mat();
  Mat *_segmented8bit = new Mat();
  Mat *_changed = new Mat();
  uint32_t _segmentN = 1;

  void firstIteration(){
    for(int row=0;row<_segmentedArray->rows;row++){
        for(int col=0;col<_segmentedArray->cols;col++){
            // copy matrix, turn pixels of interrest into 1;
            if(_originalArray->at<uint8_t>(row,col)>0){
                _segmentedArray->at<uint8_t>(row, col) = 1;
              }
          }
      }

    for(int row=0;row<this->_segmentedArray->rows;row++){
        for(int col=0;col<this->_segmentedArray->cols;col++){
            if(_originalArray->at<uint8_t>(row, col)>0){
                // pixel nie je cierny
                uint32_t neighbourVal = topOrLeftValue(row, col);
                if(neighbourVal > 1){
                    // sused ma uz pridelenu hodnotu
                    _segmentedArray->at<uint32_t>(row, col) = neighbourVal;
                  }else {
                    _segmentedArray->at<uint32_t>(row, col) = ++_segmentN;
                  }
              }
          }
      }
  }

  uint32_t neighbourValue(int row, int col){
    uint32_t botVal = MAX_32BIT;
    uint32_t rightVal = MAX_32BIT;
    uint32_t topVal = MAX_32BIT;
    uint32_t leftVal = MAX_32BIT;
    uint32_t temp = 0;
    if(row>1){
        temp = _segmentedArray->at<uint32_t>(row-1, col);
        topVal = (temp==0)?MAX_32BIT:temp;
      }
    if(col>1){
        temp = _segmentedArray->at<uint32_t>(row, col-1);
        leftVal = (temp==0)?MAX_32BIT:temp;
      }
    if(row < _originalArray->rows - 1){
        temp = _segmentedArray->at<uint32_t>(row+1, col);
        botVal = (temp==0)?MAX_32BIT:temp;
      }
    if(col < _originalArray->cols - 1){
        temp = _segmentedArray->at<uint32_t>(row, col+1);
        rightVal = (temp==0)?MAX_32BIT:temp;
      }
    return min(min(botVal, rightVal), min(leftVal, rightVal));
  }

  uint32_t topOrLeftValue(int row, int col){
    uint32_t topVal = MAX_32BIT;
    uint32_t leftVal = MAX_32BIT;
    if(row>1){
        topVal = _segmentedArray->at<uint32_t>(row-1, col);
      }
    if(col>1){
        leftVal = _segmentedArray->at<uint32_t>(row, col-1);
      }
    return min(topVal, leftVal);
  }
  void convertTo8bit();
  void nIterations();
  void convToGtex();
public:
  Segmentation(Mat *inputArray){
    *_segmentedArray = Mat::zeros(inputArray->rows, inputArray->cols, CV_32FC1);
    inputArray->copyTo(*_originalArray);
    inputArray->copyTo(*_segmented8bit);
    _changed->zeros(inputArray->rows, inputArray->cols, CV_8UC1);
  }
  Mat *computeSegments(){
    firstIteration();
    nIterations();
    cout << "NofSegments: " << _segmentN <<endl;
    return _segmentedArray;
  }
  Mat get8BitSegments(){
    convertTo8bit();
    return *_segmented8bit ;
  }

};

void Segmentation::nIterations(){
  bool changeOcc = true;
  uint32_t nOfIteration = 0;
  uint16_t nOfChanges;
  while (changeOcc){
      changeOcc = false;
      nOfChanges = 0;
      nOfIteration++;
      _changed->zeros(_segmentedArray->rows, _segmentedArray->cols, _changed->type());
      for(int row=0;row<_segmentedArray->rows;row++){
          for(int col=0;col<_segmentedArray->cols;col++){
              if(_originalArray->at<uint8_t>(row, col)>0){
                  // pixel nie je cierny
                  uint32_t neighbourVal = neighbourValue(row, col);
                  uint32_t myVal = _segmentedArray->at<uint32_t>(row, col);

                  if(neighbourVal < myVal and neighbourVal>1){
                      // sused ma uz pridelenu hodnotu
                      _segmentedArray->at<uint32_t>(row, col) = neighbourVal;
                      cout << "X:"<<row<<" Y:"<<col<<" neighbour:"<<neighbourVal<<endl;
                      //_changed->at<uint8_t>(row, col) = 127;
                      changeOcc = true;
                      nOfChanges++;
                    }
                }
            }
        }
      /*for(int row=_segmentedArray->rows;row>0;row--){
          for(int col=_segmentedArray->cols;col>0;col--){
              if(_originalArray->at<uint8_t>(row, col)>0){
                  // pixel nie je cierny
                  uint32_t neighbourVal = neighbourValue(row, col);
                  uint32_t myVal = _segmentedArray->at<uint32_t>(row, col);

                  if(neighbourVal < myVal and neighbourVal>1){
                      // sused ma uz pridelenu hodnotu
                      _segmentedArray->at<uint32_t>(row, col) = neighbourVal;
                      changeOcc = true;
                      nOfChanges++;
                    }
                }
            }
        }*/
      cout<<"Iteration: " << nOfIteration<<", Changes: " << nOfChanges<<endl;
      convertTo8bit();
      imshow("progress", *_segmented8bit);
      waitKey(0);

    }
}

void Segmentation::convertTo8bit(){
  for(int row=0;row<_segmentedArray->rows;row++){
      for(int col=0;col<_segmentedArray->cols;col++){
          uint32_t temp = _segmentedArray->at<uint32_t>(row, col);
          if(temp>0){
              _segmented8bit->at<uint8_t>(row, col) = static_cast<uint8_t>(temp % 128);
          }else{
              _segmented8bit->at<uint8_t>(row, col) = 0;
          }
        }
    }
}

double foobar(double n){
    if(n>1){
        return n*foobar(n-1);
    }else{
        return 1;
    }
}

template<typename fCall, typename param1>
void timeit(fCall function, param1 param){
    auto start = std::chrono::high_resolution_clock::now();
    cout <<function(param);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Took " << elapsed.count() << " ms\n";
}


int main(int argc, char **argv)
{
  string image_name;
  if(argc>1){
      image_name = argv[1];
    }
  if(image_name.empty()){
      image_name = TEST_IMAGE;
    }

  auto iMat = new Mat();
  auto destMat = new Mat();
  *iMat = imread(image_name, CV_8UC1);
  *destMat = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  /*resize(*iMat, *iMat, Size(),1, 1, INTER_LINEAR);
  adaptiveThreshold(*iMat,*destMat, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 175, 5);*/
  threshold(*iMat, *destMat, 1, 255, THRESH_BINARY);

  if(destMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }

  namedWindow( "progress", WINDOW_NORMAL|WINDOW_KEEPRATIO|WINDOW_GUI_EXPANDED);// Create a window for display.
  imshow( "progress", *destMat );                   // Show our image inside it.
  waitKey(0);


  auto start = std::chrono::high_resolution_clock::now();
  Segmentation sgmt(destMat);
  sgmt.computeSegments();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  std::cout << "Took " << elapsed.count() << " ms\n";

  imshow("progress", sgmt.get8BitSegments());
  waitKey(0);


  delete iMat;
  delete destMat;
  return 0;
}
