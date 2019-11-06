#define COMPILER_MSVC
#define NOMINMAX

#include <vector>
#include <mat.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.h"
#include "tf_model.h"

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
 
using namespace tensorflow;
using std::cout;
using std::endl;
using std::vector;


bool ReadRaw(string filename)
{
	return false;
}

int Tf_2dcnn(string & imgPath)
{
	FILE *raw;
	raw = fopen(".//dataset//newrawSinglefile20190711140909_lensCor_ref_flassh.raw", "rb");
	if (raw != NULL) {
		std::cout << "success." << std::endl;
		const int lines = 482; //行
		const int sample = 640; //列
		const int bands = 224; //波段
		uint8_t image[lines][sample][bands];






	// 改成读取raw文件，文件名外部传入
	// 读取mat文件
	MATFile *pMatFile1 = NULL;
	mxArray *pMxArray1 = NULL;
	MATFile *pMatFile2 = NULL;
	mxArray *pMxArray2 = NULL;

	// todo: 预加载模型
	pMatFile1 = matOpen("dataset//TRAIN909_30m.mat", "r");
	pMxArray1 = matGetVariable(pMatFile1, "scene");
	pMatFile2 = matOpen("dataset//BB1.mat", "r");
	pMxArray2 = matGetVariable(pMatFile2, "BB1");

	const size_t *n1 = mxGetDimensions(pMxArray1);
	const size_t *n2 = mxGetDimensions(pMxArray2);

	// 按w方向顺序排列
	double *pData1 = (double*)(mxGetData(pMxArray1));
	double *pData2 = (double*)(mxGetData(pMxArray2));

	// opencv数据 h:482 w:640 d:224
	int h = *n1;
	int w = *(n1 + 1);
	int d = *(n1 + 2);
	
	// todo: 预定义多个cv矩阵
	typedef cv::Vec<double, 224> vec224d;
	cv::Mat img_cube = cv::Mat::zeros(h, w, CV_64FC(224));

	for (int i = 0; i < d; i++) {
		double *source_channel = pData1 + (i*w*h);
		for (int j = 0; j < w; j++) {
			double *source_row = source_channel + (j*h);
			for (int k = 0; k < h; k++) {
				img_cube.at<vec224d>(k,j)[i] = *(source_row + k);
			}
		}
	}
	
	//  gt opencv 数据
	int h_gt = *n2;
	int w_gt = *(n2 + 1);
	cv::Mat img_gt = cv::Mat::zeros(h_gt, w_gt, CV_64FC1);
	for (int i = 0; i < w_gt; i++) {
		double *source_gt = pData2 + (i*h_gt);
		for (int j = 0; j < h_gt; j++) {
			img_gt.at<double>(j, i) = *(source_gt + j);
		}
	}
	save_imagesc(img_gt, "img_gt.jpg");

	
	// norm
	img_cube = norm(img_cube);

	//img_cube = pad_2dcnn(img_cube);

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


int Tf_3dcnn(string & imgPath)
{
	return 0;
}
