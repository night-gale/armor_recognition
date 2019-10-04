// #include <opencv2/ml.hpp>
// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>

// #include <cstdio>
// #include <vector>
// #include <iostream>

// using namespace cv;
// using namespace std;
// using namespace cv::ml;

// int main(void) {
//     int numSamples = 10;
//     int numfeatures = 2;

//     Mat trainingData(numSamples * 2, numfeatures * 2, CV_32FC1);
//     Mat trainingClassification(numSamples * 2, 1, CV_32FC1);

//     for(int i = 0; i < numSamples * 2; i += 2) {
//         trainingData.at<float>(i, 0) = 1.0f;
//         trainingData.at<float>(i, 1) = 0.0f;
//         trainingClassification.at<float>(i, 0) = 1.0f;

//         trainingData.at<float>(i + 1, 0) = 0.0f;
//         trainingData.at<float>(i + 1, 1) = 1.0f;
//         trainingClassification.at<float>(i + 1, 0) = 0.0f;
//     }

//     Mat var_type = Mat(numfeatures + 1, 1, CV_8U);
    
// }