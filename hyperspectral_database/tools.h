#ifndef TOOL_H_
#define TOOL_H_

#include <windows.h>
#include <vector>
#include <string>

using std::string;

struct hyperspectral_data_info
{
	string filename;
	uint16_t resolution_w;
	uint16_t resolution_h;
	uint16_t bands;
	string date;
	uint16_t datatype;
	float longitude;
	float latitude;
	float altitude;
	float height;
};

typedef hyperspectral_data_info HINFO;

bool GetFilesInDir(string dbPath, std::vector<string> &ofiles);
bool DirPathExist(const string DirPath);
bool ReadConfig(string dbPath, string filename, HINFO &hinfo);

string wstringToString(const std::wstring& wstr);

#endif // TOOL_H_