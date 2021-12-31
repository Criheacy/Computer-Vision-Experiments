#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const char *img_path = "horse_mask.png";
const int max_len = 1e+6;

Mat img;
int parent[max_len] = {0};

int Find(int x) {
    assert(x < max_len);
    int i = x;
    while (parent[i] != 0) i = parent[i];
    return i;
}

void Merge(int x, int y) {// x->y
    assert(x < max_len && y < max_len);
    int i = x, j = y;
    while (parent[i] != 0) i = parent[i];
    while (parent[j] != 0) j = parent[j];
    if (i != j) parent[i] = j;
}


Mat connected(Mat &src) {// 8连通的快速连通域算法
    int rows = src.rows, cols = src.cols;
    Mat processed = Mat::zeros(src.size(), src.type());

    Mat labels = Mat::zeros(rows, cols, CV_32SC1);
    int label = 0;
    // first pass
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (src.at<uchar>(y, x) != 0) {
                
                int top_left = (x - 1 < 0) || (y - 1 < 0) ? 0 : labels.at<int>(y - 1, x - 1);
                int top = y - 1 < 0 ? 0 : labels.at<int>(y - 1, x);
                int top_right = (y - 1 < 0) || (x + 1 > cols - 1) ? 0 : labels.at<int>(y - 1, x + 1);
                int left = x - 1 < 0 ? 0 : labels.at<int>(y, x - 1);

                vector<int> neighbours;
                neighbours.reserve(4);
                if (top_left != 0) neighbours.emplace_back(top_left);
                if (top != 0) neighbours.emplace_back(top);
                if (top_right != 0) neighbours.emplace_back(top_right);
                if (left != 0) neighbours.emplace_back(left);

                if (neighbours.empty()) labels.at<int>(y, x) = ++label;
                else {
                    sort(neighbours.begin(), neighbours.end());
                    int smallest = neighbours[0];
                    labels.at<int>(y, x) = smallest;
                    for (int i = 1; i < neighbours.size(); i++) Merge(neighbours[i], smallest);
                }
            }
        }
    }
    // second pass
    vector<int> counts(label, 0);// 计数
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (src.at<uchar>(y, x) != 0) labels.at<int>(y, x) = Find(labels.at<int>(y, x));
            counts[labels.at<int>(y, x)]++;
        }
    }

    auto maxLabel = max_element(counts.begin() + 1, counts.end()) - counts.begin();// label=0不能算
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (labels.at<int>(y, x) == maxLabel) processed.at<uchar>(y, x) = 255;
        }
    }

    string win2 = "connected component";
    namedWindow(win2, WINDOW_AUTOSIZE);
    moveWindow(win2, 1015, 200);
    imshow(win2, processed);

    return processed;
}

void distanceField(Mat &src) {// 距离场可视化
    int rows = src.rows, cols = src.cols;

    Mat distanceImg = Mat::zeros(rows, cols, CV_32FC1);
    distanceTransform(src, distanceImg, DIST_L2, 3);

    Mat normImg;
    normalize(distanceImg, normImg, 0, 1, NORM_MINMAX);

    string win3 = "distance field";
    namedWindow(win3, WINDOW_AUTOSIZE);
    imshow(win3, normImg);
}

int main() {
    ios::sync_with_stdio(false);

    img = imread(img_path, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {//binary
        for (int x = 0; x < img.cols; x++) {
            img.at<uchar>(y, x) = img.at<uchar>(y, x) <= 127 ? 0 : 255;
        }
    }

    string win1 = "origin";
    namedWindow(win1, WINDOW_AUTOSIZE);
    moveWindow(win1, 0, 200);
    imshow(win1, img);

    Mat testImg = connected(img);
    distanceField(testImg);

    waitKey(0);
    destroyAllWindows();

    return 0;
}