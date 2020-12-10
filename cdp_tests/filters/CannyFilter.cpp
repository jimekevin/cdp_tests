#include "CannyFilter.h"

CannyFilter::CannyFilter(int threshold1, int threshold2)
	: threshold1(threshold1), threshold2(threshold2)
{
}

const int CannyFilter::getThreshold1() {
	return threshold1;
}
const int CannyFilter::getThreshold2() {
	return threshold2;
}
void CannyFilter::updateThreshold1(int threshold1) {
	this->threshold1 = threshold1;
}
void CannyFilter::updateThreshold2(int threshold2) {
	this->threshold2 = threshold2;
}

void CannyFilter::apply(cv::Mat& depthMat, cv::Mat& rgbMat) {
	int type;
	cv::Mat image(rgbMat);

	cv::Mat mask;
	cv::inRange(image, cv::Scalar(0.0f, 0.0f, 0.0f), cv::Scalar(0.0f, 0.0f, 0.0f), mask);
	image.setTo(cv::Scalar(1.0f, 1.0f, 1.0f), mask);

	//image.convertTo(rgbMat, CV_32FC3); cv::cvtColor(rgbMat, rgbMat, cv::COLOR_GRAY2RGB); return;

	cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
	type = image.type();
	cv::threshold(image, image, 0.8f, 1.0f, cv::THRESH_BINARY);
	type = image.type();
	image.convertTo(image, CV_8UC1);
	type = image.type();

	//image.convertTo(rgbMat, CV_32FC3); cv::cvtColor(rgbMat, rgbMat, cv::COLOR_GRAY2RGB); return;

	cv::Mat element = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
	cv::morphologyEx(image, image, cv::MorphTypes::MORPH_OPEN, element);
	cv::morphologyEx(image, image, cv::MorphTypes::MORPH_CLOSE, element);
	//cv::threshold(image, image, 0.5f, 1.0f, cv::THRESH_BINARY_INV);

	//image.convertTo(rgbMat, CV_32FC3); cv::cvtColor(rgbMat, rgbMat, cv::COLOR_GRAY2RGB); return;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	type = image.type();
	cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	auto redColor = cv::Scalar(1.0f, 0.0f, 0.0f, 1.0f);
	int idx = 0;
	std::vector<cv::Scalar> colors = {
		cv::Scalar(1.0f, 0.0f, 0.0f, 1.0f),
		cv::Scalar(0.0f, 1.0f, 0.0f, 1.0f),
		cv::Scalar(0.0f, 0.0f, 1.0f, 1.0f),
		cv::Scalar(1.0f, 1.0f, 0.0f, 1.0f),
		cv::Scalar(1.0f, 0.0f, 1.0f, 1.0f),
		cv::Scalar(0.0f, 1.0f, 1.0f, 1.0f),
		cv::Scalar(0.5f, 1.0f, 0.0f, 1.0f),
		cv::Scalar(0.5f, 0.0f, 1.0f, 1.0f),
		cv::Scalar(1.0f, 0.5f, 0.0f, 1.0f),
		cv::Scalar(1.0f, 0.0f, 0.5f, 1.0f),
	};
	std::vector<std::vector<cv::Point>> hull(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], hull[i]);
	}
	std::vector<double> areas;
	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > 50) {
			continue;
		}
		auto area = cv::contourArea(hull[i]);
		areas.push_back(area);
		if (area > 1000) {
			continue;
		}
		auto color = colors[i % colors.size()];
		cv::drawContours(rgbMat, hull, i, colors[1], cv::FILLED, 8);
		cv::drawContours(rgbMat, contours, i, colors[0], 2, 8, hierarchy, 0);
	}
	return;

	//std::vector<std::vector<cv::Point>> hull(contours.size());
	/*for (size_t i = 0; i < contours.size(); i++) {
		//cv::convexHull(contours[i], hull[i]);
		auto redColor   = cv::Scalar(1.0f, 0.0f, 0.0f, 1.0f); // Red for external contours
		auto greenColor = cv::Scalar(0.0f, 0.0f, 1.0f, 1.0f); // Blue for internal contours
		//auto levels = 2; // 1 contours drawn, 2 internal contours as well, 3 ...
		//cv::drawContours(rgbMat, contours, (int)i, redColor, 2);// cv::FILLED);
		cv::drawContours(rgbMat, contours, )
	}*/
}

/*void CannyFilter::apply(cv::Mat& depthMat, cv::Mat& rgbMat) {
	cv::Mat grayMat;
	cv::cvtColor(rgbMat, grayMat, cv::COLOR_RGB2GRAY);

	cv::blur(grayMat, grayMat, cv::Size(3, 3));
	// back to gray out in rgb
	//cv::cvtColor(grayMat, rgbMat, cv::COLOR_GRAY2RGB);


	//for (int y = 0; y < depthMat.rows; y++) {
		//for (int x = 0; x < depthMat.cols; x++) {
			//auto &depth = depthMat.at<cv::Vec3f>(y, x);
			//auto &rgb = rgbMat.at<cv::Vec3f>(y, x);
			//auto t3 = grayMat.type();
			//auto &gray = grayMat.at<uchar>(y, x);
			//auto i = 3;
		//}
	//}
	//cv::cvtColor(grayMat, grayMat, CV_8UC1);
	grayMat.convertTo(grayMat, CV_8UC1);
	cv::Mat edges;
	auto t3 = grayMat.type();
	auto t4 = rgbMat.type();
	auto t5 = edges.type();
	cv::Canny(grayMat, edges, threshold1, threshold2, 3);
	auto t6 = edges.type();

	//edges.convertTo(rgbMat, CV_32FC3);
	//cv::cvtColor(rgbMat, rgbMat, cv::COLOR_GRAY2RGB);
	//auto t61 = rgbMat.type();
	//return;
	auto t7 = rgbMat.type();
	//cv::cvtColor(rgbMat, rgbMat, cv::COLOR_GRAY2RGB);
	auto t8 = rgbMat.type();

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	std::vector<std::vector<cv::Point>> hull(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], hull[i]);
	}
	//cv::Mat rgbMat = cv::Mat::zeros(cannyOut.size(), CV_8UC3);
	cv::Scalar redColor = cv::Scalar(1.0f, 0.0f, 0.0f, 1.0f);
	cv::Scalar blueColor = cv::Scalar(0.0f, 0.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < contours.size(); i++) {
		//drawContours(drawing, contours, (int)i, color);
		//drawContours(drawing, hull, (int)i, color);
		drawContours(rgbMat, contours, (int)i, redColor, 2);
		//drawContours(rgbMat, hull, (int)i, blueColor, cv::FILLED);
	}

	//auto t9 = rgbMat.type();
	//rgbMat.convertTo(rgbMat, CV_32FC3);
	//auto t10 = rgbMat.type();
	auto t62 = rgbMat.type();
}*/