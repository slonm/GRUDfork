#include <tableedit/searchable.h>
#include <tableedit/querystream.h>

namespace TableEdit {

//Field* Searchable::AddSearchField(std::string Name) {
//	Field* t = GetFieldByName(Name);
//	Fields.push_back(t);
//	return t;
//}

//std::string Searchable::ReadForm() {
//	using namespace cgicc;
//
//	std::stringstream out;
//
//	out << "Hello searchable" << br();
//
//	out << Simple::ReadForm();
//	return out.str();
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
	params << "'name' : '"+TableName+"',  ";
	params << "'fields' : [";

	QueryStream FieldList;
	std::list<Field*>::iterator i;
	for (i = Fields.begin(); i!= Fields.end(); ++i) {
		FieldList.Add("{ 'name': '"+(*i)->GetName()+"', 'type': '"+(*i)->GetType().UDTName+"' }");
	}

	params << FieldList.str();
	params << " ]}";
	return "(function (){var t = new tableEdit.Searchable("+params.str()+"); \n" 
		+ "window[t.name] = t;\n"
		+ "window[t.name].run();})();\n"
		; 
}


}; //namespace
