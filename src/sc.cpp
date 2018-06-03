
#include "sc.h"

using namespace cv;
using namespace std;


void sc::carveImage(Mat& image, int new_cols, int new_rows, int width, int height){
    cout << endl << "Processing image..." << endl;
    for(int i = 0; i < width - new_cols; i++){
        reduceSeam(image, 'v');
    }
    for(int i = 0; i < height - new_rows; i++){
        reduceSeam(image, 'h');
    }
}


void sc::computeEnergy(Mat &image, Mat &output){
    Mat dx, dy;
    Sobel(image, dx, CV_64F, 1, 0);
    Sobel(image, dy, CV_64F, 0, 1);
    magnitude(dx,dy, output);

    double min_value, max_value, Z;
    minMaxLoc(output, &min_value, &max_value);
    Z = 1/max_value * 255;
    output = output * Z;                    //normalize
    output.convertTo(output, CV_8U);
}


vector<uint> sc::findSeam(Mat &image){
    int H = image.rows, W = image.cols;

    int dp[H][W];
    for(int c = 0; c < W; c++){
        dp[0][c] = (int)image.at<uchar>(0,c);
    }

    for(int r = 1; r < H; r++){
        for(int c = 0; c < W; c++){
            if (c == 0)
                dp[r][c] = min(dp[r-1][c+1], dp[r-1][c]);
            else if (c == W-1)
                dp[r][c] = min(dp[r-1][c-1], dp[r-1][c]);
            else
                dp[r][c] = min(min(dp[r-1][c-1], dp[r-1][c]), dp[r-1][c+1]);
            dp[r][c] += (int)image.at<uchar>(r,c);
        }
    }

    int min_value = 2147483647; //infinity
    int min_index = -1;
    for(int c = 0; c < W; c++)
        if (dp[H-1][c] < min_value) {
            min_value = dp[H - 1][c];
            min_index = c;
        }

    vector<uint> path(H);
    Point pos(H-1,min_index);
    path[pos.x] = pos.y;

    while (pos.x != 0){
        int value = dp[pos.x][pos.y] - (int)image.at<uchar>(pos.x,pos.y);
        int r = pos.x, c = pos.y;
        if (c == 0){
            if (value == dp[r-1][c+1])
                pos = Point(r-1,c+1);
            else
                pos = Point(r-1,c);
        }
        else if (c == W-1){
            if (value == dp[r-1][c-1])
                pos = Point(r-1,c-1);
            else
                pos = Point(r-1,c);
        }
        else{
            if (value == dp[r-1][c-1])
                pos = Point(r-1,c-1);
            else if (value == dp[r-1][c+1])
                pos = Point(r-1,c+1);
            else
                pos = Point(r-1,c);
        }
        path[pos.x] = pos.y;
    }

    return path;
}


void sc::reducePixels(Mat& image, Mat& output, vector<uint> seam){



    for(int r = 0; r < output.rows; r++ ) {
        for (int c = 0; c < output.cols; c++){
            if (c >= seam[r])
                output.at<Vec3b>(r,c) = image.at<Vec3b>(r,c+1);
            else
                output.at<Vec3b>(r,c) = image.at<Vec3b>(r,c);
        }
    }
}

void sc::rotate90(Mat &matImage, int rotateflag){
    //1=CW, 2=CCW, 3=180
    if (rotateflag == 1){
        transpose(matImage, matImage);
        flip(matImage, matImage,1); //transpose+flip(1)=CW
    } else if (rotateflag == 2) {
        transpose(matImage, matImage);
        flip(matImage, matImage,0); //transpose+flip(0)=CCW
    } else if (rotateflag ==3){
        flip(matImage, matImage,-1);    //flip(-1)=180
    } else if (rotateflag != 0){ //if not 0,1,2,3:
        cout  << "Unknown rotation flag(" << rotateflag << ")" << endl;
    }
}

void sc::reduceSeam(Mat& image, char orientation = 'v'){
    if (orientation == 'h')
        rotate90(image,1);
    int H = image.rows, W = image.cols;

    Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);

    Mat eimage;
    computeEnergy(gray, eimage);

    vector<uint> seam = findSeam(eimage);

    Mat output(H,W-1, CV_8UC3);
    reducePixels(image, output, seam);

    if (orientation == 'h')
        rotate90(output,2);

    image = output;
}






