#include <tableedit/simple.h>
#include <tableedit/querystream.h>

namespace TableEdit {

Simple::Simple(cgicc::Cgicc* CGI, pqxx::connection* DBConnection, std::string TableName)
	: CGI(CGI), DBConnection(DBConnection), TableName(TableName){ 

	PageSize = 5;
	Caption = TableName;
};

std::string Simple::operator()() {
	std::stringstream out;
	out << cgicc::div().set("id",Scope("container")).set("class",CSSClass) << std::endl;
	out << cgicc::div() << std::endl;
	return out.str();
};

Field* Simple::GetFieldByName(std::string Name) {
	std::list<Field*>::iterator i;
	for (i = Fields.begin(); i!= Fields.end(); ++i) {
		if ( (*i)->GetName() == Name ) {
			return *i;
		}
	}
	i = Fields.begin();
	return *i;
}

Field* Simple::GetPrimaryKey() {
	std::list<Field*>::iterator i;
	for (i = Fields.begin(); i!= Fields.end(); ++i) {
		if ( (*i)->IsPrimaryKey() ) {
			return *i;
		}
	}
	i = Fields.begin();
	return *i;
}

Field* Simple::AddField(std::string Name) {
	Field* t = new Field(Name);
	t->SetTableEdit(this);
	Fields.push_back(t);
	return GetFieldByName(Name);
}

std::string Simple::GetFormElement(std::string Name) {
	cgicc::form_iterator i = CGI->getElement(Name);
	if(i != CGI->getElements().end()) {
		return i->getValue();
	}	
	return "";
}

std::string Simple::Scope(std::string Name) {
	return "tableedit-"+TableName+"-"+Name;
}


std::string Simple::OnLoadJS() { 
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
	
	//return "var t = new tableEdit.Simple("+params.str()+"); \n" 
	//	+ "eval('window.'+ t.name + '=t'); \n"
	//	+ "alert('onload');\n"
	//	+ "t.run();"
	//	; 
	return "var t = new tableEdit.Simple("+params.str()+"); \n" 
		+ "window[t.name] = t;\n"
		+ "window[t.name].run()\n"
		; 
}

std::string Simple::Button(std::string Id, std::string Text, std::string JSFunction ) {
	using namespace cgicc;
	std::stringstream OnClick, out;

	if ( Text == "" ) { Text = "&nbsp;"; }

	OnClick << "window." << TableName << JSFunction;
	out << button().set("id",Scope(Id)).set("onclick",OnClick.str());
	out << span().set("id",Id+"-limg") << "&nbsp;" << span(); // left image span
	out << span().set("id",Id+"-text") << Text << span(); //text span
	out << span().set("id",Id+"-rimg") << "&nbsp;" << span(); //right image span
	out << button() << std::endl;

	return out.str();
};

void Simple::DetectTypes() {
	QueryStream Query;

	Query << "SELECT column_name, data_type, udt_name, character_maximum_length, numeric_precision \
		FROM information_schema.columns \
		WHERE table_name = '" << TableName
		<< "' AND column_name IN (";

	for ( std::list<Field*>::iterator Field = Fields.begin(); Field != Fields.end(); ++Field) {
		Query.Add("'"+(*Field)->GetName()+"'");
	}
	Query << " )";

	pqxx::work w(*DBConnection);
	pqxx::result r = w.exec(Query.str());
	w.commit();

	for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row) {
		Field* t = GetFieldByName(row["column_name"].as<std::string>());
		t->SetType(Types[row["udt_name"].as<std::string>()]);

		t->SetCharLength(11);
		std::stringstream charlen;
		charlen << row["character_maximum_length"];

		if ( charlen.str() != "" ) {
			t->SetCharLength(row["character_maximum_length"].as<int>());
		}
	}

}

