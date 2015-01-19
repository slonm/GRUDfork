#include <iostream>

#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHTMLHeader.h>

#include <pqxx/pqxx>

#include <tableedit/tableedit.h>

using namespace cgicc;
using namespace std;

//using namespace TableEdit;

int main () {

	cgicc::Cgicc* CGI = new cgicc::Cgicc();
	pqxx::connection* DBConnection = new pqxx::connection("host=localhost port=5432 dbname= user= password=");

	TableEdit::Browsable Countries (CGI,DBConnection,"countries");
	Countries.SetCaption("TableEdit::Browsable demo. Table 'countries'");
	Countries.AddField("id")->SetHidden()->SetOrderKey()->SetPrimaryKey();
	Countries.AddField("country");
	Countries.AddField("capital");
	Countries.AddField("population");
	Countries.AddField("internet");
	Countries.AddField("currency");
	Countries.BrowseWith("id","country","country",true);

	Countries.SetPageSize(5);
	Countries.SetCSS("cities");
	Countries.DetectTypes();



	TableEdit::Searchable Cities(CGI,DBConnection,"cities");
	Cities.SetCaption("TableEdit::Searchable demo. Table 'cities'.");
	Cities.AddField("id")->SetHidden()->SetOrderKey()->SetPrimaryKey();
	Cities.AddField("id_country")->SetLabel("Country")->SetLink("countries","id","country","country",true)->SetReadOnly();
	Cities.AddField("id_region")->SetLabel("Region")->SetLink("regions","id","region","region",true)->SetReadOnly();
	Cities.AddField("city");
	Cities.AddField("longitude");
	Cities.AddField("latitude");
	Cities.AddField("timezone");
	Cities.AddField("code");
	Cities.AddSearchField("id_country");
	Cities.AddSearchField("id_region");

	Cities.SetPageSize(5);
	Cities.SetCSS("cities");
	Cities.DetectTypes();

	TableEdit::Simple People(CGI,DBConnection,"people");
	People.SetCaption("TableEdit::Simple demo. Table 'people'.");
	People.AddField("id")->SetLabel("ID")->SetPrimaryKey()->SetOrderKey()->SetHidden()->SetReadOnly();
	People.AddField("name")->SetLabel("Name");
	People.AddField("lastname")->SetLabel("LastName");
	People.AddField("birthday")->SetLabel("BirthDay");
	People.AddField("eye_color")->SetLabel("Eye Color")->SetLink("colors","id","name","name",true);
	People.AddField("gender")->SetLabel("Sex")->SetLink("genders","id","name","name",true);

	People.SetPageSize(5);
	People.SetCSS("countries");
	People.DetectTypes();


	cout << HTTPHTMLHeader() << endl;
	if ( Countries.CatchAJAX() || Cities.CatchAJAX() || People.CatchAJAX() ) {
		return 0;
	}


	cout << html() << head() << endl;
	cout << script().set("type","text/javascript").set("src","../tableedit.js") << script() << endl;
	cout << script().set("type","text/javascript").set("src","../datetimepicker_css.js") << script() << endl;
	cout << cgicc::link().set("rel","stylesheet").set("type","text/css").set("href","../style.css") << endl;

	cout << head() << endl;

	cout << body().set("onload",Cities.OnLoadJS()+People.OnLoadJS()+Countries.OnLoadJS()) << endl;
	cout << Countries() << endl;
	cout << Cities() << endl;
	cout << People() << endl;
	cout << body() << html() << endl;

	return 0;
}
