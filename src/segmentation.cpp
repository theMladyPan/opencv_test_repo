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
  this->firstIteration();
  this->PAPool->listPixAttrs();
  /*
   * bool changeOccured = false;
   * while(changeOccured){
      if(false){
          changeOccured = true;
        }
      cout<<"kek";
    }
   */
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

void Segmentation::firstIteration()
{
  // for column in this->mat->cols;
  for(int row = 0;row<this->mat->cols;row++){
      for(int col = 0;col<this->mat->cols;col++){
          if(this->mat->at<uint8_t>(row, col) != 0){ //not black
              if (neighbourInPool<uint8_t>(row, col, top)) {
                  neighbourPixAttr<uint8_t>(row, col,top)->addPair(row, col);
                } else if(neighbourInPool<uint8_t>(row, col, left)) {
                  neighbourPixAttr<uint8_t>(row, col,left)->addPair(row, col);
                } else {
                  this->PAPool->addPixAttr();
                }
            }
        }
    }

}

template<typename _Tp>
bool Segmentation::neighbourInPool(int row, int column, int type)
{
  for(auto it=this->PAPool->pool->begin(); it!=this->PAPool->pool->end();it++){
      pair<uint16_t, uint16_t> targetPair;
      switch (type) {
        case left:
           targetPair = pair<uint16_t, uint16_t>(row, column-1);
          break;
        case top:
          targetPair = pair<uint16_t, uint16_t>(row-1, column);
          break;
        case bot:
          targetPair = pair<uint16_t, uint16_t>(row+1, column-1);
          break;
        case right:
          targetPair = pair<uint16_t, uint16_t>(row, column+1);
          break;
        }
      return it->hasPair(targetPair);
    }
}

Segmentation::Segmentation(Mat imat)
{
  imat.copyTo(*mat);
}

template<typename _Tp>
Scalar Segmentation::neighbourColor(int row, int column, int type)
{
  /* TODO: Fix asserts
  assert(row>=0 );
  assert(column>=0);
  assert(row<= this->mat->rows);
  assert(column <= this->mat->cols);
  */

  Scalar tested(255);
  switch (type) {
    case left:
      if (this->mat->at<_Tp>(row-1, column) != 255) {
        tested = Scalar(this->mat->at<_Tp>(row, column-1));
        }
      break;
    case top:
      if (this->mat->at<_Tp>(row, column+1) != 255) {
        tested = Scalar(this->mat->at<_Tp>(row-1, column));
        }
      break;
    case bot:
      if (this->mat->at<_Tp>(row, column-1) != 255 ) {
        tested = Scalar(this->mat->at<_Tp>(row+1, column));
        }
      break;
    case right:
      if (this->mat->at<_Tp>(row+1, column) != 255 ) {
        tested = Scalar(this->mat->at<_Tp>(row, column+1));
        }
      break;

    }
  return tested;
}

template<typename _Tp>
PixAttr *Segmentation::neighbourPixAttr(int row, int column, int type)
{
  for(auto it=this->PAPool->pool->begin(); it!=this->PAPool->pool->end();it++){
      pair<uint16_t, uint16_t> targetPair;
      switch (type) {
        case left:
           targetPair = pair<uint16_t, uint16_t>(row, column-1);
          break;
        case top:
          targetPair = pair<uint16_t, uint16_t>(row-1, column);
          break;
        case bot:
          targetPair = pair<uint16_t, uint16_t>(row+1, column-1);
          break;
        case right:
          targetPair = pair<uint16_t, uint16_t>(row, column+1);
          break;
        }
      if(it->hasPair(targetPair)){
          return this->PAPool->getPixAttrByPair(targetPair);
        };
      throw runtime_error("No PixAttr found of neighbour");
    }
}
