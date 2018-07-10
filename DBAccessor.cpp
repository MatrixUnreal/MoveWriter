#include "DBAccessor.hpp"

//#include "Util.hpp"


DBAccessor::DBAccessor(const Config& config)
	: connected(false)
{
	connection = mysql_init(NULL);
	connected = mysql_real_connect(connection,
		config.host.c_str(),
		config.login.c_str(),
		config.password.c_str(),
		config.dbName.c_str(),
		0, NULL, 0);
}

DBAccessor::~DBAccessor()
{
	mysql_close(connection);
	connection = NULL;
}

bool DBAccessor::reconnect(const Config& config)
{
	mysql_close(connection);
	connection = NULL;
	connection = mysql_init(NULL);
	connected = mysql_real_connect(connection,
		config.host.c_str(),
		config.login.c_str(),
		config.password.c_str(),
		config.dbName.c_str(),
		0, NULL, 0);

	return connected;
}

long DBAccessor::execDirect(const std::string& query) //TODO: rework return value
{
	auto ret = mysql_query(connection, query.c_str());
	if (ret != 0)
	{
		log(std::string(mysql_error(connection)) + "\nQuery: " + query);
		return -1;
	}
	else if (query.find("INSERT") != std::string::npos)
	{
		return mysql_insert_id(connection);
	}
	return ret;
}

std::string DBAccessor::getColumnValueByParams(const std::string& table,
		const std::string& column,
		const std::vector<ParamWhere>& params,
		const std::string& orderBy,
		bool desc)
{
	auto whereStatement = formWhereStatement(params);
	std::string query = "SELECT " + column + " FROM " + table +
		whereStatement + " ORDER BY " + orderBy + (desc ? " DESC " : "") + " LIMIT 1;";

	if (mysql_query(connection, query.c_str()))
	{
		return std::string();
	}
	MYSQL_RES *result = mysql_store_result(connection);
	if (result == NULL) 
	{
		return std::string();
	}

	std::string ret;
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row)
	{
		ret = row[0];
	}
	mysql_free_result(result);

	return ret;
}

std::string DBAccessor::getLastEventByName(const std::string& name)
{
	std::string query = "SELECT event FROM Entry "
		"WHERE name = \"" + name + "\" and time_event > DATE_SUB(now(), INTERVAL 5 MINUTE) "
		"ORDER BY id DESC LIMIT 1;";
	if (mysql_query(connection, query.c_str()))
	{
		return std::string();
	}
	MYSQL_RES *result = mysql_store_result(connection);
	if (result == NULL) 
	{
		return std::string();
	}

	std::string event;
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row)
	{
		event = row[0];
	}
	mysql_free_result(result);

	return event;
}

void DBAccessor::init()
{
	if (mysql_library_init(0, NULL, NULL))
	{
	    log("Could not initialize MySQL client library\n");
	}
}

std::string DBAccessor::formWhereStatement(const std::vector<ParamWhere>& params)
{
	if (params.empty())
	{
		return std::string();
	}

	std::string statement = " WHERE ";
	std::string paramVal;
	for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++)
	{
		paramVal = paramIt->value;
		if (paramIt->isString)
		{
			paramVal = "\"" + paramVal + "\"";
		}
		statement += paramIt->key + paramIt->compareSign + paramVal + " ";
		if (paramIt != (params.begin() + (params.size() - 1)))
		{
			statement += "and ";
		}
	}

	return statement;
}