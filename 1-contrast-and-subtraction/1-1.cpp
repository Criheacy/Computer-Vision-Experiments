#include <opencv2/opencv.hpp>
#include <iostream>

#include "color.hpp"

using namespace cv;
using namespace std;

Mat image;
String windowName;

double contrast;
double saturation;

void UpdateImage()
{
	Mat processed = image.clone();

	uint8_t* pixelPtr = (uint8_t*)processed.data;
	int cn = processed.channels();

	for (int i = 0; i < processed.rows; i++)
	{
		for (int j = 0; j < processed.cols; j++)
		{
			uint8_t* b, * g, * r;
			b = &pixelPtr[i * processed.cols * cn + j * cn + 0]; // B
			g = &pixelPtr[i * processed.cols * cn + j * cn + 1]; // G
			r = &pixelPtr[i * processed.cols * cn + j * cn + 2]; // R

			Color pixel = Color(*r, *g, *b);

			pixel.SetContrast(contrast);
			pixel.SetSaturation(saturation);

			*r = pixel.GetR();
			*g = pixel.GetG();
			*b = pixel.GetB();
		}
	}
	imshow(windowName, processed); // Show our image inside the created window.
}

void onContrastChanged(int value, void*)
{
	contrast = (value - 50.0) / 20;
	UpdateImage();
}

void onSaturationChanged(int value, void*)
{
	saturation = (value - 50.0) / 20;
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

	UpdateImage();
	imshow(windowName, image); // Show our image inside the created window.
	createTrackbar("Contrast", windowName, 0, 100, onContrastChanged);
	createTrackbar("Saturation", windowName, 0, 100, onSaturationChanged);

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window

	return 0;
}