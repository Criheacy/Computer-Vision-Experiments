#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const char *img_path = "11-1.png";
const char *img_template_path = "11-2.png";

Mat src_img, src_template;
Mat img_tp;
int alpha, beta, width, height;

void mergeImg(vector<Mat> &vec, Mat &dst, bool scale = true, int capability = 2, int maxNum = 6)
{
    //多张图片（至多maxNum个）合并为一张，每行capability个，scale = true时将图片按capability放缩
    int count = (int)vec.size();
    assert(count > 0 && count <= maxNum);
    int rowScale = count % capability == 0 ? count / capability : count / capability + 1;
    int colScale = min(capability, count);
    int maxCol = 0, maxRow = 0;
    for (auto &img : vec)
    {
        int col = img.cols, row = img.rows;
        if (scale)
            col /= colScale, row /= colScale;
        resize(img, img, Size(col, row));
        maxCol = max(col, maxCol), maxRow = max(row, maxRow);
    }
    dst.create(maxRow * rowScale, maxCol * colScale, vec[0].type());
    for (int i = 0; i < count; i++)
    {
        int x = (i % capability) * maxCol, y = (i / capability) * maxRow;
        vec[i].copyTo(dst(Rect(x, y, vec[i].cols, vec[i].rows)));
    }
}

void showImage(Mat &image, const char *windowName, int location_x = 0, int location_y = 0)
{
    //当窗口关闭时，getWindowProperty将返回-1
    if (getWindowProperty(windowName, WND_PROP_AUTOSIZE) == -1)
        namedWindow(windowName, WINDOW_AUTOSIZE);
    moveWindow(windowName, location_x, location_y);
    imshow(windowName, image);
}

void matching(Mat &src, Mat &temp, Mat &dst, int method)
{
    Mat src_gray, temp_gray;
    if (src.type() == CV_8UC3 || src.type() == CV_32FC3)
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
    else
        src.copyTo(src_gray);
    if (temp.type() == CV_8UC3 || temp.type() == CV_32FC3)
        cvtColor(temp, temp_gray, COLOR_BGR2GRAY);
    else
        temp.copyTo(temp_gray);

    //模板匹配
    Mat img_matched;
    matchTemplate(src_gray, temp_gray, img_matched, method);

    //寻找最佳匹配位置
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(img_matched, &minVal, &maxVal, &minLoc, &maxLoc);

    src.copyTo(dst);
    rectangle(dst, maxLoc, Point(maxLoc.x + temp.cols, maxLoc.y + temp.rows), Scalar(0, 0, 255), 5);
}

void brightnessController(int, void *)
{
    src_img.convertTo(img_tp, -1, alpha * 0.1, beta);

    Mat res_TM_SQDIFF, res_TM_CCOEFF_NORMED;
    matching(img_tp, src_template, res_TM_SQDIFF, TM_SQDIFF);
    matching(img_tp, src_template, res_TM_CCOEFF_NORMED, TM_CCOEFF_NORMED);

    vector<Mat> images(2);
    images[0] = res_TM_SQDIFF, images[1] = res_TM_CCOEFF_NORMED;
    Mat show;
    mergeImg(images, show, false);
    showImage(show, "change contrast and brightness", 600, 0);
}

void sizeController(int, void *)
{
    int col = width * 10 > src_template.cols ? width * 10 : src_img.cols;
    int row = height * 10 > src_template.rows ? height * 10 : src_img.rows;
    resize(src_img, img_tp, Size(col, row));

    Mat res_TM_SQDIFF, res_TM_CCOEFF_NORMED;
    matching(img_tp, src_template, res_TM_SQDIFF, TM_SQDIFF);
    matching(img_tp, src_template, res_TM_CCOEFF_NORMED, TM_CCOEFF_NORMED);

    vector<Mat> images(2);
    images[0] = res_TM_SQDIFF, images[1] = res_TM_CCOEFF_NORMED;
    Mat show;
    mergeImg(images, show, false);
    showImage(show, "change size", 600, 500);
}

int main()
{
    ios::sync_with_stdio(false);

    src_img = imread(img_path);
    src_template = imread(img_template_path);
    showImage(src_img, "original image", 100, 0);
    showImage(src_template, "template");

    alpha = 10, beta = 0;
    width = src_img.cols / 10, height = src_img.rows / 10;
    createTrackbar("alpha*0.1", "original image", &alpha, 100, brightnessController);
    createTrackbar("beta", "original image", &beta, 100, brightnessController);
    createTrackbar("width*10", "original image", &width, 100, sizeController);
    createTrackbar("height*10", "original image", &height, 100, sizeController);
    brightnessController(0, nullptr);
    sizeController(0, nullptr);

    waitKey(0);
    destroyAllWindows();

    return 0;
}