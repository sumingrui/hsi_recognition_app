#include "share.h"
#include <time.h>
#include <iostream>
#include<mutex>

std::string GetTimeCurrent() {
	char tmp[20];
	time_t timestamp;
	time(&timestamp);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d_%H-%M-%S", localtime(&timestamp));
	return tmp;
}

std::string logTimestamp = GetTimeCurrent();
std::mutex m_mutex;

// logfile timestamp
void log(LOGLEVEL level, std::string content) {
	// 日志级别
	std::string strLoglevel;
	if (level == info) {
		strLoglevel = ",[INFO]: ";
	}
	else if (level == warnning) {
		strLoglevel = ",[WARRNING]: ";
	}
	else {
		strLoglevel = ",[ERROR]: ";
	}

	// 写日志
	FILE *logfile;
	std::unique_lock<std::mutex> locker(m_mutex);
	logfile = fopen((".//results//"+logTimestamp + ".log").c_str(), "at+");
	if (logfile != NULL) {
		char timeBuffer[20];
		time_t current;
		time(&current);
		strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localtime(&current));
		std::string singleLog = std::string(timeBuffer) + strLoglevel + content + "\r\n";
		fwrite(singleLog.c_str(), singleLog.size(), 1, logfile);
		fclose(logfile);
		locker.unlock();
	}
	else {
		std::cout << "Log wrong." << std::endl;
	}
}
