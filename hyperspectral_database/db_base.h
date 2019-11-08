// Author: sumingrui
// dll导出虚基类

#ifndef DB_BASE_
#define DB_BASE_

#include <string>
using std::string;

#ifdef DB_EXPORTS
#define DB_API __declspec(dllexport)
#else
#define DB_API __declspec(dllimport)
#endif

class DBEXPORT
{
public:
	virtual bool Connect(const char*, const char*, const char*, const char*, unsigned int) = 0;
	virtual bool Intialize() = 0;
	virtual bool CheckNewData() = 0;
	virtual bool InsertOneRow(string) = 0;
	virtual bool ExportOneRow(string , string &) = 0;
	virtual string GetTaskfile() = 0;
	virtual void Release() = 0;
};


extern "C" DB_API DBEXPORT* CreateExportObj(string dbPath);
extern "C" DB_API void DestoryExportObj(DBEXPORT* pDBEXPORT);

#endif // DB_BASE_