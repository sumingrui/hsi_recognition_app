#pragma once

#include <opencv2/opencv.hpp>
#include <string>

void save_imagesc(cv::Mat img, std::string path);
cv::Mat norm(cv::Mat mat);
cv::Mat bf_pad(cv::Mat mat, int nChannels, int kernel_size);
void pad_2dcnn(cv::Mat &mat);
void save_xml(cv::Mat mat, std::string path);