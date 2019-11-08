#define COMPILER_MSVC
#define NOMINMAX

#include <vector>
#include <mat.h>
#include <iostream>
#include "utils.h"
#include "tf_model.h"
#include "../share/share.h"

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
 
using namespace tensorflow;
using std::vector;

typedef cv::Vec<double, 224> vec224d;
const int lines = 482;
const int samples = 640;
const int bands = 224;

bool ReadRawfile(string rawfilepath, cv::Mat &img_cube)
{
	FILE *rawfile;
	img_cube = cv::Mat::zeros(lines, samples, CV_64FC(224));
	rawfile = fopen(rawfilepath.c_str(), "rb");
	if (rawfile != NULL) {
		for (int h = 0; h < lines; h++) {
			fseek(rawfile, h*samples*bands*2, SEEK_SET);
			uint16_t raw_arr[samples*bands] = { 0 };
			fread(raw_arr, sizeof(uint16_t), samples*bands, rawfile);
			for (int d = 0; d < bands; d++) {
				for (int w = 0; w < samples; w++) {
					img_cube.at<vec224d>(h, w)[d] = double(raw_arr[d*samples + w]);
				}
			}
		}
		fclose(rawfile);
		log(info, "load raw file success: " + rawfilepath);
		return TRUE;
	}
	else {
		log(warnning, "load raw file failed: " + rawfilepath);
		return false;
	}
}


bool Draw_gt(string matfilepath, string key) {
	MATFile *pMatFile = NULL;
	mxArray *pMxArray = NULL;
	pMatFile = matOpen(matfilepath.c_str(), "r");
	pMxArray = matGetVariable(pMatFile, key.c_str());

	const size_t *n = mxGetDimensions(pMxArray);
	int h_gt = *n;
	int w_gt = *(n + 1);

	double *pData = (double*)(mxGetData(pMxArray));
	cv::Mat img_gt = cv::Mat::zeros(h_gt, w_gt, CV_64FC1);
	for (int i = 0; i < w_gt; i++) {
		double *source_gt = pData + (i*h_gt);
		for (int j = 0; j < h_gt; j++) {
			img_gt.at<double>(j, i) = *(source_gt + j);
		}
	}
	save_imagesc(img_gt, "img_gt.jpg");
	log(info, "Draw ground truth: img_gt.jpg");
}


int TF_2dcnn(string rawfilepath, string filename, string & sendImgPath) {
	cv::Mat img_cube = cv::Mat::zeros(lines, samples, CV_64FC(224));
	rawfilepath = rawfilepath + filename + ".raw";

	if (!ReadRawfile(rawfilepath, img_cube)) {
		return 0;
	}
	
	// norm
	img_cube = norm(img_cube);

	// pad 224->225
	pad_2dcnn(img_cube);

	img_cube.convertTo(img_cube, CV_32F);

	//构建sess and model
	Session* session;
	Status status = NewSession(SessionOptions(), &session);
	if (!status.ok()) {
		log(LOGLEVEL::error, status.ToString());
		return 0;
	}

	GraphDef graph_def;
	status = ReadBinaryProto(Env::Default(), "2dcnn.pb", &graph_def);
	if (!status.ok()) {
		log(LOGLEVEL::error, status.ToString());
		return 0;
	}

	status = session->Create(graph_def);
	if (!status.ok()) {
		log(LOGLEVEL::error, status.ToString());
		return 1;
	}

	vector<std::pair<string, Tensor>> inputs;
	vector<Tensor> outputs;

	// 赋值给tensor
	typedef cv::Vec<float, 225> vec225f;
	Tensor input_tensor(DT_FLOAT, TensorShape({ lines*samples,15,15,1 }));
	auto input_tensor_mapped = input_tensor.tensor<float, 4>();

	inputs = { {"input_1",input_tensor} };

	for (int nRow = 0; nRow < lines; nRow++) {
		for (int nCol = 0; nCol < samples; nCol++) {
			for (int nChannel = 0; nChannel < 225; nChannel++) {
				input_tensor_mapped(nRow*samples + nCol, nChannel / 15, nChannel % 15, 0) =
					img_cube.at<vec225f>(nRow,nCol)[nChannel];
			}
		}
	}

	status = session->Run(inputs, { "fc1/Softmax" }, {}, &outputs);
	if (!status.ok()) {
		log(LOGLEVEL::error, status.ToString());
	}

	auto output = outputs[0].tensor<float, 2>();
	int output_num = outputs[0].shape().dim_size(0);
	int output_dim = outputs[0].shape().dim_size(1);
	
	cv::Mat img_spectral = cv::Mat::zeros(lines, samples, CV_64FC1);

	for (int n = 0; n < output_num; n++) {
		double output_prob = 0.0;
		double class_id = 0.0;
		for (int j = 0; j < output_dim; j++) {
			if (output(n, j) >= output_prob) {
				class_id = j;
				output_prob = output(n, j);
			}
		}
		img_spectral.at<double>(n/samples, n%samples) = double(class_id);
	}

	//save opencv mat
	save_xml(img_spectral, ".//results//" + filename + ".xml");
	save_imagesc(img_spectral, ".//results//" + filename + ".jpg");
	sendImgPath = ".//results//" + filename + ".jpg";

	session->Close();
	
	return 0;
}


