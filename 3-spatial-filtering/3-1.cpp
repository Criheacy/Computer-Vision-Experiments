#include <opencv2/opencv.hpp>
#include <iostream>

#include "gaussian.hpp"

using namespace cv;
using namespace std;

const int MAX_PADDING = 12;

Mat image;
String windowName;

double* imageSum;
int borderedWidth;
int borderedHeight;
int channels;

int sigma;
int normal;

inline double& at(int x, int y, int c = 0) {
    return imageSum[(x * borderedWidth + y) * channels + c];
}

void InitializeNormalSum() {
    if (imageSum != nullptr) {
        delete imageSum;
        imageSum = nullptr;
    }
    Mat borderedImage;
    cv::copyMakeBorder(image, borderedImage, MAX_PADDING, MAX_PADDING, MAX_PADDING, MAX_PADDING,
                       cv::BORDER_DEFAULT);

    borderedWidth = borderedImage.cols;
    borderedHeight = borderedImage.rows;
    channels = borderedImage.channels();

    imageSum = new double[borderedImage.rows * borderedImage.cols * borderedImage.channels()];
    for (int i = 0; i < borderedHeight; i++) {
        for (int j = 0; j < borderedWidth; j++) {
            for (int c = 0; c < channels; c++) {
                at(i, j, c) = borderedImage.at<Vec3b>(i, j)[c];

                if (i != 0) {
                    at(i, j, c) += at(i - 1, j, c);
                }
                if (j != 0) {
                    at(i, j, c) += at(i, j - 1, c);
                }
                if (i != 0 && j != 0) {
                    at(i, j, c) -= at(i - 1, j - 1, c);
                }
            }
        }
    }
}

void UpdateGaussianImage()
{
    Mat borderedImage, processedImage;
    int width = image.cols;
    int height = image.rows;
    int padding = sigma * 3 - 1;    // border size = 6 * sigma - 1

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

void UpdateNormalImage() {
    Mat processedImage;
    int padding = normal;
    int width = image.cols;
    int height = image.rows;

    processedImage = image.clone();

    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < channels; c++) {
                int _i = i + MAX_PADDING;
                int _j = j + MAX_PADDING;
                processedImage.at<Vec3b>(i, j)[c] = (at(_i + padding, _j + padding, c)
                        - at(_i - padding - 1, _j + padding, c)
                        - at(_i + padding, _j - padding - 1, c)
                        + at(_i - padding - 1, _j - padding - 1, c))
                                / (std::pow(padding * 2 + 1, 2));

            }
        }
    }

    imshow(windowName, processedImage); // Show our image inside the created window.
}

void onGaussianChanged(int value, void*)
{
    sigma = value / 10 + 1;
    UpdateGaussianImage();
}

void onNormalChanged(int value, void*)
{
    normal = value / 10 + 1;
    UpdateNormalImage();
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
    normal = 1;

    InitializeNormalSum();
    imshow(windowName, image); // Show our image inside the created window.
    createTrackbar("Gaussian Value", windowName, nullptr, 100, onGaussianChanged);
    createTrackbar("Normal Value", windowName, nullptr, 100, onNormalChanged);

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window

    return 0;
}