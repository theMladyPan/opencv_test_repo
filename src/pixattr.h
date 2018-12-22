#ifndef PIXATTR_H
#define PIXATTR_H
#include <iostream>
#include <vector>
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

static RNG rng(12345);

class PixAttr
{
private:
    uint16_t nOfPixels = 0;
    vector<pair<uint16_t, uint16_t>> *pixels = new vector<pair<uint16_t, uint16_t>>();
    /// Unique index for PixAttr
    uint16_t index;
    Scalar *color = new Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
public:
    /** Construct new pixel attirbute
     * param uint16_t index - unique index for attribute */
    PixAttr(uint16_t index);
    uint16_t getIndex();
    Scalar getColor();
    uint16_t getNOfPixels();
    /** Add pair(column, row) for common attribute */
    void addPair(uint16_t column, uint16_t row);
    bool hasPair(pair<uint16_t, uint16_t> coordinate);


};
/// Compare two attributes based on index
bool operator > (PixAttr lPA, PixAttr rPA);
/// Compare two attributes based on index
bool operator == (PixAttr lPA, PixAttr rPA);

class PixAttrPool
{
private:
    /// Highest index of PixAttr in pool
    uint16_t highestIndex;
    friend uint16_t getIndex(PixAttrPool *pool, pair<uint16_t, uint16_t> coordinates);
public:
    vector<PixAttr> *pool = new vector<PixAttr>();
    /// Create new pool of PixAttrs
    PixAttrPool();
    /// Add PixAttr to pool
    void addPixAttr(PixAttr pa);
    /// Generate and return new PixAttr to pool
    PixAttr addPixAttr();
    bool hasPixAttr(PixAttr *pa);
    PixAttr *getPixAttrByPair(pair<uint16_t, uint16_t> coordinate);
    void listPixAttrs();
};

#endif // PIXATTR_H
