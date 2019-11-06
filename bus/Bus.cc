#include <Windows.h>
#include <iostream>
#include <io.h>

#include "Bus.h"
#include "../share/share.h"
#include "tf_model.h"


HSIC::Bus::Bus() {
	// 读配置文件
	if (ReadAppConfig()) {
		log(info,"Read config success.");
	}

	// 数据库初始化
	pHsiDB = CreateExportObj(config_.dataPath);
	if (!pHsiDB->Connect(config_.hostName,config_.account,config_.passwd,config_.db,config_.port)) {
		exit(0);
	}
	if (!pHsiDB->Intialize()) {
		exit(0);
	}
	else {
		log(info, "Intialize HSI database.");
	}
}

HSIC::Bus::~Bus() {
	DestoryExportObj(pHsiDB);
}


// 读配置文件	
bool HSIC::Bus::ReadAppConfig() {
	if (_access("settings.ini", 0)) {
		return FALSE;
	}

	GetPrivateProfileString("Database", "HostName", "localhost", config_.hostName, 64, ".//settings.ini");
	GetPrivateProfileString("Database", "Account", "root", config_.account, 20, ".//settings.ini");
	GetPrivateProfileString("Database", "Passwd", "123456", config_.passwd, 20, ".//settings.ini");
	GetPrivateProfileString("Database", "DB", "smr", config_.db, 20, ".//settings.ini");
	GetPrivateProfileString("Database", "DataPath", "H:\\hyperspectral_database\\", config_.dataPath, 128, ".//settings.ini");
	config_.port = GetPrivateProfileInt("Database", "Port", 3306, ".//settings.ini");
	config_.speed_level = GetPrivateProfileInt("Algorithm", "speed_level", 2, ".//settings.ini");

	return TRUE;
}

void HSIC::Bus::MultiModule() {
	
	



}

void HSIC::Bus::ObjSearch() {
	while (TRUE) {
		// 监控新数据
		if (pHsiDB->CheckNewData()) {
			// 选择算法
			config_.speed_level = GetPrivateProfileInt("Algorithm", "speed_level", 2, ".//settings.ini");
			switch (config_.speed_level)
			{
			case 1:
				// slow
				log(info, "Choose algorithm speed 1.");

				break;

			case 2:
				// middle
				log(info, "Choose algorithm speed 2.");

				break;

			case 3:
				//fast
				log(info, "Choose algorithm speed 3.");

				break;

			default:
				log(warnning, "No such speed level.");
				break;
			}
			


			// 详查算法


		}




	}


}


