#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>


using namespace cv;
using namespace std;

class noiseFilter
{
private:
    Mat mat;

public:
  noiseFilter(Mat imat);
  void applyFilter(int count);
  Mat getMat();
};

#endif // NOISE_FILTER_H
