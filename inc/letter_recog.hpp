#pragma once
#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::ml;

bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses );

template<typename T>
Ptr<T> load_classifier(const string& filename_to_load);

Ptr<TrainData>
prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples);

void test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save);

bool build_rtrees_classifier( const string& data_filename,
                         const string& filename_to_save,
                         const string& filename_to_load );


