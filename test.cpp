#include <iostream>

#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHTMLHeader.h>

#include <pqxx/pqxx>

#include <tableedit/tableedit.h>

using namespace cgicc;
using namespace std;

//using namespace TableEdit;

int main() {

	try {
		cgicc::Cgicc* CGI = new cgicc::Cgicc();
		pqxx::connection* DBConnection = new pqxx::connection("host=localhost port=5432 dbname=fork user=fork password=krof");

		TableEdit::Searchable Countries(CGI, DBConnection, "countries");
		Countries.SetCaption("TableEdit::Searchable demo. Table 'countries'");
		Countries.AddField("id")->SetHidden()->SetOrderKey()->SetPrimaryKey();
		Countries.AddField("country");
		Countries.AddField("capital");
		Countries.AddField("population");
		Countries.AddField("internet");
		Countries.AddField("currency");
		//Countries.BrowseWith("id", "country", "country", true);

		Countries.SetPageSize(5);
		Countries.SetCSS("cities");
		Countries.DetectTypes();



		TableEdit::Searchable Cities(CGI, DBConnection, "cities");
		Cities.SetCaption("TableEdit::Searchable demo. Table 'cities'.");
		Cities.AddField("id")->SetHidden()->SetOrderKey()->SetPrimaryKey();
		Cities.AddField("id_country")->SetLabel("Country")->SetLink("countries", "id", "country", "country", true)->SetReadOnly();
		Cities.AddField("id_region")->SetLabel("Region")->SetLink("regions", "id", "region", "region", true)->SetReadOnly();
		Cities.AddField("city");
		Cities.AddField("longitude");
		Cities.AddField("latitude");
		Cities.AddField("timezone");
		Cities.AddField("code");

		Cities.SetPageSize(5);
		Cities.SetCSS("countries");
		Cities.DetectTypes();

		TableEdit::Searchable People(CGI, DBConnection, "people");
		People.SetCaption("TableEdit::Searchable demo. Table 'people'.");
		People.AddField("id")->SetLabel("ID")->SetPrimaryKey()->SetOrderKey()->SetHidden()->SetReadOnly();
		People.AddField("name")->SetLabel("Name");
		People.AddField("lastname")->SetLabel("LastName");
		People.AddField("birthday")->SetLabel("BirthDay");
		People.AddField("eye_color")->SetLabel("Eye Color")->SetLink("colors", "id", "name", "name", true);
		People.AddField("gender")->SetLabel("Sex")->SetLink("genders", "id", "name", "name", true);

		People.SetPageSize(5);
		People.SetCSS("countries");
		People.DetectTypes();

		TableEdit::Searchable Regions(CGI, DBConnection, "regions");
		Regions.SetCaption("TableEdit::Searchable demo. Table 'regions'.");
		Regions.AddField("id")->SetLabel("ID")->SetPrimaryKey()->SetOrderKey()->SetHidden()->SetReadOnly();
		Regions.AddField("region")->SetLabel("Region");
		Regions.SetPageSize(5);
		Regions.SetCSS("countries");
		Regions.DetectTypes();

		TableEdit::Constructor constructor(CGI, DBConnection, "c1");
		constructor.SetCSS("countries");
		constructor.AddTable(Countries)
						.AddTable(Cities)
						.AddTable(People)
						.AddTable(Regions);

		cout << HTTPHTMLHeader() << endl;
		if (Countries.CatchAJAX() || Cities.CatchAJAX() || People.CatchAJAX() || Regions.CatchAJAX() || constructor.CatchAJAX()) {
			return 0;
		}


		cout << html() << head() << endl;
		cout << script().set("type", "text/javascript").set("src", "http://code.jquery.com/jquery-2.1.3.min.js") << script() << endl;
		cout << script().set("type", "text/javascript").set("src", "../tableedit.js") << script() << endl;
		cout << script().set("type", "text/javascript").set("src", "../datetimepicker_css.js") << script() << endl;
		cout << script().set("type", "text/javascript").set("src", "http://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js") << script() << endl;
		cout << cgicc::link().set("rel", "stylesheet").set("type", "text/css").set("href", "http://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css") << endl;
		cout << cgicc::link().set("rel", "stylesheet").set("type", "text/css").set("href", "../style.css") << endl;

		cout << head() << endl;

//		cout << body().set("onload", Cities.OnLoadJS() + People.OnLoadJS() + Countries.OnLoadJS() + constructor.OnLoadJS()) << endl;
		cout << body().set("onload", constructor.OnLoadJS()) << endl;
		cout << constructor() << endl;
//		cout << br() << br();
//		cout << Countries() << endl;
//		cout << br() << br();
//		cout << Cities() << endl;
//		cout << br() << br();
//		cout << People() << endl;
		cout << body() << html() << endl;
	} catch (std::exception& ex) {
		cout << ex.what() << endl;
	}

	return 0;
}
