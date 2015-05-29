#ifndef _tableedit_simple_h_
#define _tableedit_simple_h_

#include <list>
#include <string>
#include <sstream>

#include <tableedit/field.h>
#include <tableedit/fieldtypes.h>
#include <tableedit/util.h>
#include <tableedit/querystream.h>

#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <pqxx/pqxx>


namespace TableEdit {

	class Field;

	class Simple {
		private:
		protected:
			pqxx::connection *DBConnection;
			cgicc::Cgicc *CGI;
			int PageSize;
			std::string CSSClass;
			std::string Caption;

			FieldTypes Types;

			size_t selectQueryNextParamId;
			virtual void BindSelectQueryParamTypes(pqxx::prepare::declaration& d);
			virtual void BindSelectQueryParams(pqxx::prepare::invocation& inv);
			virtual std::string ReadForm();
			std::string InsertForm();
			bool CommitInsert();
			virtual std::string UpdateForm();
			bool CommitUpdate();
			std::string DeleteForm();
			bool CommitDelete();

			virtual void FillSelectWhere(QueryStream& WhereClause);
			std::string GetSelectQuery();
			std::string GetSelectForUpdateQuery(std::string id);
			std::string Button(std::string Id, std::string Text, std::string JSFunction );
			std::string Pagination();

		public:
			std::string TableName;
			std::list<Field*> Fields;
			Simple(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string TableName);
			std::string operator()();
			std::string OnLoadJS();
			std::string Scope(std::string Name);
			pqxx::connection* GetDBConnection() { return DBConnection; };

			Field* AddField(std::string Name);
			Field* GetFieldByName(std::string Name);
			Field* GetPrimaryKey();
			Field* operator[](std::string Name) { return GetFieldByName(Name); } ;

			void SetPageSize(int S) { PageSize = S; };
			void SetCSS(std::string S) { CSSClass = S; };
			void SetCaption(std::string C) { Caption = C;};
			void DetectTypes();
			virtual bool CatchAJAX();
			std::string GetFormElement(std::string Name);

	};

};

#endif
