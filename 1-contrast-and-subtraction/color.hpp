#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

class Color {
public:
	Color();
	Color(int _r, int _g, int _b, int base = 255);
	Color(cv::String hex);

	int GetR(int base = 255);
	int GetG(int base = 255);
	int GetB(int base = 255);

	void SetSaturation(double saturation);
	void SetContrast(double contrast);

private:
	double r, g, b;
};

Color::Color() {
	r = 0;
	g = 0;
	b = 0;
}

Color::Color(int _r, int _g, int _b, int base) {
	r = _r * 1.0 / base;
	g = _g * 1.0 / base;
	b = _b * 1.0 / base;
}

Color::Color(cv::String hex) {
	// throw std::exception("not implemented");
}

int Color::GetR(int base) {
	return r * base;
}

int Color::GetG(int base) {
	return g * base;
}

int Color::GetB(int base) {
	return b * base;
}


inline double Sigmoid(double x) {
	return 1 / (1 + std::exp(-x));
}

void Color::SetSaturation(double saturation) {
	double deltaR = r - 0.5;
	double deltaG = g - 0.5;
	double deltaB = b - 0.5;

	double maxDelta = std::max(std::abs(deltaR), std::max(std::abs(deltaG), std::abs(deltaB)));

	if (maxDelta == 0) {
		return;
	}

	double scale = 0.5 / maxDelta;
	double sigmoid = Sigmoid(saturation - std::log(scale - 1));

	r = 0.5 + deltaR * scale * sigmoid;
	g = 0.5 + deltaG * scale * sigmoid;
	b = 0.5 + deltaB * scale * sigmoid;
}

/// <summary>
/// 获取一个点关于平面x+y+z=0的对称点
/// </summary>
inline void GetOpposite(double &x, double &y, double &z) {
	double k = (x + y + z) / 1.5;
	x = x - k;
	y = y - k;
	z = z - k;
}

void Color::SetContrast(double contrast) {
	double mid = 1.5; // Rmax / 2 + Gmax / 2 + Bmax / 2
	
	double deltaR, deltaG, deltaB;

	bool reversed = false;
	if (r + g + b > mid) {	// same for >= and >
		r = r - 1;
		g = g - 1;
		b = b - 1;
		GetOpposite(r, g, b);
		reversed = true;
	}
	deltaR = r;
	deltaG = g;
	deltaB = b;

	double scale = mid / (deltaR + deltaG + deltaB);
	double sigmoid = Sigmoid(-contrast / scale - std::log(scale - 1)) * scale;

	r = sigmoid * r;
	g = sigmoid * g;
	b = sigmoid * b;
	if (reversed) {
		GetOpposite(r, g, b);
		r = r + 1;
		g = g + 1;
		b = b + 1;
	}
}
