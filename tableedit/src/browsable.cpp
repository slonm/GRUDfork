#include <tableedit/browsable.h>
#include <tableedit/querystream.h>

namespace TableEdit {

void Browsable::BrowseWith(std::string _PrimaryKey, std::string _ValueKey, std::string _OrderKey, bool _OrderAscending) {
	BrowseField = new Field((*GetFieldByName(_PrimaryKey)));
	BrowseField->SetLink(TableName,_PrimaryKey, _ValueKey, _OrderKey, _OrderAscending);
};

std::string Browsable::ReadForm() {
	using namespace cgicc;
	std::stringstream out;

	std::stringstream OnChange;
	OnChange << "window." << TableName << ".action('update',getElementById('"+Scope("browse")+"').value)";

	out << table();
	out << tr();
	out << th().set("colspan","2") << Caption << th();
	out << tr();

	out << tr();
	out << td().set("colspan","2") << BrowseField->GetInputHTML("",Scope("browse"),OnChange.str()) << td();
	out << tr();

	out << tr();
	out << td().set("colspan","2") << "Choose record to edit" << td();
	out << tr();
	

	out << tr();
	out << th().set("colspan","2");
	out << Button("create","New Record",".action('insert');") ;
	out << th();
	out << tr();

	out << table();

	return out.str();
};

std::string Browsable::UpdateForm() {
	using namespace cgicc;
	std::stringstream out;

	std::stringstream OnChange;
	OnChange << "window." << TableName << ".action('update',getElementById('"+Scope("browse")+"').value)";

	out << table();
	out << tr();
	out << th().set("colspan","2") << Caption << th();
	out << tr();

	out << tr();
	out << td().set("colspan","2") << BrowseField->GetInputHTML("",Scope("browse"),OnChange.str()) << td();
	out << tr();
	out << table();

	out << Simple::UpdateForm();
	return out.str();
}


std::string Browsable::OnLoadJS() { 
	std::stringstream params;
	params << " { ";
	params << "'name' : '"+TableName+"',  ";
	params << "'fields' : [";

	QueryStream FieldList;
	std::list<Field*>::iterator i;
	for (i = Fields.begin(); i!= Fields.end(); ++i) {
		FieldList.Add("'"+(*i)->GetName()+"'");
	}

	params << FieldList.str();
	params << " ]}";
	
	/*return "var t = new tableEdit.Browsable("+params.str()+"); \n" 
		+ "eval('window.'+ t.name + '=t'); \n"
		+ "t.run();"
		; 
		*/
	return "var t = new tableEdit.Simple("+params.str()+"); \n" 
		+ "window[t.name] = t;\n"
		+ "window[t.name].run()\n"
		; 
}

};
