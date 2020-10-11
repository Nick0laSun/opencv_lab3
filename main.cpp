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

    VideoCapture cap("video.mp4");

    if(!cap.isOpened()) {
        cout << "Error" << endl;
        return -1;
    }

    namedWindow("Params", WINDOW_AUTOSIZE);
    int SliderValue1 = 59;
    createTrackbar("Width up plane", "Params", &SliderValue1, 100);
    int SliderValue2 = 74;
    createTrackbar("Heigth up plane", "Params", &SliderValue2, 100);
    int SliderValue3 = 82;
    createTrackbar("Width low plane", "Params", &SliderValue3, 100);
    int SliderValue4 = 98;
    createTrackbar("Heigth low plane", "Params", &SliderValue4, 100);

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



        blur(frame, blur_frame, Size(ksize, ksize), Point(-1,-1));
        cvtColor(frame, red_frame, COLOR_BGR2HSV);
        inRange(red_frame, Scalar(170, 70, 50), Scalar(180, 255, 255), red_frame);
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
        bord_frame = gray_frame.clone();
        Canny(gray_frame, bord_frame, 10, 100, 3);

//        char TrackbarName[50];
//        sprintf( TrackbarName, "Alpha x %d", alpha_slider_max );
//        createTrackbar( TrackbarName, "Tracbar test", &alpha_slider, alpha_slider_max, on_trackbar);
//        on_trackbar( alpha_slider, 0 );

        int rows = frame.rows;
        int cols = frame.cols;

        double p1_x = cols/100*SliderValue3;
        double p14_y = rows/100*SliderValue4;
        double p4_x = cols/100*(100 - SliderValue3);

        double p2_x = cols/100*SliderValue1;
        double p23_y = rows/100*SliderValue2;
        double p3_x = cols/100*(100 - SliderValue1);

        vector<Point> points;
        points.push_back(Point(p1_x, p14_y));
        points.push_back(Point(p2_x, p23_y));
        points.push_back(Point(p3_x, p23_y));
        points.push_back(Point(p4_x, p14_y));

        vector<Point> dst_points;
        dst_points.push_back(Point(0, 0));
        dst_points.push_back(Point(0, 480));
        dst_points.push_back(Point(640, 480));
        dst_points.push_back(Point(640, 0));


        polylines(frame, points, 1, Scalar(255, 0, 0), 4);

        Mat M = getPerspectiveTransform(points, dst_points);
        Mat dst;
        warpPerspective(frame, dst, M, dst.size(), INTER_LINEAR, BORDER_CONSTANT);

        imshow("Frame", frame);
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
