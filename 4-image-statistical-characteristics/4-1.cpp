#include <opencv2/opencv.hpp>
#include <iostream>

// #include "tracker.hpp"

const std::string VIDEO_INPUT_PATH = "theshy.mp4";
const std::string VIDEO_OUTPUT_PATH = "theshy_processed.mp4";

const std::string WINDOW_VIDEO_NAME = "video";

int main(int argc, char** argv)
{
    cv::VideoCapture capture(VIDEO_INPUT_PATH);

    if (!capture.isOpened())
    {
        std::cout << "Video " + VIDEO_INPUT_PATH + " cannot open!" << std::endl;
        return -1;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);

    cv::Mat frame;
    while (true) {
        capture >> frame;

        if (frame.empty()) {
            break;
        }

        if (cv::waitKey(1000 / fps) == 27) {    // ESC
            break;
        }

        cv::imshow(WINDOW_VIDEO_NAME, frame);
    }

    return 0;
}