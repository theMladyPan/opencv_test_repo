#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pixattr.h"

using namespace std;
using namespace cv;

class Segmentation
{
private:
    enum Neighbour{
        top, bot, left, right
    };
    Mat *mat = new Mat();
    Mat *segmented = new Mat();
    PixAttrPool *PAPool = new PixAttrPool();
    vector<Scalar> *usedColors = new vector<Scalar>();
    bool colorUsed(Scalar color);
    void useColor(Scalar color);
    bool ommitedPixInImage(int ommited = 255);// TODO: Refactor for differen matrix types

    template<typename _Tp>
    Scalar neighbourColor(int row, int column, int type);
    void firstIteration();

    template<typename _Tp>
    bool neighbourInPool(int row, int column, int type);
    template<typename _Tp>
    PixAttr *neighbourPixAttr(int row, int column, int type);
public:
    Segmentation(Mat imat);
    void calculate();
};

#endif // SEGMENTATION_H
