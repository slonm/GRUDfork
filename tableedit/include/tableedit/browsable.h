
#ifndef _tableedit_browsable_h_
#define _tableedit_browsable_h_

#include <tableedit/simple.h>

namespace TableEdit {

	class Browsable : public Simple {
		private:
		protected:
			Field* BrowseField;

			std::string ReadForm();
			std::string InsertForm();
			bool CommitInsert();
			std::string UpdateForm();
			bool CommitUpdate();
			std::string DeleteForm();
			bool CommitDelete();
		public:
			Browsable(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string TableName):
				Simple(CGI,DBConnection,TableName) {};
			void BrowseWith(std::string _PrimaryKey, std::string _ValueKey, std::string _OrderKey, bool _OrderAscending);
			std::string OnLoadJS();
	};
};

#endif
