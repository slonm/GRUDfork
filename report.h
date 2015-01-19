#ifndef _report_h_
#define _report_h_

#include <iostream>
#include <list>
#include <vector>

#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <pqxx/pqxx>

#include "tableedit/include/tableedit/querystream.h"

enum ParamTypes { ptArray, ptDate, ptForeign, ptNumeric, ptText  };

struct Parametro{
	std::string nome;
	std::string valor;
};

class Report;

class Param {

public:
	ParamTypes Type;
	std::vector <std::string> Array;
	int Length;
	bool Link;
	std::string LinkTable;
	std::string LinkKey;
	std::string LinkValue;
	std::string LinkOrder;
	std::string Filtro;
	bool LinkAscending;

	std::string Label;

	Report* ParentReport;

	std::string Name;
	std::string Value;

	Param(Report* ParentReport);
	std::string GetInputHTML(std::string Value="",std::string Id = "", std::string OnChange = "");
};

class Report {
private:
protected:

	pqxx::connection *DBConnection;
	cgicc::Cgicc *CGI;

	std::string ReportName;
	std::string CSSClass;

	std::string Title;
	std::string InputQuery;
	std::string Query;
	std::string Query1;

	std::list <Param*> Params;

	std::vector<Parametro> parametros;

	void AddParam(ParamTypes Type, std::string Label);
	void AddArrayParam(std::string str, std::string Label);
	void AddForeignParam(std::string str, std::string Label);
	void AddTextParam(std::string str, std::string Label);



	std::string Trim(std::string str);
	void ParseInput();
	void ParseParam(std::string str);
	void SubstituteParams();

	std::string Button(std::string Id, std::string Text, std::string JSFunction );

public:
	Report(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string ReportName, std::string Title, std::string Query,std::string Query1);
	std::string Scope(std::string Name);
	pqxx::connection* GetDBConnection() { return DBConnection; };
	std::string OnLoadJS(); 
	std::string GetFormElement(std::string Name);
	bool CatchAJAX();
	void SetCSS(std::string _S) { CSSClass = _S; };
	std::string InputForm();
	std::string OutputForm();
	std::string operator()();
	void setParametros(std::vector<Parametro> par);
};

#endif
