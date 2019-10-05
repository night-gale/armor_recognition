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
void train_Rtree(char* filename_dataset, int number_of_video, const string& data_filename, const string& data_filename_to_save);
cv::Mat decrease_to_size(Mat& src, int size);

int main(void) {
    char filename[14] = "./video/0.mp4";
    string data_filename = "./trainData/database.dat";
    string data_filename_to_save = "./model/numberRecog.model"; 
    Mat src = imread("./img/numbers/five1.jpg");
    Mat dst = pre_processing(src);
    dst = decrease_to_size(dst, 8);
    Mat test_data;
    float r;
    for(int i = 0; i < dst.cols; i++) {
        for(int j = 0; j < dst.rows; j++) {
            test_data.push_back((int)dst.data[i*dst.cols + j] / 255);
        }
    }

    Ptr<RTrees> model = load_classifier<RTrees>(data_filename_to_save);

    r = model->predict(test_data);
    printf("%f", r);
}


cv::Mat pre_processing(const Mat &src) {
    Mat dst = src.clone();
    vector<vector<Point2i>> contour_result;
    vector<Vec4i> hierarchy;
    vector<RotatedRect> rotated_rect;
    cvtColor(dst, dst, COLOR_RGB2GRAY);
    threshold(dst, dst, 130, 255, THRESH_BINARY_INV);   //highlight the background of the numbers
    

    // namedWindow("threshold", WINDOW_FREERATIO);
    // imshow("threshold", dst);
    // waitKey(0);

    //get the rotateRectangle, which include the number, here has the biggest area
    findContours(dst, contour_result, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);   
    
    for(size_t i = 0; i < contour_result.size(); i++) {
        RotatedRect rRect = minAreaRect(contour_result[i]);
        if(rRect.size.area() > 250) {
            rotated_rect.push_back(rRect);
        }
    }

//sort descending, according to area of the rotateRect
    sort(rotated_rect.begin(), rotated_rect.end(), 
        [](RotatedRect &a1, RotatedRect &a2) {
        return a1.size.area() < a2.size.area();});

    rectangle(dst, rotated_rect[0].boundingRect(), Scalar(255, 0, 0), 5);
    // imshow("threshold", dst);
    // waitKey(0);
    return dst(rotated_rect[0].boundingRect());
}

cv::Mat decrease_to_size(Mat& src, int size) {
    float temp = log2f(min(src.rows, src.cols) / size);
    int temp1 = temp;
    float temp2 = temp1 + 1 - temp;
    int newSize = size * powf(2, temp1);
    //decrease the size of the photo to size x size
    Mat partial = Mat(src, Rect2i(Point2i((src.cols - newSize) / 2, 
                                        (src.rows - newSize) / 2), 
                                        Point2i((src.cols + newSize) / 2, 
                                        (src.rows + newSize) / 2)));

    for(int i = 0; i < temp1; i++) {
        pyrDown(partial, partial, Size2i(partial.cols / 2, partial.rows / 2));
    }
    threshold(partial, partial, 130, 255, THRESH_BINARY_INV);
    return partial;
}

void write(const Mat toWrite, int tag) {
    std::ofstream outDataBase;
    std::ofstream outResponse;
    outDataBase.open("./trainData/database.dat");
    outDataBase << tag <<' ';
    for(size_t i = 0; i < toWrite.cols; i++) {
        for(size_t j = 0; j < toWrite.rows; j++) {
            outDataBase << (int)toWrite.data[i*toWrite.cols + j] / 255 << ' ';
            std::cout << (int)(toWrite.data[i*toWrite.cols + j]) / 255 << ' ';
        }
    }
    outDataBase << std::endl;
    
}

void train_rTree(char* filename_dataset, int number_of_video, const string& data_filename, const string& data_filename_to_save) {
        for(int counter = 1; counter <= number_of_video; counter++) {
        filename_dataset[number_of_video] = counter + 48;
        VideoCapture cap(filename_dataset);
        Mat src;
        Mat dst;
        char key;
        namedWindow("test", WINDOW_FREERATIO);

        while(true) {
            key = waitKey(0);
            cap >> src;
            if(src.empty() || key == 27) {
                break;
            }

            dst = pre_processing(src);
            dst = decrease_to_size(dst, 8);

            imshow("test", dst);
            key = waitKey(0);

            if(key != 'd') {
                write(dst, counter);
            }
        }
    }
    build_rtrees_classifier(data_filename, data_filename_to_save, NULL);
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