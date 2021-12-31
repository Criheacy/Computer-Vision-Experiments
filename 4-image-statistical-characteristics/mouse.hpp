#include <opencv2/opencv.hpp>

class Mouse {
public:
	static Mouse& Instance();
	Mouse(Mouse const&) = delete;
	void operator=(Mouse const&) = delete;

	void OnEvent(int event, int x, int y, int flags, void *param);

	bool mLPressing;
	cv::Point mPosition;
	cv::Point mLBegin;
	cv::Point mLEnd;

private:
	Mouse() {};
	static Mouse mInstance;
};

Mouse& Mouse::Instance() {
	static Mouse instance;
	return instance;
}

void Mouse::OnEvent(int event, int x, int y, int flags, void *param)
{
	if (event == cv::EVENT_LBUTTONDOWN) {
		mLPressing = true;
		mLBegin = cv::Point(x, y);
	}
	if (event == cv::EVENT_LBUTTONUP) {
		mLPressing = false;
		mLEnd = cv::Point(x, y);
	}
	if (event == cv::EVENT_MOUSEMOVE) {
		mPosition = cv::Point(x, y);
	}
}