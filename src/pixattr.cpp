#include "pixattr.h"

PixAttr::PixAttr(uint16_t index)
{
  this->index = index;
}

uint16_t PixAttr::getIndex()
{
  return this->index;
}

Scalar PixAttr::getColor()
{
  return *(this->color);
}

void PixAttr::addPair(uint8_t column, uint8_t row)
{
  pixels->push_back(pair<uint8_t, uint8_t>(column, row));
  this->nOfPixels++;
}

bool operator>(PixAttr lPA, PixAttr rPA)
{
  return lPA.getIndex()>rPA.getIndex();
}


PixAttrPool::PixAttrPool()
{
  this->highestIndex = 0;
}

void PixAttrPool::addPixAttr(PixAttr pa)
{
  if(!this->hasPixAttr(&pa)){
      this->pool->push_back(pa);
      if(pa.getIndex()>this->highestIndex){
          this->highestIndex = pa.getIndex();
        }
    }
}

PixAttr PixAttrPool::addPixAttr()
{
  auto newPA = PixAttr(++highestIndex);
  this->pool->push_back(newPA);
  return newPA;
}

bool PixAttrPool::hasPixAttr(PixAttr* pa)
{
  for(auto it=this->pool->begin();it!=this->pool->end();it++){
      if(*pa == *it){
          return true;
        }
    }
  return false;
}

bool operator ==(PixAttr lPA, PixAttr rPA)
{
  return lPA.getIndex() == rPA.getIndex();
}
