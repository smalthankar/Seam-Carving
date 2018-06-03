#ifndef SEAMCARVINGCOMP665156
#define SEAMCARVINGCOMP665156

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class sc {

public:

    sc(Mat& im){
       Mat ii_image = im;
    }

    void computeEnergy(Mat &image, Mat &output);

    vector<uint> findSeam(Mat &image);

    void reducePixels(Mat &image, Mat &output, vector<uint> seam);

    void rotate90(Mat &matImage, int rotflag);

    void reduceSeam(Mat &image, char orientation);

    void carveImage(Mat &image, int new_cols, int new_rows, int width, int height);
};




#endif
