#include <iostream>

#include <cgicc/Cgicc.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHTMLHeader.h>

#include <pqxx/pqxx>

#include <report.h>

using namespace std;
using namespace cgicc;

int main () {

	cgicc::Cgicc* CGI = new cgicc::Cgicc();
	pqxx::connection* DBConnection = new pqxx::connection("host=dbhost port=5432 dbname=dbregulacao user=dbregulacao password=qualquercoisa");

	//Report Test(CGI,DBConnection,"pop","Population","SELECT * FROM countries WHERE population > [D Population from:] AND population < [N Population to:]");
	//Report Test(CGI,DBConnection,"pop","Population","SELECT * FROM regions \
	//					WHERE \
	//					id_country = [F Choose country,countries,id,country,country,true ]");

	Report Test(CGI,DBConnection,"cities","Cities By Country","SELECT * FROM cities WHERE \
									id_country = [F Country:,countries,id,country,country,1] AND \
									latitude > [N Latitude from:] AND \
									latitude < [N Latitude to:] AND \
									longitude > [N Longitude from:] AND \
									longitude < [N Longitude to:] "
									);

	//Report Test(CGI,DBConnection,"cities","Cities By Country","SELECT * FROM cities WHERE \
									//id_country = [F Country:,countries,id,country,country,true]");
//

	cout << HTTPHTMLHeader() << endl;
	if ( Test.CatchAJAX() ) {
		return 0;
	}


	cout << html() << head() << endl;
	cout << script().set("type","text/javascript").set("src","../report.js") << script() << endl;
	cout << script().set("type","text/javascript").set("src","../datetimepicker_css.js") << script() << endl;

	cout << head() << endl;
	cout << body().set("onload",Test.OnLoadJS()) << endl;

	cout << Test() << endl;

	cout << body() << html() << endl;
}
