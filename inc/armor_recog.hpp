#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define RED 1
#define BLUE 2
#define thres_max_color_red 50
#define thres_max_color_blue 100
#define BRIGHTNESS_THRES_RED 80
#define BRIGHTNESS_THRES_BLUE 120
#define Max(a,b) (((a) < (b)) ? (b) : (a))
#define Min(a,b) (((a) > (b)) ? (b) : (a))
#define Abs(a) ((a > 0)? (a): (-a))
#define square(a) ((a) * (a))
#define DELTA_ANGLE_THRESHOLD 25
#define STANDARD_LENGTH_WIDTH_RATIO 2.26 

using namespace cv;
using namespace std;

typedef struct {
    cv::RotatedRect bound;
    cv::Point2f center;
    float distance;
}Armor;

class Armor_recog {
    private:
    //
    public:
        int threshold_color;
        int threshold_brightness;
        int enemyColor;
        int sentry_mode;
        std::vector<Armor> armor_detected;
        Armor to_aim;
        cv::Mat current_frame;
        cv::Mat processed_frame;  
        std::vector<cv::RotatedRect> edges_detected;

        Armor_recog(int enemyColor, int sentry_mode);

        bool read_frame(cv::Mat frame);  //read frame in, return false if the frame is empty
        void pre_process();  //preprocess to emphasized the existing LED light bar
        void edge_detect();  //detects possible edges after preprocess, invalid edges like nearly horizontal ones and those with invalid length/width ratio
        bool armor_detect();
        static Armor edge_to_armor(cv::RotatedRect &a, cv::RotatedRect &b, double width);
        void showImage();
};