#pragma once
#include <iostream>
#include <vector>

#define int64 mysqlInt64
#define uint64 mysqlUInt64
#include <mysql.h>
#include <my_global.h>
#undef int64
#undef uint64

class DBAccessor
{
public:
	struct Config
	{
		std::string host;
		std::string login;
		std::string password;
		std::string dbName;
	};

	struct ParamWhere
	{
		std::string key;
		std::string value;
		std::string compareSign;
		bool isString = false;
	};

	DBAccessor(const Config & config);
	~DBAccessor();

	bool isConnected() const { return connected; }
	bool reconnect(const Config& config);
	long execDirect(const std::string& query);
	std::string getColumnValueByParams(const std::string& table,
		const std::string& column,
		const std::vector<ParamWhere>& params,
		const std::string& orderBy,
		bool desc = false);
	std::string getLastEventByName(const std::string& name);

	static void init();
	static std::string formWhereStatement(const std::vector<ParamWhere>& params);

private:
	MYSQL* connection;
	bool connected;
};