std::string Simple::GetSelectQuery() {
	std::stringstream Query;
	QueryStream FieldsClause, TablesClause, WhereClause, OrderClause;
	std::map<std::string,std::string> Tables;

	bool HasLinks = false;
	bool HasOrder = false;

	Tables.insert(std::pair<std::string,std::string>(TableName,TableName));
	for ( std::list<Field*>::iterator i = Fields.begin(); i != Fields.end(); ++i) {
		Field *Field = *i;
		if ( Field->IsLink() ) {
			FieldsClause.Add( Field->GetLinkTable()+"."+Field->GetLinkValue()  );
			Tables.insert(std::pair<std::string,std::string>(Field->GetLinkTable(),Field->GetLinkTable()));
			WhereClause.Add( Field->GetLinkTable()+"."+Field->GetLinkKey()+"="+TableName+"."+Field->GetName(), " AND ");
			HasLinks = true;
		} else {
			FieldsClause.Add( TableName+"."+Field->GetName() );
		}
		if ( Field->IsOrderKey() ) {
			HasOrder = true;
			OrderClause << " ORDER BY " << TableName << "." << Field->GetName() ;
		}
	}

	for ( std::map<std::string,std::string>::const_iterator it = Tables.begin(); it != Tables.end(); ++it) {
		TablesClause.Add(it->first);
	}

	Query << "SELECT " << FieldsClause.str() << " FROM " << TablesClause.str();

	if ( HasLinks == true ) {
		Query << " WHERE " << WhereClause.str();
	}

	if ( HasOrder == true ) {
		Query << OrderClause.str();
	}

	return Query.str();
}


std::string Simple::GetSelectForUpdateQuery(std::string id) {
	QueryStream Query;

	Query << "SELECT ";

	for ( std::list<Field*>::iterator i = Fields.begin(); i != Fields.end(); ++i) {
		Query.Add((*i)->GetName());
	}
	Query << " FROM " << TableName << " WHERE " << GetPrimaryKey()->GetName() << " = " << id;
	return Query.str();
}


std::string Simple::Pagination() {
	std::stringstream out;
	out << " LIMIT " << PageSize << " OFFSET " << PageSize*( atoi(GetFormElement(Scope("page")).c_str()) -1);
	return out.str();
}

std::string Simple::ReadForm() {
	using namespace std;
	using namespace cgicc;

	stringstream out;
	stringstream Colspan; 
	Colspan << Fields.size()+2;
	out << table() << endl;


	out << tr();
	out << th().set("colspan",Colspan.str());
	out << Caption;
	out << th();
	out << tr();

	out << tr() << endl;
	for (std::list<Field*>::iterator Field = Fields.begin(); Field!=Fields.end(); ++Field) {
		if ( (*Field)->IsHidden() == true ) {
			out << th() << th() << endl;
		} else {
			out << th() << (*Field)->GetLabel() << th() << endl;
		}
	}

	out << th() << "Delete" << th();
	out << th() << "Edit" << th();
	out << tr() << endl;



	pqxx::work w(*DBConnection);
	pqxx::result r = w.exec(GetSelectQuery()+Pagination());
	w.commit();

	stringstream KeyValue;

	for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row) {
		out << tr();
		std::list<Field*>::iterator i = Fields.begin();
		for (pqxx::result::tuple::const_iterator field = row->begin(); field != row->end(); ++field) {
			if ( (*i)->IsPrimaryKey() == true) {
				KeyValue.str("");
				KeyValue << field->c_str();
			}
			string value = field->c_str();
			encode(value);
			if ( (*i)->IsHidden() == true) {
				out << td() << td();
			} else {
				out << td() << value << td();
			}
			++i;
		}

		out << td();
		out << Button("delete","",".action('delete','" + KeyValue.str() + "');");
		out << td();
		out << td();
		out << Button("update","",".action('update','" + KeyValue.str() + "');");
		out << td();

		out << tr();
	}
	out << tr();
	out << td().set("colspan",Colspan.str());

	out << Button("prev","",".prevPage();") << endl;
	out << "Page " << GetFormElement(Scope("page"));
	out << Button("next","",".nextPage();") << endl;
	out << Button("create","New Record",".action('insert');") << endl;
	out << Button("refresh","Refresh",".action('select');") << endl;

	out << td();
	out << tr();
	out << table();

	return out.str();
}

