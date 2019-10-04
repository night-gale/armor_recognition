#include <armor_recog.hpp>
#include <stdio.h>
#include <fstream>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <letter_recog.hpp>

using namespace cv;
using namespace std;

Mat pre_processing(const Mat &src);
void write(const Mat toWrite, int tag);

int main(void) {
    Mat src = imread("./img/numbers/five1.jpg");
    Mat dst = pre_processing(src);
    float temp = log2f(min(dst.rows, dst.cols) / 8);
    int temp1 = temp;
    float temp2 = temp1 + 1 - temp;
    int newSize = 8 * powf(2, temp1);
    namedWindow("test", WINDOW_FREERATIO);

    Mat partial(dst, Rect2i(Point2i((dst.cols - newSize) / 2, (dst.rows - newSize) / 2), Point2i((dst.cols + newSize) / 2, (dst.rows + newSize) / 2)));
    for(int i = 0; i < temp1; i++) {
        pyrDown(partial, partial, Size2i(partial.cols / 2, partial.rows / 2));
    }
    threshold(partial, partial, 130, 255, THRESH_BINARY_INV);
    imshow("test", partial);
    printf("%d, %d\n", partial.rows, partial.cols);
    waitKey(0);
}


cv::Mat pre_processing(const Mat &src) {
    Mat dst = src.clone();
    vector<vector<Point2i>> contour_result;
    vector<Vec4i> hierarchy;
    vector<RotatedRect> rotated_rect;
    cvtColor(dst, dst, COLOR_RGB2GRAY);
    threshold(dst, dst, 130, 255, THRESH_BINARY_INV);
    

    namedWindow("threshold", WINDOW_FREERATIO);
    imshow("threshold", dst);
    waitKey(0);
    findContours(dst, contour_result, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    for(size_t i = 0; i < contour_result.size(); i++) {
        RotatedRect rRect = minAreaRect(contour_result[i]);
        if(rRect.size.area() > 250) {
            rotated_rect.push_back(rRect);
        }
    }

    sort(rotated_rect.begin(), rotated_rect.end(), [](RotatedRect &a1, RotatedRect &a2) {
        return a1.size.area() < a2.size.area();
    });

    rectangle(dst, rotated_rect[0].boundingRect(), Scalar(255, 0, 0), 5);
    imshow("threshold", dst);
    waitKey(0);
    return dst(rotated_rect[0].boundingRect());
}

void write(const Mat toWrite, int tag) {
    std::ofstream outDataBase;
    std::ofstream outResponse;
    outDataBase.open("./trainData/database.dat");
    outResponse.open("./trainData/response.dat");
    for(size_t i = 0; i < toWrite.cols; i++) {
        for(size_t j = 0; j < toWrite.rows; j++) {
            outDataBase << (int)toWrite.data[i*toWrite.cols + j] / 255 << " ";
            std::cout << (int)(toWrite.data[i*toWrite.cols + j]) / 255 << " ";
        }
    }
    outDataBase << std::endl;
    outResponse << tag <<" "<<std::endl;
}

// int main(void) {
//     VideoCapture cap;
//     cap.open("./video/red.mp4");
//     if(!cap.isOpened()) {
//         printf("no video captured");
//         return -1;
//     }
//     while(waitKey(30) != 'q') {
//         Mat test;
//         cap >> test;
//         if(test.empty()) {
//             printf("no frame remained");
//             break;
//         }
//         Armor_recog ar(RED, 0);
//         ar.read_frame(test);
//         ar.pre_process();
//         ar.edge_detect();
//         ar.armor_detect();
//         ar.showImage();
//     }
// }