int TF_3dcnn(string rawfilepath, string filename, string & sendImgPath) {


	/*
// bf and pad
// todo： 增加参数配置文件
// int kernel_size = 5;
cv::Mat img_bf;
// img_bf = bf_pad(img_cube, d, kernel_size);




// 构建sess and model
Session* session;
Status status = NewSession(SessionOptions(), &session);
if (!status.ok()) {
	cout << status.ToString() << "\n";
	return 1;
}

GraphDef graph_def;
status = ReadBinaryProto(Env::Default(), "resnet_v1.pb", &graph_def);
if (!status.ok()) {
	cout << status.ToString() << "\n";
	return 1;
}

status = session->Create(graph_def);
if (!status.ok()) {
	cout << status.ToString() << "\n";
	return 1;
}

vector<std::pair<string, Tensor>> inputs;
vector<Tensor> outputs;

cv::Mat img_spectral = cv::Mat::zeros(w, h, CV_64FC1);

// 赋值给tensor
// todo: 增加多个tensorflow cv矩阵
typedef cv::Vec<float, 200> vec200f;
Tensor input_tensor(DT_FLOAT, TensorShape({ w,kernel_size,kernel_size,d,1 }));
auto input_tensor_mapped = input_tensor.tensor<float, 5>();
inputs = { {"input_1",input_tensor} };

for (int nRow = 0; nRow < h; nRow++) {
	for (int nCol = 0; nCol < w; nCol++) {
		for (int kw = 0; kw < kernel_size; kw++) {
			for (int kh = 0; kh < kernel_size; kh++) {
				for (int nDepth = 0; nDepth < d; nDepth++) {
					input_tensor_mapped(nCol, kw, kh, nDepth, 0) =
						img_bf.at<vec200f>(nRow + kw, nCol + kh)[nDepth];
				}
			}
		}
	}

	status = session->Run(inputs, { "dense_1/Softmax" }, {}, &outputs);
	if (!status.ok()) {
		cout << status.ToString() << endl;
	}
	else {
		cout << "complete " << std::setprecision(4) << double(nRow + 1) / double(h) * 100 << "%" << endl;
	}

	auto output = outputs[0].tensor<float, 2>();
	int output_num = outputs[0].shape().dim_size(0);
	int output_dim = outputs[0].shape().dim_size(1);

	for (int n = 0; n < output_num; n++) {
		double output_prob = 0.0;
		double class_id = 0.0;
		for (int j = 0; j < output_dim; j++) {
			if (output(n, j) >= output_prob) {
				class_id = j;
				output_prob = output(n, j);
			}
		}
		if (img_gt.at<double>(nRow, n) != 0.0) {
			img_spectral.at<double>(nRow, n) = double(class_id + 1);
		}
	}
}

mxDestroyArray(pMxArray1);
mxDestroyArray(pMxArray2);
matClose(pMatFile1);
matClose(pMatFile2);

// save opencv mat
save_xml(img_spectral, "img_spectral.xml");
save_imagesc(img_spectral, "img_spectral.jpg");
imgPath = "img_spectral.jpg";

session->Close();
*/


	return 0;
}
