#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include<cmath>

using namespace cv;
using namespace std;

class angleSolver
{
private:
    double H = 0.5;
    double L = 0.01; 
    double Co = 0.000001; 
    double g = 9.7883;
    double Miu = 0.00001;
    double Z = 0.5;
    double V = 8;
    double X = 0;
    double Y = 5;
    double X_velocity = 3; // to the right 
    double Y_velocity = 3; // to the front
    double select_alpha(double dis);
    double get_thet(double X, double Y, double X_velocity ,double Y_velocity, double thet0);
    double dis = sqrt(X*X+Y*Y);    
    double get_para_t(double vx0,double dis,double alpha);
    double get_para_height(double vx0, double vy0, double dis, double alpha);
    double get_str_v(double alpha);
    double get_height(double alpha,double dis);
    

public:
    double get_ALPHA();
    double get_THET();
    void setXYZ(double X, double Y, double Z );
    void setRelativeV(double Vx, double Vy);
};


