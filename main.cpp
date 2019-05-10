#include <iostream>
#include "opencv2/opencv.hpp"
#include "qrcode.hpp"

using namespace std;
using namespace cv;

static void drawQRCodeContour(Mat &color_image, vector<Point> transform);
static void drawFPS(Mat &color_image, double fps);
static int imageQRCodeDetect(const string& in_file, const string& out_file);

int main()
{
    cout << "Hello World!" << endl;

    string input_file_name = "../image/1.jpg";
    string output_file_name = "../image/result.jpg";
    int return_code = 0;

    return_code = imageQRCodeDetect(samples::findFile(input_file_name), output_file_name);
    return return_code;
}

void drawQRCodeContour(Mat &color_image, vector<Point> transform)
{
    if (!transform.empty())
    {
        double show_radius = (color_image.rows > color_image.cols)
                             ? (2.813 * color_image.rows) / color_image.cols
                             : (2.813 * color_image.cols) / color_image.rows;
        double contour_radius = show_radius * 0.4;

        vector< vector<Point> > contours;
        contours.push_back(transform);
        drawContours(color_image, contours, 0, Scalar(211, 0, 148), cvRound(contour_radius));

        RNG rng(1000);
        for (size_t i = 0; i < 4; ++i)
        {
            Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
            circle(color_image, transform[i], cvRound(show_radius), color, -1);
        }
    }
}

void drawFPS(Mat &color_image, double fps)
{
    ostringstream convert;
    convert << cvRound(fps) << " FPS (QR detection)";
    putText(color_image, convert.str(), Point(25, 25), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);
}

int imageQRCodeDetect(const string& in_file, const string& out_file)
{
    Mat color_src = imread(in_file, IMREAD_COLOR), src;
    cvtColor(color_src, src, COLOR_BGR2GRAY);
    Mat straight_barcode;
    string decoded_info;
    vector<Point> transform;
    const int count_experiments= 10;
    double transform_time = 0.0;
    bool result_detection = false;
    TickMeter total;
    myqr::QRCodeDetector qrcode;
    for (size_t i = 0; i < count_experiments; ++i)
    {
        total.start();
        transform.clear();
        result_detection = qrcode.detect(src, transform);
        total.stop();
        transform_time += total.getTimeSec();
        total.reset();
        if(!result_detection)
            continue;

        total.start();
        decoded_info = qrcode.decode(src, transform, straight_barcode);
        total.stop();
        transform_time += total.getTimeSec();
        total.reset();
    }
    double fps = count_experiments / transform_time;
    if (!result_detection)
        cout << "QR code not found" << endl;
    if (decoded_info.empty())
        cout << "QR code cannot be decoded" << endl;

    drawQRCodeContour(color_src, transform);
    drawFPS(color_src, fps);

    cout << "Input  image file path: " << in_file << endl;
    cout << "Output image file path: " << out_file << endl;
    cout << "Size: " <<color_src.size() << endl;
    cout << "FPS: " << fps << endl;
    cout << "Decoded info: " << decoded_info << endl;

    if (!out_file.empty())
    {
        imwrite(out_file, color_src);
    }

    for (;;)
    {
        imshow("Detect QR code on image", color_src);
        if (waitKey(0) == 27) //ASCII code 27 refers to ESC key. Focus must be on the show window when press ESC.
            break;
    }
    return 0;
}