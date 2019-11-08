// Author: sumingrui
// This is a database IO to hyperspectral

#ifndef HYPERSPECTRAL_DB_H_
#define HYPERSPECTRAL_DB_H_

#include "db_base.h"
#include <iostream>
#include <string>
#include <mysql.h> 
#include <stdio.h>
#include <vector>

using std::string;
using std::vector;

class SpectralDataSQL: public DBEXPORT {
public:
	SpectralDataSQL(string path);
	~SpectralDataSQL();

	virtual bool Connect(const char * host, const char * user, const char * passwd, const char * db, unsigned int port);
	virtual bool Intialize();
	virtual bool CheckNewData();
	virtual bool InsertOneRow(string filename);
	virtual bool ExportOneRow(string infile, string &outfile);
	virtual string GetTaskfile();
	virtual void Release();

private:
	MYSQL hyperspectral_sql_;
	string dbPath_;
	vector<string> ofiles_;
	string task_file_;

};

#endif // HYPERSPECTRAL_DB_H_
