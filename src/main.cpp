/*

Reference: www.wikipedia.com
www.gitub.com
http://www.cs.princeton.edu/courses/archive/fall13/cos226/assignments/seamCarving.html
http://vision.gel.ulaval.ca/~jflalonde/cours/4105/h14/tps/results/tp2/jingweicao/index.html

*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "sc.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    if(argc!=5){
        cout<<"Usage: ../sc input_image new_width new_height output_image"<<endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1]);

    if(!image.data)
    {
        cout<<"Could not load input image!!!"<<endl;
        return -1;
    }

    if(image.channels()!=3){
        cout<<"Image does not have 3 channels!!! "<<image.depth()<<endl;
        return -1;
    }

    sc s(image);
    if (!image.data) {
        cout << "No image found" << endl;
        return -1;
    }
    imshow( "Original image", image );
    int newRows=atoi(argv[3]);
    int newCols=atoi(argv[2]);

    s.carveImage(image, newCols, newRows, image.cols, image.rows);
    cout << "Done!" << endl;
    imwrite(argv[4], image);


    imshow( "Seam Carved Image", image );

    while (waitKey(20) != 27);
    image.release();

    return 0;
}
