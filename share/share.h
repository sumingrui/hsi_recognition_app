// author: sumingrui
// shared function
#pragma once

#include <string>

enum LOGLEVEL {
	info,
	warnning,
	error
};

extern std::string logTimestamp;

std::string GetTimeCurrent();
void log(LOGLEVEL level, std::string content);