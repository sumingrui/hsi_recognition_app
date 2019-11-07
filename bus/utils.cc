#include "utils.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

void save_imagesc(cv::Mat img, std::string path) {
	double max = 0.0, min = 0.0;
	cv::minMaxIdx(img, &min, &max);
	img = (img - min) / (max - min);
	img.convertTo(img, CV_8UC1, 255.0, 0);
	applyColorMap(img, img, cv::COLORMAP_HOT);
	cv::resize(img, img, cv::Size(img.cols * 3, img.rows * 3));
	cv::imwrite(path.c_str(), img);
}


cv::Mat norm(cv::Mat mat) {
	cout << "import normalize ... ..." << endl;
	double max = 0.0, min = 0.0;
	cv::minMaxIdx(mat, &min, &max);
	mat = (mat - min) / (max - min);
	return mat;
}

// todo: ·ÖÀëbfºÍpad
cv::Mat bf_pad(cv::Mat mat,int nChannels,int kernel_size) {
	cout << "import bf ... ..." << endl;
	cv::Mat img_bf;
	mat.convertTo(img_bf, CV_32F);
	vector<cv::Mat> single_channel(nChannels);
	cv::split(img_bf, single_channel);

	int pad = (kernel_size - 1) / 2;

	vector<cv::Mat> dst(nChannels);
	for (int c = 0; c < nChannels; c++) {
		cv::bilateralFilter(single_channel[c], dst[c], 7, 50, 50);
		cv::copyMakeBorder(dst[c], dst[c], pad, pad, pad, pad, cv::BORDER_CONSTANT, 0);
	}
	cv::merge(dst, img_bf);
	return img_bf;
}


// 2dcnn_pad
void pad_2dcnn(cv::Mat &mat) {
	cv::Mat single_zero_pad = cv::Mat::zeros(mat.size().height, mat.size().width, CV_64FC1);
	vector<cv::Mat> img_pad;
	img_pad.push_back(mat);
	img_pad.push_back(single_zero_pad);
	cv::merge(img_pad, mat);
}






void save_xml(cv::Mat mat,std::string path) {
	cv::FileStorage fs(path.c_str(), cv::FileStorage::WRITE);
	fs << "img_spectral" << mat;
	fs.release();
}