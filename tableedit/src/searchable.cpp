#include <tableedit/searchable.h>

namespace TableEdit {

Field* Searchable::AddSearchField(std::string Name) {
	Field* t = GetFieldByName(Name);
	Fields.push_back(t);
	return t;
}

std::string Searchable::ReadForm() {
	using namespace cgicc;

	std::stringstream out;

	out << "Hello searchable" << br();

	out << Simple::ReadForm();
	return out.str();
}

}; //namespace
