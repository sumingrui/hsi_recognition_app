// db_base: Define the entry point for the DLL application.

#include "db_base.h"
#include "hyperspectral_db.h"

DB_API DBEXPORT * CreateExportObj(string dbPath)
{
	return new SpectralDataSQL(dbPath);
}

DB_API void DestoryExportObj(DBEXPORT* pDBEXPORT) {
	pDBEXPORT->Release();
}