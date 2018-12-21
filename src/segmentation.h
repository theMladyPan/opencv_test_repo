#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class segmentation
{
private:
    vector<Scalar> usedColors;
    bool colorUsed(Scalar color);
    void segmentation();
    void useColor(Scalar color);
public:
    segmentation();
};

#endif // SEGMENTATION_H
