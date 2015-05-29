#ifndef _tableedit_constructor_h_
#define _tableedit_constructor_h_

#include <tableedit/simple.h>
#include <list>

namespace TableEdit {

	class Constructor : public Simple {
	private:
	protected:
		typedef std::list<Simple*> Tables;
		Tables tables;
	public:

		Constructor(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string key)
		: Simple(CGI, DBConnection, key) {
		};
		Constructor& AddTable(Simple& table);
		std::string operator()();
		std::string OnLoadJS();
		virtual bool CatchAJAX();
	};
};

#endif
