#pragma once
#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::ml;

static bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses );

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load);

static Ptr<TrainData>
prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples);

static void test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save);

static bool
build_rtrees_classifier( const string& data_filename,
                         const string& filename_to_save,
                         const string& filename_to_load );


