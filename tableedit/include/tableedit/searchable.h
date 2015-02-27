#ifndef _tableedit_searchable_h_
#define _tableedit_searchable_h_

#include <tableedit/simple.h>

namespace TableEdit {

	class Searchable : public Simple {
	private:

		struct Constraint {
			bool isBinary;
			std::string value;
			std::string predicate;
			Field* field;
			FieldType linkFieldType;
		};
		std::list<Constraint> Constraints;
		void FillPredicate(std::string strPredicate, Constraint& cnt);
		FieldType DetectLinkValueType(std::string table, std::string field);
	protected:
		//			std::list<Field*> SearchFields;
	public:
		// propagate constructor to base class
		Searchable(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string TableName) :
		Simple(CGI, DBConnection, TableName) {
		};

		//			Field* AddSearchField(std::string Name);
		std::string operator()();
		std::string OnLoadJS();
		virtual void FillSelectWhere(QueryStream& WhereClause);
		virtual void BindSelectQueryParamTypes(pqxx::prepare::declaration& d);
		virtual void BindSelectQueryParams(pqxx::prepare::invocation& inv);
		virtual bool CatchAJAX();
	};
};

#endif
