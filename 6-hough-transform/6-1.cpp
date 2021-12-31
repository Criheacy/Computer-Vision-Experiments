#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

string sourceImagePath = "dataset/39.jpg";
string sourceWindowName = "source";
string targetWindowName = "target";
Mat source;
Mat binary;
Mat edge;

void EdgeDetection(const Mat &binary, int thresholdParam1, int thresholdParam2)
{
    Canny(binary, edge, thresholdParam1, thresholdParam2);
    imshow("edge", edge);
}

void CircleDetection(const Mat &source, const Mat &binary, const Mat &edge,
                     int minRadius, int maxRadius,
                     int minCenterProbability)
{
    Mat dX, dY; //水平梯度和垂直梯度
    Sobel(binary, dX, CV_32F, 1, 0);
    Sobel(binary, dY, CV_32F, 0, 1);

    imshow("dX", dX);
    imshow("dY", dY);

    int height = binary.rows;
    int width = binary.cols;
    Mat centerProbability = Mat::zeros(height, width, CV_32SC1);
    vector<Point> circumferenceList;

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            // not on edge
            if (edge.at<uchar>(x, y) == 0) {
                continue;
            }
            circumferenceList.push_back(Point(x, y));

            double currentdX = dX.at<float>(x, y);
            double currentdY = dY.at<float>(x, y);
            double k = atan(currentdX / currentdY);

            for (int r = minRadius; r <= maxRadius; r += 1) {
                for (double theta = -0.1; theta <= 0.1; theta += 0.05) {
                    // angle tolerance
                    int rX = r * cvRound(cos(k) + theta);
                    int rY = r * cvRound(sin(k) + theta);

                    if (0 <= x + rX && x + rX < height && 0 <= y + rY && y + rY < width) {
                        centerProbability.at<int>(x + rX, y + rY)++;
                    }
                    if (0 <= x - rX && x - rX < height && 0 <= y - rY && y - rY < width) {
                        centerProbability.at<int>(x - rX, y - rY)++;
                    }
                }
            }
        }
    }

    Mat centerProbabilityTemp;
    centerProbability.convertTo(centerProbabilityTemp, CV_64FC1);
    
    for (int x = 0; x < height; x++)
        for (int y = 0; y < width; y++)
            centerProbabilityTemp.at<double>(x, y) /= 25.0;
    imshow("temp", centerProbabilityTemp);

    vector<Point> centerCandidates;
    // filter possible centers
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            int tX[] = {-1, 0, 1, 0};
            int tY[] = {0, -1, 0, 1};
            bool qualified = true;
            if (centerProbability.at<int>(x, y) < minCenterProbability) {
                qualified = false;
            }
            for (int t = 0; t < 4; t++) {
                if (x + tX[t] >= 0 && x + tX[t] < height && y + tY[t] >= 0 && y + tY[t] < width) {
                    if (centerProbability.at<int>(x, y) < centerProbability.at<int>(x + tX[t], y + tY[t])) {
                        qualified = false;
                    }
                }
            }
            if (qualified) {
                centerCandidates.push_back(Point(x, y));
            }
        }
    }

    // get average radius and draw circles
    Mat detection;
    source.copyTo(detection);
    for (auto& center: centerCandidates) {
        int supportersCount = 0;
        double supportersRadiusAvg = 0;
        for (auto& circumference: circumferenceList) {
            double distance = sqrt(pow(circumference.x - center.x, 2) + pow(circumference.y - center.y, 2));
            if (distance >= minRadius && distance <= maxRadius) {
                supportersCount++;
                supportersRadiusAvg += distance;
            }
        }
        supportersRadiusAvg /= supportersCount;
        circle(detection, center, supportersRadiusAvg, Scalar(0, 0, 255));
    }

    imshow(sourceWindowName, detection);
}

int minRadiusRaw, maxRadiusRaw, detectTolerance, thresholdParam1, thresholdParam2;

void onTrackBarChanged(int, void*) {
    EdgeDetection(binary, thresholdParam1, thresholdParam2);
    CircleDetection(source, binary, edge,
        minRadiusRaw, maxRadiusRaw, detectTolerance);
}

int main(int argc, char **argv)
{
    // Read the image file
    source = imread(sourceImagePath);

    // Check for failure
    if (source.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    cvtColor(source, binary, cv::COLOR_RGB2GRAY);
    GaussianBlur(binary, binary, Size(9, 9), 2, 2);

    namedWindow(sourceWindowName); // Create a window

    imshow(sourceWindowName, source);

    minRadiusRaw = 10;
    maxRadiusRaw = 30;
    detectTolerance = 100;
    thresholdParam1 = 115;
    thresholdParam2 = 300;
    createTrackbar("ThresholdParam1", sourceWindowName, &thresholdParam1, 1000, onTrackBarChanged);
    createTrackbar("ThresholdParam2", sourceWindowName, &thresholdParam2, 1000, onTrackBarChanged);
    createTrackbar("Min Radius", sourceWindowName, &minRadiusRaw, 200, onTrackBarChanged);
    createTrackbar("Max Radius", sourceWindowName, &maxRadiusRaw, 200, onTrackBarChanged);
    createTrackbar("Detect Tolerance", sourceWindowName, &detectTolerance, 100, onTrackBarChanged);

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(sourceWindowName); //destroy the created window
    return 0;
}