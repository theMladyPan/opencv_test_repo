#include <string>
#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <math.h>
#include <cstdlib>
#include <map>

using namespace cv;
using namespace std;

typedef Point2i coord_;

#define TEST_IMAGE "/home/stanke/test.png";

struct compPix {
    bool operator() (const Point2d& lhs, const Point2d& rhs) const
        {return (lhs.x*UINT16_MAX + lhs.y) < (rhs.x*UINT16_MAX + rhs.y);}
};
typedef map<coord_, int, compPix> segments_;

void getNeighbours(vector<coord_>& neighbours, const segments_& segments, const uint16_t& x, const uint16_t& y){
    vector<coord_> coords;
    coords.push_back(coord_( x  , y-1 ));
    coords.push_back(coord_( x-1, y-1 ));
    coords.push_back(coord_( x+1, y-1 ));
    coords.push_back(coord_( x  , y+1 ));
    coords.push_back(coord_( x-1, y+1 ));
    coords.push_back(coord_( x+1, y+1 ));
    coords.push_back(coord_( x-1, y   ));
    coords.push_back(coord_( x+1, y   ));

    neighbours.clear();
    for(auto coord:coords){
        if(segments.count(coord)==1){
            neighbours.push_back(coord);
        }
    }
}

uint32_t getLowestIndex(vector<coord_>& neighborHood, segments_& segments){
    uint32_t lowest(UINT32_MAX);
    uint32_t current;
    for(auto& coord:neighborHood){
        current = segments.at(coord);
        lowest = current<lowest?current:lowest;
    }
    return lowest;
}

bool setIndex(const vector<coord_>& neighborHood, segments_& segments, const uint32_t index){
    bool changed = false;
    uint32_t current;
    for(auto& coord:neighborHood){
        current = segments[coord];
        if(current != index){
            segments[coord]=index;
            changed = true;
        }
    }
    return changed;
}

void reconstruct(const segments_& segments, Mat outArr){
    for(auto& elem:segments){
        outArr.at<uint8_t>(elem.first.x, elem.first.y) = elem.second%192 + 64;
    }
}

void printDuration(chrono::high_resolution_clock::time_point start){
    chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << duration.count()<< "ms" << endl;
}

int main(int argc, char *argv[])
{
    string image_name;
    if(argc>1){
      image_name = argv[1];
    }
    if(image_name.empty()){
      image_name = TEST_IMAGE;
    }
    auto input = imread(image_name, CV_8UC1);
    if(input.empty()){
        // image is empty;
        return 5;
    }

    cout << "Prvotna segmentacia"<<endl;
    auto c_start = chrono::high_resolution_clock::now();
    segments_ segments;
    uint8_t tmp;
    uint32_t index(0);
    for(int row=0;row<=input.rows;row++){
        for(int col=0;col<input.cols; col++){
            tmp=input.at<uint8_t>(row, col);
            if(tmp<255){
                segments[coord_(row, col)] = index++;
            }
        }
    }
    printDuration(c_start);


    vector<coord_> neighborhood;
    bool changed = true;
    int iteracia(0);
    while(changed){
        changed = false;
        cout<<"Iteracia: "<<++iteracia<<endl;
        auto start = chrono::high_resolution_clock::now();
        for(auto& elem:segments){
            auto pix = elem.first;

            getNeighbours(neighborhood, segments, pix.x, pix.y);
            uint32_t lowest = getLowestIndex(neighborhood, segments);

            if(setIndex(neighborhood, segments, lowest)){
                changed = true;
            }
        }
        for(auto elem=segments.end();elem!=segments.begin();elem--){
            auto pix = elem->first;

            getNeighbours(neighborhood, segments, pix.x, pix.y);
            uint32_t lowest = getLowestIndex(neighborhood, segments);

            if(setIndex(neighborhood, segments, lowest)){
                changed = true;
            }

        }
        printDuration(start);
    }

    cout << "Spolu" <<endl;
    printDuration(c_start);

    Mat outarr(input.rows, input.cols, input.type());
    reconstruct(segments, outarr);
    imwrite("/home/stanke/test2.png", outarr);
}
