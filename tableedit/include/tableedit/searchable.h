#ifndef _tableedit_searchable_h_
#define _tableedit_searchable_h_

#include <tableedit/simple.h>

namespace TableEdit {

	class Searchable : public Simple {
		private:
		protected: 
			std::list<Field*> SearchFields;
			std::string ReadForm();
		public:
			// propagate constructor to base class
			Searchable(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string TableName):
				Simple(CGI,DBConnection,TableName) {};

			Field* AddSearchField(std::string Name);
	};
};

#endif
