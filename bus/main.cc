#pragma once

#include "tf_model.h"
#include "demo_read.h"
#include "Bus.h"
#include <string>
#include <iostream>
#include <future>

typedef void(*GeneralCallback)(const char*);
GeneralCallback g_pCallback;

void RegistGeneralCallback(GeneralCallback addrFun) {
	g_pCallback=addrFun;
}

void Callcb(const char* addr) {
	g_pCallback(addr);
}

int main() {
	HSIC::Bus bus;
	bus.MultiModule();


	//std::future<int> runSerialportThread_;
	//runSerialportThread_ = std::async(std::launch::async, RunSerialport);
	
	//RegistGeneralCallback(GeneralSearch);
	//std::string imgPath;
	//int status=tf_classify(imgPath);
	//if (!status) {
	//	std::cout << "path=" << imgPath << std::endl;
	//	Callcb(imgPath.c_str());
	//}
	system("pause");
	return 0;
}