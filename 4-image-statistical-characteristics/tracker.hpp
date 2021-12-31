#include <opencv2/opencv.hpp>
#include <cmath>

class GaussianFilter {
public:
	static double Gaussian(double sigma, double offsetX);
	static double Gaussian(double sigma, double offsetX, double offsetY);

    GaussianFilter(double sigma);

    double sigma;
    int size;
    double* kernel;
};

double GaussianFilter::Gaussian(double sigma, double offsetX) {
    return std::exp(-(std::pow(offsetX, 2)) / (2 * std::pow(sigma, 2)))
           / (std::sqrt(2 * M_PI) * sigma);
}

double GaussianFilter::Gaussian(double sigma, double offsetX, double offsetY) {
	return std::exp(-(std::pow(offsetX, 2) + std::pow(offsetY, 2)) / (2 * std::pow(sigma, 2)))
	        / (2 * M_PI * std::pow(sigma, 2));
}

GaussianFilter::GaussianFilter(double sigma) {
    // 1-dimension
    this->sigma = sigma;
    size = (int)round(6 * sigma - 1);
    if (sigma <= 0) {
        sigma = 0;
        kernel = nullptr;
        return;
    }
    kernel = new double[size];
    double kernelSum = 0.0;
    for (int i = 0; i < size; i++) {
        kernel[i] = Gaussian(sigma, i - (size - 1) / 2);
        kernelSum += kernel[i];
    }

    for (int i = 0; i < size; i++) {
        kernel[i] /= kernelSum;
    }
}
