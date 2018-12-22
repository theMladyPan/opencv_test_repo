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

void PixAttr::addPair(uint16_t column, uint16_t row)
{
  pixels->push_back(pair<uint16_t, uint16_t>(column, row));
  this->nOfPixels++;
}

bool PixAttr::hasPair(pair<uint16_t, uint16_t> coordinate)
{
  for(auto it=this->pixels->begin();it!=this->pixels->end();it++){
      if(coordinate.first == it->first && coordinate.second == it->second){
          return true;
        }
    }
  return false;
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

uint16_t getIndex(PixAttrPool *pool, pair<uint16_t, uint16_t> coordinates)
{
  for(auto it=pool->pool->begin();it!=pool->pool->end();it++){
      if(it->hasPair(coordinates)){
          return it->getIndex();
        }
    }
  return 0;
}
