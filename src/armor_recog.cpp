/*
TODO: find the proper light exposure
TODO: delta_angle in armor_detect(), to be modified
*/
#include <armor_recog.hpp>

using namespace std;
using namespace cv;

Armor_recog::Armor_recog(int enemyColor, int sentry_mode)
{
    Armor_recog::enemyColor = enemyColor;
    Armor_recog::sentry_mode = sentry_mode;
    if (enemyColor == RED)
    {
        threshold_color = thres_max_color_red;
        threshold_brightness = BRIGHTNESS_THRES_RED;
    }
    else if (enemyColor == BLUE)
    {
        threshold_color = thres_max_color_blue;
        threshold_brightness = BRIGHTNESS_THRES_BLUE;
    }
}

bool Armor_recog::read_frame(cv::Mat frame)
{
    current_frame = frame;
    if (current_frame.empty())
    {
        return false;
    }
    return true;
}

void Armor_recog::pre_process()
{
    Mat dilateElement = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    Mat thres_whole;
    Mat color;

    vector<Mat> splited;
    split(current_frame, splited);
    cvtColor(current_frame, thres_whole, COLOR_BGR2GRAY);
    threshold(thres_whole, thres_whole, threshold_brightness, 255, THRESH_BINARY); //binary-lize the image
    // imshow("origin", current_frame);
    // imshow("thresh_whole", thres_whole);

    if (enemyColor == BLUE)
    {
        subtract(splited[0], splited[2], color); //subtract the Blue channel with Red channel
    }
    else
    {
        subtract(splited[2], splited[0], color);
    }
    // imshow("subtracted1", color);
    threshold(color, color, threshold_color, 255, THRESH_BINARY); //binary-lize to obtain the blue area
    // imshow("subtracted2", color);
    dilate(color, color, dilateElement); //dilate, enlarge the bright area of the input image, which is the blue area
    // imshow("dilated", color);

    processed_frame = color & thres_whole; //use & logic operator to get rid of area that is bright but not the target color
    dilate(processed_frame, processed_frame, dilateElement);
    // imshow("dilated again", processed_frame);
}

void Armor_recog::edge_detect()
{
    vector<vector<Point2i>> contours_max;
    vector<Vec4i> hierarchy;

    //linear regression and classification, fit multiple points into one rotate Rectangle
    //that is, find the boundaries
    findContours(processed_frame, contours_max, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours_max.size(); i++)
    {
        RotatedRect rRect = minAreaRect(contours_max[i]);
        double rRect_length = MAX(rRect.size.width, rRect.size.height);
        double rRect_width = MIN(rRect.size.width, rRect.size.height);
        //fiter out those are not likely a RGB LED bar
        bool isWidthValid = (rRect_width > 5);
        bool isAProperRect = (rRect_length / rRect_width >= 1.1) && ((rRect_length / rRect_width) < 20);
        bool NotHorizontal1 = ((Abs(rRect.angle) < 45) && (rRect.size.height > rRect.size.width));
        bool NotHorizontal2 = ((Abs(rRect.angle) > 45) && (rRect.size.height < rRect.size.width));
        if (isWidthValid && (NotHorizontal1 || NotHorizontal2) && isAProperRect)
            edges_detected.push_back(rRect);
    }
}

