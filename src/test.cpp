#include <armor_recog.hpp>
#include <stdio.h>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;

int main(void) {
    VideoCapture cap;
    cap.open("./video/red.mp4");
    if(!cap.isOpened()) {
        printf("no video captured");
        return -1;
    }
    while(waitKey(30) != 'q') {
        Mat test;
        cap >> test;
        if(test.empty()) {
            printf("no frame remained");
            break;
        }
        Armor_recog ar(RED, 0);
        ar.read_frame(test);
        ar.pre_process();
        ar.edge_detect();
        ar.armor_detect();
        ar.showImage();
    }
}