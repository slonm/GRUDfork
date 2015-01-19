#include <tableedit/field.h>

namespace TableEdit {

	Field::Field(std::string Name): Name(Name) {
		PrimaryKey = false;
		OrderKey = false;
		ReadOnly = false;
		Label = Name;
		Link = false;
		LinkTable = "";
		LinkKey = "";
		LinkValue = "";
		LinkOrder = "";
		LinkAscending=false;
		Hidden=false;
		Searchable = false;
		SearchType = stMatch;
	}

	Field* Field::SetLink(std::string TableName, std::string KeyField, std::string ValueField, std::string OrderField, bool OrderAsc) {
		Link = true;
		LinkTable = TableName;
		LinkKey = KeyField;
		LinkValue = ValueField;
		LinkOrder = OrderField;
		LinkAscending =  OrderAsc;
		return this;
	}

	std::string Field::GetInputHTML(std::string Value, std::string Id, std::string OnChange ) {
		std::stringstream out,Width;
		cgicc::HTMLAtomicElement<cgicc::inputTag> inp;

		if ( Type.UDTName == "date" ) {
			inp.set("type","Text").set("id",Table->Scope("field-"+Name));
			inp.set("size","25").set("maxlength","25");
			if ( Value != "" ) {
				inp.set("value",Value);
			}
			out << inp;
			out << cgicc::img().set("src","/images/cal.gif").set("onclick","javascript:NewCssCal('"+Table->Scope("field-"+Name)+"')").set("style","cursor:pointer");
		} else if (Link == true ) {
			cgicc::HTMLBooleanElement<cgicc::selectTag> sel;
			cgicc::HTMLBooleanElement<cgicc::optionTag> opt;
			std::stringstream Query;
			Query << "SELECT " << LinkKey << "," << LinkValue << " FROM " << LinkTable << " ORDER BY " << LinkOrder;

			if (LinkAscending == true) {
				Query << " ASC ";
			} else {
				Query << " DESC ";
			}

			pqxx::work w(*Table->GetDBConnection());
			pqxx::result r = w.exec(Query.str());
			w.commit();
			if ( Id == "" ) {
				sel.set("id",Table->Scope("field-"+Name));
			} else {
				sel.set("id",Id);
			}
			if ( OnChange != "" ) {
				sel.set("OnChange",OnChange);
			}
			out << sel;

			opt.set("disabled").set("value","select");
			if ( Value == "" ) {
				opt.set("selected");
			}
			out << opt;
			out << "-Select-";	
			out << opt;


			for (pqxx::result::const_iterator row = r.begin(); row != r.end(); ++row) {
				cgicc::HTMLBooleanElement<cgicc::optionTag> opt;
				opt.set("value",row[0].as<std::string>());
				if(Value == row[0].as<std::string>()) {
					opt.set("selected");
				}
				out << opt;
				out << row[1].as<std::string>();
				out << opt;
			}

			out << sel;
		} else if ( IsHidden() ) {
			inp.set("type","hidden").set("id",Table->Scope("field-"+Name));
			if ( Value != "" ) {
				inp.set("value",Value);
			}
			out << inp;
		} else {
			Width << CharLength;
			inp.set("type","edit").set("name",Name).set("id",Table->Scope("field-"+Name));
			inp.set("size",Width.str()).set("maxlength",Width.str());
			inp.set("value",Value);
			out << inp;
		}
		return out.str();
	}


}; // namespace
