#include <tableedit/constructor.h>
#include <tableedit/querystream.h>
#include <string>
#include <memory>
#include <list>
#include <sstream>

namespace TableEdit {
	using namespace std;

	std::string Constructor::operator()() {
		using namespace cgicc;
		std::stringstream out;
		out << "<div id='" << Scope("constructor") << "' class='" << CSSClass << "'></div>" << std::endl;
		return out.str();
	}

	std::string Constructor::OnLoadJS() {
		std::stringstream params;
		params << "{ name : '" << TableName << "'";
		params << ", tables: [";
		bool firstPass = true;
		for (Tables::const_iterator tIt = tables.begin(); tIt != tables.end(); tIt++) {
			if (firstPass) {
				firstPass = false;
			} else {
				params << ", ";
			}
			params << " { name : '" << (*tIt)->TableName << "'";
			params << ", fields : [";
			QueryStream FieldList;
			std::list<Field*>::iterator i;
			for (i = (*tIt)->Fields.begin(); i != (*tIt)->Fields.end(); ++i) {
				FieldList.Add("{ 'name': '" + (*i)->GetName()
								+ "', 'type': '" + (*i)->GetType().UDTName
								+ "', 'hidden': " + ((*i)->IsHidden() ? "true" : "false")
								+ " , 'label': '" + (*i)->GetLabel()
								+ "', 'linktable': '" + (*i)->GetLinkTable()
								+ "', 'linkkey': '" + (*i)->GetLinkKey()
								+ "', 'linkvalue': '" + (*i)->GetLinkValue()
								+ "' }");
			}

			params << FieldList.str();
			params << " ]";
			params << "} ";
		}
		params << "]} ";
		return "(function (){var t = new tableEdit.Constructor(" + params.str() + "); \n"
						+ "window[t.name] = t;\n"
						+ "})();\n"
						;
	}

	bool Constructor::CatchAJAX() {
		string Action = GetFormElement(Scope("action"));
		if (!Action.empty()) {
//			if (Action == "sql") {
//				std::cout << GetSelectQuery();
//			}
			return true;
		}
		return false;
	}

	Constructor& Constructor::AddTable(Simple& table) {
		tables.push_back(&table);
		return *this;
	}
}; //namespace
