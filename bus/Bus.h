#pragma once

#include <stdint.h>
#include "db_base.h"

namespace HSIC {

	struct Config
	{
		// database config
		char hostName[64];
		char account[20];
		char passwd[20];
		char db[20];
		char dataPath[128];
		uint32_t port;

		// algorithm config
		uint32_t speed_level;		// 1: slow  2:middle  3:fast
	};

	class Bus
	{
	public:
		Bus();
		~Bus();
		bool ReadAppConfig();
		void MultiModule();
		void ObjSearch();

	private:
		Config config_;
		DBEXPORT* pHsiDB;
	};
}


