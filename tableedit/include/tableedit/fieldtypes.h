#ifndef _fieldtypes_h_
#define _fieldtypes_h_

#include <map>
#include <string>
#include <pqxx/pqxx>

struct FieldType {
	std::string UDTName;
	std::string DataType;
	pqxx::prepare::param_treatment Treatment;
};

class FieldTypes {

private:
	std::map <std::string,FieldType> m;

public:
	FieldTypes() {
		m["text"] 	= 	(FieldType){ "text", 		"text", 			pqxx::prepare::treat_string };
		m["timestampz"] =	(FieldType){ "timestampz", 	"timestamp with time zone", 	pqxx::prepare::treat_string };
		m["bool"] 	= 	(FieldType){ "bool", 		"boolean", 			pqxx::prepare::treat_bool };
		m["int8"] 	= 	(FieldType){ "int8", 		"bigint", 			pqxx::prepare::treat_direct };
		m["int2"] 	= 	(FieldType){ "int2", 		"smallint", 			pqxx::prepare::treat_direct };
		m["float4"] 	= 	(FieldType){ "float4", 		"real", 			pqxx::prepare::treat_direct };
		m["char"] 	= 	(FieldType){ "char", 		"\"char\"", 			pqxx::prepare::treat_string };
		m["float8"] 	= 	(FieldType){ "float8", 		"double precision",		pqxx::prepare::treat_direct };
		m["varchar"] 	= 	(FieldType){ "varchar",		"character varying",		pqxx::prepare::treat_string };
		m["date"] 	= 	(FieldType){ "date",		"date",				pqxx::prepare::treat_string };
		m["int4"] 	= 	(FieldType){ "int4", 		"integer", 			pqxx::prepare::treat_direct };
	}

	FieldType operator[](std::string key) {
		return m[key];
	}

};

#endif
