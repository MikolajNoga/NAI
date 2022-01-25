#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    bool flipStatus = false;
    bool capturing = true;
    
    cv::VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        cerr << "error while opening" << endl;
        return -1;
    }
    
    do {
        Mat frame;
        Mat dst = frame;
        char k = waitKey(1);
        if (k == ' ') flipStatus = !flipStatus;

        if (cap.read(frame)) {

            if (flipStatus) {
                flip(frame, dst, 1);
                imshow("Not-yet smart window", dst);
            }
            else
                imshow("Not-yet smart window", frame);
        }
        else
            capturing = false;
        
        if (k == 'q' || k == 27)
            capturing = false;

    } while (capturing);
    return 0;
}