#pragma once
#include <string>
#include <opencv2/opencv.hpp>

using std::string;

bool ReadRawfile(string rawfilepath, cv::Mat &img_cube);
bool Draw_gt(string matfilepath, string key);
int TF_2dcnn(string rawfilepath, string & imgPath);
int TF_3dcnn(string rawfilepath, string & imgPath);