std::string Simple::InsertForm() {
	using namespace cgicc;
	using namespace std;

	stringstream out;


	out << table() << endl;

	out << th().set("colspan","2");
	out << Caption << " New Record";
	out << th();

	QueryStream OnClick;
	OnClick << "window."+TableName+".commitCreate([";

	for ( std::list<Field*>::iterator i = Fields.begin(); i != Fields.end(); ++i) {
		Field* Field = *i;
		if(Field->IsHidden()) {
			out << Field->GetInputHTML();
			continue;
		}
		OnClick.Add("'"+Field->GetName()+"'");
		out << tr() << endl;
		out << td() << Field->GetLabel() << td() << endl;

		out << td();
		out << Field->GetInputHTML();
		out << td() << endl;

		out << tr() << endl;
	}
	OnClick << "]);";

	
	out << tr();
	out << td().set("colspan","2");
	out << input().set("value","Save").
			set("type","submit").
			set("onclick","window."+TableName+".action('commit-insert');");

	out << input().set("value","Cancel").
			set("type","submit").
			set("onclick","window."+TableName+".action('select');");
	out << td();
	out << tr();

	out << table() << endl;

	return out.str();

};

bool Simple::CommitInsert() {
	QueryStream Query;

	Query << "INSERT INTO " << TableName << " (";

	for ( std::list<Field*>::iterator it = Fields.begin(); it != Fields.end(); ++it) {
		if ( (*it)->IsReadOnly() ) {
			continue;
		}
		Query.Add((*it)->GetName());
	}

	Query << ") VALUES ( ";

	int i=1;
	Query.New();
	for ( std::list<Field*>::iterator it = Fields.begin(); it != Fields.end(); ++it) {
		if ( (*it)->IsReadOnly() ) {
			continue;
		} else if ( (*it)->IsHidden() ) {
			Query.Add("DEFAULT");

		} else {
			Query.Add("$");
			Query << i;
			i++;
		}
	}

	Query << " )";


	pqxx::work w(*DBConnection);
	pqxx::prepare::declaration d(DBConnection->prepare("q",Query.str()));

	for ( std::list<Field*>::iterator it = Fields.begin(); it != Fields.end(); ++it) {
		if ( (*it)->IsReadOnly() ) {
			continue;
		}
		if ( (*it)->IsHidden() ) {
			continue;
		}
		d((*it)->GetType().UDTName, (*it)->GetType().Treatment);
	}

	pqxx::prepare::invocation inv ( w.prepared("q") );

	try {
	for ( std::list<Field*>::iterator it = Fields.begin(); it != Fields.end(); ++it) {
		std::string val = GetFormElement(Scope("field-"+(*it)->GetName()));
		if ( (*it)->IsReadOnly() ) {
			continue;
		}
		if ( (*it)->IsHidden()) {
			continue;
		}
		if((*it)->GetType().Treatment == pqxx::prepare::treat_direct) {
			inv(atoi(val.c_str()));
		} else {
			inv(val);
		}
	}

	pqxx::result r = inv.exec();
	w.commit();
	} catch (std::exception& ex) {
		std::cout << "exception: " << ex.what() << std::endl;
	}

	return true;
};

std::string Simple::UpdateForm() {
	using namespace cgicc;

	std::stringstream out;
	std::string UpdateId = GetFormElement(Scope("update-id"));

	//out << th() << "Update Record" << th() << std::endl;
	out << table() << std::endl;

	pqxx::work w(*DBConnection);
	pqxx::result r = w.exec(GetSelectForUpdateQuery(UpdateId));
	w.commit();

	//out << GetSelectForUpdateQuery(UpdateId);

	pqxx::result::const_iterator row = r.begin(); //row++;
	pqxx::result::tuple::const_iterator field = row->begin();// field++;

	for ( std::list<Field*>::iterator i = Fields.begin(); i != Fields.end(); ++i) {
		Field *Field = *i;
		std::string value = field->c_str();
		encode(value);

		if(Field->IsHidden()) {
			out << Field->GetInputHTML(value);
			field++;
			continue;
		}

		out << tr();
		out << td() << Field->GetLabel() << td();

		out << td();
		out << Field->GetInputHTML(value);
		out << td() << std::endl;
		out << tr();
		field++;
	}

		
	out << tr();
	out << td().set("colspan","2");
	out << input().set("value","Save").
			set("type","submit").
			set("onclick","window."+TableName+".action('commit-update','" + GetFormElement(Scope("update-id")) + "');");
	out << input().set("value","Cancel").set("type","submit").set("onclick","window."+TableName+".action('select');") ;
	out << td();
	out << tr();
	out << table();

	return out.str();
};