bool Armor_recog::armor_detect()       
{
    if (edges_detected.size() < 2)
    {
        cout << "not target detected\n";
        return false;
    }
    else
    {
        for (int i = 0; i < edges_detected.size() - 1; i++)
        {
            //RotatedRect.points, the array start with the point with smallest y value, and goes clockwise
            Point2f pointsI[4];
            Point2f pointsJ[4];
            edges_detected[i].points(pointsI);
            double lengthI = Max(edges_detected[i].size.width, edges_detected[i].size.height);
            double widthI = Min(edges_detected[i].size.width, edges_detected[i].size.width);
            double angelI = abs(edges_detected[i].angle);
            double deltaAngle;

            for (int j = i + 1; j < edges_detected.size(); j++)
            {
                edges_detected[j].points(pointsJ);
                double lengthJ = Max(edges_detected[j].size.width, edges_detected[j].size.height);
                double widthJ = Min(edges_detected[j].size.width, edges_detected[j].size.width);
                double angelJ = abs(edges_detected[j].angle);
                double distance = sqrtf64(square(edges_detected[i].center.x - edges_detected[j].center.x) + square(edges_detected[i].center.y - edges_detected[j].center.y));
                double length_panel = (lengthI + lengthJ) / 2;
                double length_width_ratio_panel = distance / length_panel;
                // //get the angle of the line between center of two edges to the horizon
                // double angle_center = abs(atan2(edges_detected[i].center.y - edges_detected[j].center.y, edges_detected[i].center.x - edges_detected[j].center.x));
                // angle_center = angle_center * 180 / CV_PI;
                // //get the angle of first edge to horizon
                // double angle_edge = abs(atan2(pointsI[0].y - edges_detected[i].center.y, pointsI[0].x - edges_detected[i].center.x));
                // angle_edge = angle_edge * 180 / CV_PI;

                // imshow("thresh_whole", thres_whole);(angelI > 45 && angelJ < 45)
                if (angelI > 45 && angelJ < 45)
                {
                    deltaAngle = abs(abs(90 - angelI) - angelJ);
                }
                else if (angelI < 45 && angelJ > 45)
                {
                    deltaAngle = abs(abs(90 - angelI) - angelJ);
                }
                else
                {
                    deltaAngle = abs(angelI - angelJ);
                }

                // bool isLengthWidthVertical = (angle_edge + angle_center) > 45 && (angle_edge + angle_center) < 135;
                bool isTwoEdgeAtSameLevel =  ((pointsI[2].y < pointsJ[0].y) && (pointsI[0].y > pointsJ[2].y));
                bool isDeltaAngleValid = deltaAngle < DELTA_ANGLE_THRESHOLD;
                bool isAPanel = (length_width_ratio_panel > STANDARD_LENGTH_WIDTH_RATIO * 0.5) && (length_width_ratio_panel < STANDARD_LENGTH_WIDTH_RATIO * 1.6);
                if (isDeltaAngleValid && isAPanel && isTwoEdgeAtSameLevel)
                {
                    Armor temp = edge_to_armor(edges_detected[i], edges_detected[j], distance);
                    armor_detected.push_back(temp);
                }
            }
        }
    }
}

Armor Armor_recog::edge_to_armor(cv::RotatedRect &a, cv::RotatedRect &b, double width)
{
    Point centerA = a.center, centerB = b.center;
    Point2f center = (centerA + centerB) / 2.0;

    double widthA = Min(a.size.width, a.size.height);
    double lengthA = Max(a.size.width, a.size.height);
    double widthB = Min(b.size.width, b.size.height);
    double lengthB = Min(b.size.width, b.size.height);
    float angle = std::atan2(centerA.y - centerB.y, centerA.x - centerB.x);
    float height = Max(lengthA, lengthB);
    Armor armor;
    armor.bound = RotatedRect(center, Size2f(width, height), angle * 180 / CV_PI);
    armor.center = center;
    armor.barLen = (lengthA + lengthB) / 2;
    armor.distance = -1;
    return armor;
}

void Armor_recog::showImage()
{
    Mat test;
    current_frame.copyTo(test);
    for (size_t i = 0; i < edges_detected.size(); i++)
    {
        Point2f vertices[4];
        edges_detected[i].points(vertices);
        for (int j = 0; j < 4; j++)
        {
            line(test, vertices[j], vertices[(j + 1) % 4], Scalar(255, 0, 0), 5);
            // imshow("test", test);
            // waitKey(0);
        }
    }
    for (size_t i = 0; i < armor_detected.size(); i++)
    {
        Point2f vertices[4];
        armor_detected[i].bound.points(vertices);
        for (int j = 0; j < 4; j++)
        {
            line(test, vertices[j], vertices[(j + 1) % 4], Scalar(255, 0, 0), 5);
            // imshow("test", test);
            // waitKey(0);
        }
    }
    namedWindow("test", WINDOW_FREERATIO);
    imshow("test", test);
}