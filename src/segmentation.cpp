#include "segmentation.h"

bool Segmentation::colorUsed(Scalar color)
{
  bool flag=false;
  for(auto it=this->usedColors->begin(); it!=this->usedColors->end();it++){
      if(color == *it) flag = true;
    }
  return flag;
}

void Segmentation::calculate()
{
  bool changeOccured = true;
  while(changeOccured){
      changeOccured = false;
      cout<<"kek";
    }
}

void Segmentation::useColor(Scalar color)
{
  this->usedColors->push_back(color);
}

bool Segmentation::ommitedPixInImage(int ommited)
{
  // default for ommited is 255 - means pixel was not processed yet
  for(auto it=this->mat->begin<uint8_t>();it!=this->mat->end<uint8_t>();it++){
      if(*it == ommited){
          return true;
        }
    }
    return false;
}

Segmentation::Segmentation(Mat imat)
{
  imat.copyTo(*mat);
}

template<typename _Tp>
Scalar Segmentation::neighbourHaveColor(int row, int column, int type)
{
  assert(row>=0 && column>=0 && row<= this->mat->rows && column <= this->mat->rows);
  Scalar tested(255);
  switch (type) {
    case left:
      if (this->mat->at<_Tp>(row-1, column) != 255) {
        tested = Scalar(this->mat->at<_Tp>(row-1, column));
        }
     case top:
      if (this->mat->at<_Tp>(row, column+1) != 255) {
        tested = Scalar(this->mat->at<_Tp>(row, column+1));
        }
     case bot:
      if (this->mat->at<_Tp>(row, column-1) != 255 ) {
        tested = Scalar(this->mat->at<_Tp>(row, column-1));
        }
     case right:
      if (this->mat->at<_Tp>(row+1, column) != 255 ) {
        tested = Scalar(this->mat->at<_Tp>(row+1, column));
        }

    }
  return tested;
}