bool Simple::CommitUpdate() {

	QueryStream Query;
	std::string UpdateId = GetFormElement(Scope("update-id"));

	Query << "UPDATE " << TableName << " SET ";

	int i=1;
	for ( std::list<Field*>::iterator Field = Fields.begin(); Field != Fields.end(); ++Field) {
		if ((*Field)->IsReadOnly()) {
			continue;
		} else {
			Query.Add((*Field)->GetName());
			Query << " = $" << i;
			i++;
		}
	}

	Query << " WHERE " << GetPrimaryKey()->GetName() << " = " << UpdateId;

	try{

	pqxx::work w(*DBConnection);
	pqxx::prepare::declaration d(DBConnection->prepare("u",Query.str()));

	for ( std::list<Field*>::iterator Field = Fields.begin(); Field != Fields.end(); ++Field) {
		if( (*Field)->IsReadOnly()) {
			continue;
		} else {
			d((*Field)->GetType().UDTName, (*Field)->GetType().Treatment);
		}
	}

	pqxx::prepare::invocation inv ( w.prepared("u") );

	for ( std::list<Field*>::iterator Field = Fields.begin(); Field != Fields.end(); ++Field) {
		std::string val = GetFormElement(Scope("field-"+(*Field)->GetName()));
		if((*Field)->IsReadOnly()) {
			continue;
		}
		if((*Field)->GetType().Treatment == pqxx::prepare::treat_direct) {
			inv(atoi(val.c_str()));
		} else {
			inv(val);
		}
	}

	pqxx::result r = inv.exec();
	w.commit();

	} catch (std::exception& ex) {
		std::cout << "exception: " << ex.what();
		return false;
	}

	return true;

};

std::string Simple::DeleteForm() {
	using namespace cgicc;
	std::stringstream out;
	out << table();
	out << tr() << th();
	out << Caption << " Delete Record";
	out << th() << tr();
	out << tr() << td();
	out << "Are you sure want to delete record?"  << std::endl;
	out << td() << tr();

	out << tr() << td();
	out << input().set("value","Delete").set("type","submit").
		set("onclick","window."+TableName+".action('commit-delete','" + GetFormElement(Scope("delete-id")) + "');");
	out << input().set("value","Cancel").set("type","submit").set("onclick","window."+TableName+".action('select');") ;
		//out << Button("delete","",".action('delete','" + KeyValue.str() + "');");
	out << td() << tr();
	return out.str();

};

bool Simple::CommitDelete() {
	std::stringstream Query;
	std::string id =  GetFormElement(Scope("delete-id"));
	Query << "DELETE FROM " << TableName << " WHERE " << GetPrimaryKey()->GetName() << "=" << id;


	try {

	pqxx::work w(*DBConnection);
	pqxx::result r = w.exec(Query.str());
	w.commit();

	} catch (std::exception& ex) {
		std::cout << "exception: " << ex.what();
		return false;
	}

	return true;
};

bool Simple::CatchAJAX() {
	if ( GetFormElement(Scope("action")) != "" ) {
		std::string Action = GetFormElement(Scope("action"));
		
		if (Action == "insert") {
			std::cout << InsertForm();
		}
		else if (Action == "commit-insert") {
			CommitInsert();
		}
		else if (Action == "read") {
			std::cout << ReadForm();
		}
		else if (Action == "update") {
			std::cout << UpdateForm();
		}
		else if (Action == "commit-update") {
			CommitUpdate();
		}
		else if (Action == "delete") {
			std::cout << DeleteForm();
		}
		else if (Action == "commit-delete") {
			std::cout << CommitDelete();
		}
		else {
		//ReadForm();
		}

		return true;
	}

	return false;
}


}; // namespace
