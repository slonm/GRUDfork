#include <tableedit/searchable.h>
#include <tableedit/querystream.h>
#include <string>
#include <memory>
#include <list>
#include <sstream>

namespace TableEdit {
	using namespace std;
	//Field* Searchable::AddSearchField(std::string Name) {
	//	Field* t = GetFieldByName(Name);
	//	Fields.push_back(t);
	//	return t;
	//}

	std::string Searchable::operator()() {
		using namespace cgicc;
		std::stringstream out;
		out << "<div id='" << Scope("searchable") << "' class='" << CSSClass << "'>" << std::endl;
		out << "<div id='" << Scope("filter") << "' class='" << CSSClass << "'></div>" << std::endl;
		out << Simple::operator()();
		out << "</div>" << std::endl;
		return out.str();
	}

	std::string Searchable::OnLoadJS() {
		std::stringstream params;
		params << " { ";
		params << "'name' : '" + TableName + "',  ";
		params << "'fields' : [";

		QueryStream FieldList;
		std::list<Field*>::iterator i;
		for (i = Fields.begin(); i != Fields.end(); ++i) {
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
		params << " ]}";
		return "(function (){var t = new tableEdit.Searchable(" + params.str() + "); \n"
						+ "window[t.name] = t;\n"
						+ "window[t.name].run();})();\n"
						;
	}

	void Searchable::FillPredicate(std::string strPredicate, Constraint& cnt) {
		if (strPredicate == "like") {
			cnt.isBinary = true;
			cnt.predicate = "LIKE";
		} else if (strPredicate == "eq") {
			cnt.isBinary = true;
			cnt.predicate = "=";
		} else if (strPredicate == "lt") {
			cnt.isBinary = true;
			cnt.predicate = "<";
		} else if (strPredicate == "eqlt") {
			cnt.isBinary = true;
			cnt.predicate = "<=";
		} else if (strPredicate == "gt") {
			cnt.isBinary = true;
			cnt.predicate = ">";
		} else if (strPredicate == "eqgt") {
			cnt.isBinary = true;
			cnt.predicate = ">=";
		} else if (strPredicate == "true") {
			cnt.isBinary = false;
			cnt.predicate = "= TRUE";
		} else if (strPredicate == "false") {
			cnt.isBinary = false;
			cnt.predicate = "= FALSE";
		} else if (strPredicate == "empty") {
			cnt.isBinary = false;
			cnt.predicate = "IS NULL";
		} else if (strPredicate == "n-like") {
			cnt.isBinary = true;
			cnt.predicate = "NOT LIKE";
		} else if (strPredicate == "n-eq") {
			cnt.isBinary = true;
			cnt.predicate = "<>";
		} else if (strPredicate == "n-lt") {
			cnt.isBinary = true;
			cnt.predicate = ">=";
		} else if (strPredicate == "n-eqlt") {
			cnt.isBinary = true;
			cnt.predicate = ">";
		} else if (strPredicate == "n-gt") {
			cnt.isBinary = true;
			cnt.predicate = "<=";
		} else if (strPredicate == "n-eqgt") {
			cnt.isBinary = true;
			cnt.predicate = "<";
		} else if (strPredicate == "n-true") {
			cnt.isBinary = false;
			cnt.predicate = "<> TRUE";
		} else if (strPredicate == "n-false") {
			cnt.isBinary = true;
			cnt.predicate = "<> FALSE";
		} else if (strPredicate == "n-empty") {
			cnt.isBinary = false;
			cnt.predicate = "IS NOT NULL";
		}
	}

	void Searchable::FillSelectWhere(QueryStream& WhereClause) {
		Constraints.clear();
		for (size_t i = 0;; i++) {
			Constraint c;
			std::stringstream SStrI;
			SStrI << i;
			string StrI(SStrI.str());
			string fieldName = GetFormElement("fieldName[" + StrI + "]");
			if (fieldName.empty()) {
				break;
			}
			c.field = GetFieldByName(fieldName);
			FillPredicate(GetFormElement("predicate[" + StrI + "]"), c);
			if (c.predicate.empty()) {
				continue;
			}
			if (c.isBinary) {
				c.value = GetFormElement("value[" + StrI + "]");
			}
			Constraints.push_back(c);
			string param;
			if (c.isBinary) {
				std::stringstream SStrP;
				SStrP << selectQueryNextParamId++;
				param = " $" + SStrP.str();
			}
			string strField = TableName + "." + c.field->GetName();
			if (c.field->IsLink()) {
				strField = c.field->GetLinkTable() + "." + c.field->GetLinkValue();
				c.linkFieldType = DetectLinkValueType(c.field->GetLinkTable(), c.field->GetLinkValue());
			}
			WhereClause.Add(strField + " " + c.predicate + param, "AND");
		}
	}

	FieldType Searchable::DetectLinkValueType(string table, string field) {
		QueryStream Query;

		Query << "SELECT column_name, data_type, udt_name, character_maximum_length, numeric_precision \
		FROM information_schema.columns \
		WHERE table_name = '" << table << "' AND column_name = '" << field << "'";

		pqxx::work w(*DBConnection);
		pqxx::result r = w.exec(Query.str());
		w.commit();

		for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row) {
			return Types[row["udt_name"].as<std::string>()];
		}
	}

	void Searchable::BindSelectQueryParamTypes(pqxx::prepare::declaration& d) {
		for (std::list<Constraint>::iterator it = Constraints.begin(); it != Constraints.end(); ++it) {
			if (it->field->IsLink()) {
				d(it->linkFieldType.UDTName, it->linkFieldType.Treatment);
			} else {
				d(it->field->GetType().UDTName, it->field->GetType().Treatment);
			}
		}
	}

	void Searchable::BindSelectQueryParams(pqxx::prepare::invocation& inv) {
		for (std::list<Constraint>::iterator it = Constraints.begin(); it != Constraints.end(); ++it) {
			pqxx::prepare::param_treatment Treatment = it->field->GetType().Treatment;
			if (it->field->IsLink()) {
				Treatment = it->linkFieldType.Treatment;
			}
			if (Treatment == pqxx::prepare::treat_direct) {
				inv(atoi(it->value.c_str()));
			} else {
				inv(it->value.c_str());
			}
		}
	}

	bool Searchable::CatchAJAX() {
		string Action = GetFormElement(Scope("action"));
		if (!Action.empty()) {
			if (Action == "sql") {
				std::cout << GetSelectQuery();
			} else {
				return Simple::CatchAJAX();
			}
			return true;
		}
		return false;
	}

}; //namespace
