#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <math.h>

using namespace cv;
using namespace std;
typedef vector<Point> contour;


RNG rng(12345);

#define TEST_IMAGE "/home/stanke/samples/original2.png";

double average(vector<float> &array){
    double average(0);
    int num(0);
    for(auto it : array){
        average+=static_cast<double>(it);
        num++;
    }
    return average/static_cast<double>(num);
}

Point avgPoint(vector<Point> &points){
    int x(0), y(0), i(0);
    for(auto it=points.begin();it!=points.end();it++){
        x+=it->x;
        y+=it->y;
        i++;
    }
    return Point(static_cast<int>(x/i), static_cast<int>(y/i));
}

int showMat(string name, Mat &inpArr, int delay_ms=0){
    namedWindow(name, WINDOW_NORMAL);// Create a window for display.
    imshow(name, inpArr);                   // Show our image inside it.
    return waitKey(delay_ms);
}

void filterHorizontalContours(vector<contour> &contours, vector<contour> &filtered){
    /// does not work
    int maxx, minx, maxy, miny;
    filtered=vector<contour>();
    cout<<contours.begin()->at(0);
    for(auto contour=contours.begin();contour!=contours.end();contour++) {
        cout<<"never executed?";
        // for every contour
        maxx=contour->begin()->x;
        minx=contour->begin()->x;
        maxy=contour->begin()->y;
        miny=contour->begin()->y;
        for(auto it=contour->begin();it!=contour->end();it++){
            if(it->x>maxx){
                maxx=it->x;
            }else if (it->x<minx) {
                minx=it->x;
            }
            if(it->y<miny){
                miny=it->y;
            }else if (it->y>maxy) {
                maxy=it->y;
            }
        }
        if((maxy-miny)>(maxx-minx)){
            filtered.push_back(*contour);
        }
        cout << (maxy-miny) << ":" << (maxx-minx)<<"\n";
    }
}

void findVerticalLines(contour input, Point &top, Point &bot){
    vector<Point> candidateTop;
    vector<Point> candidateBot;
    int maxY(input.begin()->y);
    int minY(input.begin()->y);

    for(auto it=input.begin();it!=input.end(); it++){
        if(it->y < minY){
            minY = it->y;
        }else if (it->y > maxY) {
            maxY = it->y;
        }
    }
    for(auto it=input.begin();it!=input.end(); it++){
        if(it->y == minY){
            candidateTop.push_back(*it);
        }else if (it->y == maxY) {
            candidateBot.push_back(*it);
        }
    }

    top = avgPoint(candidateTop);
    bot = avgPoint(candidateBot);
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
  adaptiveThreshold(original, *iMat, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 35,25);
  for (int i = 100; i < iMat->rows; i+=100) {
      line(*iMat, Point(0,i), Point(iMat->cols, i), Scalar(255,255,255), 2, LINE_8);
  }
  for (int i = 100; i < iMat->cols; i+=100) {
      line(*iMat, Point(i,0), Point(i, iMat->rows), Scalar(255,255,255), 2, LINE_8);
  }
  copyMakeBorder(*iMat, *iMat, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  copyMakeBorder(original, original, 1, 1, 1, 1, BORDER_CONSTANT, 255);
  showMat("AT", *iMat, 1);
  if(iMat->empty()){
      cerr << "image is empty!" << endl;
      return -1;
    }
/*
  // Setup SimpleBlobDetector parameters.
  SimpleBlobDetector::Params params;

  // Change thresholds
  params.minThreshold = 100;
  params.maxThreshold = 150;

  // Filter by Area.
  params.filterByArea = true ;
  params.minArea = 300;

  // Filter by Circularity
  params.filterByCircularity = false;
  params.minCircularity = 0.1f;

  // Filter by Convexity
  params.filterByConvexity = false;
  params.minConvexity = 0.5f;

  // Filter by Inertia
  params.filterByInertia = false;
  params.maxInertiaRatio = .1f;


  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

  // Detect blobs.
  std::vector<KeyPoint> keypoints;
  detector->detect( *iMat, keypoints);

  // Draw detected blobs as red circles.
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

  Mat im_with_keypoints = Mat::zeros(iMat->rows, iMat->cols, CV_8UC1);
  bitwise_not(im_with_keypoints, im_with_keypoints);
  drawKeypoints( *iMat, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
*/
  // Show blobs
  // showMat("input", im_with_keypoints, 1);

  vector<contour> contours;
  vector<Vec4i> hierarchy;
  findContours(*iMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  /*Mat drawing = Mat::zeros( iMat->size(), CV_8UC3 );
  drawColorContours(drawing, contours, hierarchy);*/

  //showMat( "Contours", drawing, 10 );
  vector<vector<Point>> largeContours;

  for(auto vec=contours.begin(); vec!=contours.end(); vec++){
      if(vec->size() > 50){
            largeContours.push_back(*vec);
        }
  }

  Mat arrLargeContours = Mat::zeros(original.rows, original.cols, CV_8UC3);

  //showMat("Large contours", arrLargeContours, 10);
/*
  Mat withLine;
  Point tl, br;
  for(auto it=largeContours.begin(); it!=largeContours.end();it++){
      findVerticalLines(*it, tl, br);
      arrowedLine(arrLargeContours,tl, br, Scalar(rng.uniform(0,256),rng.uniform(0,256),rng.uniform(0,256)), 2, LINE_AA);
  }
*/

  Mat imatcolor = Mat(iMat->rows, iMat->cols, CV_8UC3);
  cvtColor(original, imatcolor,COLOR_GRAY2BGR);
  drawColorContours(imatcolor, largeContours, hierarchy);
  //addWeighted(arrLargeContours, 0.5, imatcolor, 0.5, 0, arrLargeContours);

  vector<float> widths;

  vector<RotatedRect> rectangles;
  for(auto contour: largeContours) {
    rectangles.push_back(minAreaRect(contour));
  }
  for(auto rectangle:rectangles){

        Point2f rect_points[4]; rectangle.points( rect_points );
        auto randomColor(Scalar(rng.uniform(0,125), rng.uniform(0,125), rng.uniform(0,125)));
        for( int j = 0; j < 4; j++ ){
          line( imatcolor, rect_points[j], rect_points[(j+1)%4], randomColor, 1, LINE_AA );
        }
        stringstream center;
        float lesser(rectangle.size.width<rectangle.size.height?rectangle.size.width:rectangle.size.height);
        widths.push_back(lesser);
        center<<"Width "<<lesser<<" pix";
        putText(imatcolor, center.str(), rectangle.center, FONT_HERSHEY_COMPLEX, 1, randomColor, 1, LINE_AA);

}
  stringstream text;
  text << "Average line width: "<<average(widths)<< "pix.";
  putText(imatcolor, "Vertical line detector", Point(10,50), FONT_HERSHEY_COMPLEX, 1, Scalar(125,125,0), 1, LINE_AA);
  putText(imatcolor, text.str(), Point(10,100), FONT_HERSHEY_COMPLEX, 1, Scalar(125,125,0), 1, LINE_AA);


  end = chrono::high_resolution_clock::now();
  elapsed = end-start;
  cout << "Got and calculated in " << elapsed.count() << " ms\n";
  showMat("Lines", imatcolor);
  return 0;
}
