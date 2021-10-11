#include <opencv2/opencv.hpp>
#include <iostream>

#include "gaussian.hpp"

using namespace cv;
using namespace std;

Mat image;
String windowName;

int sigma;

void UpdateImage()
{
    Mat borderedImage, processedImage;
    int width = image.cols;
    int height = image.rows;
    int channels = image.channels();
    int padding = sigma * 3 - 1;    // border size = 6 * sigma - 1

    printf("sigma = %d\n", sigma);

    GaussianFilter* gaussianFilter = new GaussianFilter(sigma);

    double* temp = new double[(width + padding * 2) * (height + padding * 2) * channels];
    processedImage = image.clone();
    if (gaussianFilter->size == 0) {
        delete gaussianFilter;
        return;
    }

    cv::copyMakeBorder(image, borderedImage, padding, padding, padding, padding, cv::BORDER_DEFAULT);

    for(int i = 0; i < height + padding * 2; i++) {
        for (int j = padding; j < width + padding; j++) {
            for (int c = 0; c < channels; c++) {
                temp[(i * width + j) * channels + c] = 0;
                for (int offset = -padding; offset <= padding; offset++) {
                    temp[(i * width + j) * channels + c] +=
                            borderedImage.at<Vec3b>(i, j + offset)[c]
                            * gaussianFilter->kernel[offset + padding];
                }
            }
        }
    }

    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < channels; c++) {
                double result = 0;
                for (int offset = -padding; offset <= padding; offset++) {
                    result += temp[((i + padding + offset) * width + (j + padding)) * channels + c]
                            * gaussianFilter->kernel[offset + padding];
                }
                processedImage.at<Vec3b>(i, j)[c] = result;
            }
        }
    }

    delete gaussianFilter;
    imshow(windowName, processedImage); // Show our image inside the created window.
}

void onGaussianChanged(int value, void*)
{
    sigma = value + 1;
    UpdateImage();
}

int main(int argc, char** argv)
{
    // Read the image file
    image = imread("11.jpg");

    // Check for failure
    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    windowName = "11"; //Name of the window

    namedWindow(windowName); // Create a window

    sigma = 1;
    UpdateImage();
    imshow(windowName, image); // Show our image inside the created window.
    createTrackbar("Gaussian Value", windowName, nullptr, 100, onGaussianChanged);

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window

    return 0;
}