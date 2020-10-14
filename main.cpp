#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

Mat frame;
Mat bord_frame;

int main()
{
    int ksize = 5;

    VideoCapture cap("solidYellowLeft.mp4");

    if(!cap.isOpened()) {
        cout << "Error" << endl;
        return -1;
    }

    namedWindow("Params", WINDOW_AUTOSIZE);
    int WidthUpPlane = 72;
    createTrackbar("Width up plane", "Params", &WidthUpPlane, 100);
    int HeigthUpPlane = 77;
    createTrackbar("Heigth up plane", "Params", &HeigthUpPlane, 100);
    int WidthLowPlane = 88;
    createTrackbar("Width low plane", "Params", &WidthLowPlane, 100);
    int HeigthLowPlane = 91;
    createTrackbar("Heigth low plane", "Params", &HeigthLowPlane, 100);

    while(1) {

        Mat blur_frame;
        Mat red_frame;
        Mat gray_frame;

        cap >> frame;

        if(frame.empty()) {
            cout << "Frame empty" << endl;
            break;
        }

//        namedWindow("Tracbar test", WINDOW_AUTOSIZE);

//        alpha_slider = 0;



//        blur(frame, blur_frame, Size(ksize, ksize), Point(-1,-1));
//        cvtColor(frame, red_frame, COLOR_BGR2HSV);
//        inRange(red_frame, Scalar(170, 70, 50), Scalar(180, 255, 255), red_frame);
//        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
//        bord_frame = gray_frame.clone();
//        Canny(gray_frame, bord_frame, 10, 100, 3);

//        char TrackbarName[50];
//        sprintf( TrackbarName, "Alpha x %d", alpha_slider_max );
//        createTrackbar( TrackbarName, "Tracbar test", &alpha_slider, alpha_slider_max, on_trackbar);
//        on_trackbar( alpha_slider, 0 );

        float rows = frame.rows;
        float cols = frame.cols;

//        float p1_x = cols/100.0*WidthLowPlane;
//        float p14_y = rows/100.0*HeigthLowPlane;
//        float p4_x = cols/100.0*(100.0 - WidthLowPlane);

//        float p2_x = cols/100.0*WidthUpPlane;
//        float p23_y = rows/100.0*HeigthUpPlane;
//        float p3_x = cols/100.0*(100.0 - WidthUpPlane);

        float p4_x = cols/100.0*WidthLowPlane;
        float p43_y = rows/100.0*HeigthLowPlane;
        float p3_x = cols/100.0*(100.0 - WidthLowPlane);

        float p1_x = cols/100.0*WidthUpPlane;
        float p12_y = rows/100.0*HeigthUpPlane;
        float p2_x = cols/100.0*(100.0 - WidthUpPlane);

        vector<Point> polyline;
        polyline.push_back(Point(p1_x, p12_y));
        polyline.push_back(Point(p2_x, p12_y));
        polyline.push_back(Point(p3_x, p43_y));
        polyline.push_back(Point(p4_x, p43_y));

        vector<Point2f> points;
        points.push_back(Point2f(p1_x, p12_y));
        points.push_back(Point2f(p2_x, p12_y));
        points.push_back(Point2f(p3_x, p43_y));
        points.push_back(Point2f(p4_x, p43_y));

        vector<Point2f> dst_points;
        dst_points.push_back(Point2f(640.0, 0.0));
        dst_points.push_back(Point2f(0.0, 0.0));
        dst_points.push_back(Point2f(0.0, 480.0));
        dst_points.push_back(Point2f(640.0, 480.0));

        double fps = cap.get(CAP_PROP_FPS);
        int time_mls = cap.get(CAP_PROP_POS_MSEC);
        cout << "FPS: " << fps << " Time(mls): " << time_mls << endl;
        Mat farame2poly = frame.clone();
        polylines(farame2poly, polyline, 1, Scalar(255, 0, 0), 4);

        Mat Matrix = getPerspectiveTransform(points, dst_points);
        Mat dst;
        warpPerspective(frame, dst, Matrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);

        imshow("Frame", farame2poly);
        imshow("Bird", dst);
//        imshow("Blur frame", blur_frame);
//        imshow("Red frame", red_frame);
//        imshow("Canny frame", bord_frame);

        char c=(char)waitKey(25);
        if(c==27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
