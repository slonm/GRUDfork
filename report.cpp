#include "report.h"

Param::Param(Report* ParentReport) :
		Type(ptNumeric), Length(11), Link(false), LinkTable(""), LinkKey(""), LinkValue(
				""), LinkOrder(""), Filtro(""), LinkAscending(true), ParentReport(
				ParentReport) {

}
;

std::string Param::GetInputHTML(std::string Value, std::string Id,
		std::string OnChange) {
	std::stringstream out, Width;
	cgicc::HTMLAtomicElement < cgicc::inputTag > inp;

	if (Type == ptDate) {
		out
				<< "<input type=\"text\" id=\""
						+ ParentReport->Scope("param-" + Name) + "\" name=\""
						+ ParentReport->Scope("param-" + Name)
						+ "\" size=\"10\" maxlength=\"10\" onkeydown=\"return mascara(this, '99+/99+/9999', event);\" /> "
								"<A HREF=\"#\" onClick=\"dataF=document.getElementById('"+ ParentReport->Scope("param-" + Name) +"');cal.select(document.forms['form2'].dataI,'"
						+ Name + "a"
						+ "','dd/MM/yyyy');\" NAME=\""
						+ Name + "a"
						+ "\" ID=\"" + Name
						+ "a" + "\">"
						"<IMG SRC=\"/imagens/calendar.gif\" BORDER=0></A>";
	} else if (Type == ptForeign) {
		cgicc::HTMLBooleanElement < cgicc::selectTag > sel;
		cgicc::HTMLBooleanElement < cgicc::optionTag > opt;
		std::stringstream Query;
		Query << "SELECT " << LinkKey << "," << LinkValue << " FROM "
				<< LinkTable << " WHERE " << Filtro << " ORDER BY "
				<< LinkOrder;

		if (LinkAscending == true) {
			Query << " ASC ";
		} else {
			Query << " DESC ";
		}

		pqxx::work w(*ParentReport->GetDBConnection());
		pqxx::result r = w.exec(Query.str());
		w.commit();
		if (Id == "") {
			sel.set("id", ParentReport->Scope("param-" + Name));
		} else {
			sel.set("id", Id);
		}
		if (OnChange != "") {
			sel.set("OnChange", OnChange);
		}
		out << sel;

		opt.set("disabled").set("value", "select");
		if (Value == "") {
			opt.set("selected");
		}
		out << opt;
		out << "-Selecione-";
		out << opt;

		for (unsigned int ind = 0; ind < r.size(); ind++) {
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt;
			opt.set("value", r[ind][0].as<std::string>());
			if (Value == r[ind][0].as<std::string>()) {
				opt.set("selected");
			}
			out << opt;
			out << r[ind][1].as<std::string>();
			out << opt;
		}

		out << sel;
		/*
		 } else if ( IsHidden() ) {
		 inp.set("type","hidden").set("id",Table->Scope("field-"+Name));
		 if ( Value != "" ) {
		 inp.set("value",Value);
		 }
		 out << inp;
		 */
	} else if (Type == ptArray) {
		cgicc::HTMLBooleanElement < cgicc::selectTag > sel;

		out
				<< sel.set("name", Name).set("id",
						ParentReport->Scope("param-" + Name));
		if (Label.compare("Mes") == 0) {
			pqxx::work w(*ParentReport->GetDBConnection());
			pqxx::result r = w.exec("SELECT to_char(current_date,'mm')");
			w.commit();
			std::string _str_data = r[0][0].c_str();
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt1;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt2;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt3;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt4;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt5;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt6;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt7;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt8;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt9;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt10;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt11;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt12;
			opt1.set("value", "01");
			if (_str_data.compare("01") == 0)
				opt1.set("selected", "true");
			out << opt1;
			out << "JAN";
			out << opt1;
			opt2.set("value", "02");
			if (_str_data.compare("02") == 0)
				opt2.set("selected", "true");
			out << opt2;
			out << "FEV";
			out << opt2;
			opt3.set("value", "03");
			if (_str_data.compare("03") == 0)
				opt3.set("selected", "true");
			out << opt3;
			out << "MAR";
			out << opt3;
			opt4.set("value", "04");
			if (_str_data.compare("04") == 0)
				opt4.set("selected", "true");
			out << opt4;
			out << "ABR";
			out << opt4;
			opt5.set("value", "05");
			if (_str_data.compare("05") == 0)
				opt5.set("selected", "true");
			out << opt5;
			out << "MAI";
			out << opt5;
			opt6.set("value", "06");
			if (_str_data.compare("06") == 0)
				opt6.set("selected", "true");
			out << opt6;
			out << "JUN";
			out << opt6;
			opt7.set("value", "07");
			if (_str_data.compare("07") == 0)
				opt7.set("selected", "true");
			out << opt7;
			out << "JUL";
			out << opt7;
			opt8.set("value", "08");
			if (_str_data.compare("08") == 0)
				opt8.set("selected", "true");
			out << opt8;
			out << "AGO";
			out << opt8;
			opt9.set("value", "09");
			if (_str_data.compare("09") == 0)
				opt9.set("selected", "true");
			out << opt9;
			out << "SET";
			out << opt9;
			opt10.set("value", "10");
			if (_str_data.compare("10") == 0)
				opt10.set("selected", "true");
			out << opt10;
			out << "OUT";
			out << opt10;
			opt11.set("value", "11");
			if (_str_data.compare("11") == 0)
				opt11.set("selected", "true");
			out << opt11;
			out << "NOV";
			out << opt11;
			opt12.set("value", "12");
			if (_str_data.compare("12") == 0)
				opt12.set("selected", "true");
			out << opt12;
			out << "DEZ";
			out << opt12;
		} else if (Label.compare("Ano") == 0) {
			pqxx::work w(*ParentReport->GetDBConnection());
			pqxx::result r = w.exec("SELECT to_char(current_date,'yyyy')");
			w.commit();
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt1;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt2;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt3;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt4;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt5;
			cgicc::HTMLBooleanElement < cgicc::optionTag > opt6;
			std::string _str_data = r[0][0].c_str();
			opt1.set("value", "2011");
			if (_str_data.compare("2011") == 0)
				opt1.set("selected", "true");
			out << opt1;
			out << "2011";
			out << opt1;
			opt2.set("value", "2012");
			if (_str_data.compare("2012") == 0)
				opt2.set("selected", "true");
			out << opt2;
			out << "2012";
			out << opt2;
			opt3.set("value", "2013");
			if (_str_data.compare("2013") == 0)
				opt3.set("selected", "true");
			out << opt3;
			out << "2013";
			out << opt3;
			opt4.set("value", "2014");
			if (_str_data.compare("2014") == 0)
				opt4.set("selected", "true");
			out << opt4;
			out << "2014";
			out << opt4;
			opt5.set("value", "2015");
			if (_str_data.compare("2015") == 0)
				opt5.set("selected", "true");
			out << opt5;
			out << "2015";
			out << opt5;
			opt6.set("value", "2016");
			if (_str_data.compare("2016") == 0)
				opt6.set("selected", "true");
			out << opt6;
			out << "2016";
			out << opt6;
		} else {
			for (unsigned int j = 0; j < Array.size(); j++) {
				cgicc::HTMLBooleanElement < cgicc::optionTag > opt;
				opt.set("value", Array[j]);
				out << opt;
				out << Array[j];
				out << opt;
			}
		}
		out << sel;
	} else {
		Width << Length;
		//out << Length;
		inp.set("type", "edit").set("name", Name).set("id",
				ParentReport->Scope("param-" + Name));
		inp.set("size", Width.str()).set("maxlength", Width.str());
		inp.set("value", Value);
		out << inp;
	}
	return out.str();
}

Report::Report(cgicc::Cgicc* CGI, pqxx::connection* DBConnection,
		std::string ReportName, std::string Title, std::string Query,
		std::string Query1) :
		CGI(CGI), DBConnection(DBConnection), ReportName(ReportName), Title(
				Title), InputQuery(Query), Query1(Query1) {

	CSSClass = "";

	ParseInput();
}

std::string Report::Trim(std::string str) {
	size_t pos1 = str.find_first_not_of(' ');
	size_t pos2 = str.find_last_not_of(' ');
	str = str.substr(pos1 == std::string::npos ? 0 : pos1,
			pos2 == std::string::npos ? str.length() - 1 : pos2 - pos1 + 1);
	return str;
}

void Report::AddParam(ParamTypes Type, std::string Label) {
	Param* t = new Param(this);
	t->Type = Type;
	t->Label = Label;
	Params.push_back(t);
}

void Report::AddArrayParam(std::string str, std::string Label) {
	size_t Found;
	std::vector < std::string > Results;

	Found = str.find_first_of(',');
	while (Found != std::string::npos) {
		if (Found > 0) {
			Results.push_back(str.substr(0, Found));
		}
		str = str.substr(Found + 1);
		Found = str.find_first_of(',');
	}

	if (str.length() > 0) {
		Results.push_back(str);
	}

	Param* t = new Param(this);
	t->Type = ptArray;
	t->Label = Label;

	for (int i = 0; i < Results.size(); i++) {
		t->Array.push_back(Results[i]);
	}

	Params.push_back(t);
}

void Report::AddForeignParam(std::string str, std::string Label) {
	size_t Found;
	std::vector < std::string > Results;

	Found = str.find_first_of(',');
	while (Found != std::string::npos) {
		if (Found > 0) {
			Results.push_back(str.substr(0, Found));
		}
		str = str.substr(Found + 1);
		Found = str.find_first_of(',');
	}

	if (str.length() > 0) {
		Results.push_back(str);
	}

	Param* t = new Param(this);
	t->Type = ptForeign;
	t->Label = Label;

	t->Link = true;
	t->LinkTable = Results[0];
	t->LinkKey = Results[1];
	t->LinkValue = Results[2];
	t->LinkOrder = Results[3];
	if (Results.size() == 6)
		t->Filtro = Results[5];
	else
		t->Filtro = "1=1";

	std::istringstream Stream(Results[4]);
	Stream >> t->LinkAscending;

//	std::cout << "|" << Results[4] << "|" << t->LinkAscending << "|" << std::endl;

	Params.push_back(t);
}

void Report::AddTextParam(std::string str, std::string Label) {
	Param* t = new Param(this);
	t->Type = ptText;
	t->Label = Label;

	std::istringstream Stream(str);
	Stream >> t->Length;

	Params.push_back(t);
}

void Report::ParseParam(std::string str) {
	char Type;

	size_t Found;
	std::string Label;

	Type = str[0];
	str = str.substr(1, str.size() - 1);

	Found = str.size();
	Found = str.find_first_of(',');

	Label = Trim(str.substr(0, Found));

	if (Found == std::string::npos) {
		str = "";
	} else {
		str = str.substr(Found + 1, str.size() - Found);
	}

	//std::cout << "|" << Label << "|" << std::endl;

	switch (Type) {
	case 'A':
		AddArrayParam(str, Label);
		break;
	case 'D':
		AddParam(ptDate, Label);
		break;
	case 'F':
		AddForeignParam(str, Label);
		break;
	case 'N':
		AddParam(ptNumeric, Label);
		break;
	case 'T':
		AddTextParam(str, Label);
		break;
	default:
		break;
	}

}

void Report::SubstituteParams() {
	Query = "";
	std::stringstream t;
	int ParamNum = 1;

	bool Push = false;
	bool repete = false;

	for (int i = 0; i < InputQuery.length(); i++) {
		if (InputQuery[i] == '[') {
			Push = true;
			if(repete){
				Push = false;
			}
			repete = true;
		}else
			repete = false;
		if (Push == false) {
			t << InputQuery[i];
		} else {
			if (InputQuery[i] == ']') {
				t << "$" << ParamNum;
				ParamNum++;
				Push = false;
			}
		}
	}

	Query = t.str();
	//std::cout << Query << std::endl;
}

void Report::ParseInput() {
	size_t Found = 0;
	size_t LeftPos, RightPos;

	//!!!
	//std::cout << InputQuery << std::endl;
	std::string ParamString;

	while (Found != std::string::npos) {

		Found++;
		Found = InputQuery.find("[", Found);
		LeftPos = Found;
		//std::cout << "L" << LeftPos << " ";

		if (LeftPos == std::string::npos) {
			continue;
		}

		Found++;
		Found = InputQuery.find("]", Found);
		RightPos = Found;
		//std::cout << "R" << RightPos << " " << std::endl;

		if (RightPos == std::string::npos) {
			continue;
		}

		if ((LeftPos != std::string::npos) && (RightPos != std::string::npos)) {
			ParamString = InputQuery.substr(LeftPos + 1,
					RightPos - LeftPos - 1);
			//std::cout << ParamString << std::endl;
			ParseParam(ParamString);
		}

	}

	SubstituteParams();

}

std::string Report::Scope(std::string Name) {
	return "report-" + ReportName + "-" + Name;
}

std::string Report::OnLoadJS() {
	std::stringstream params;
	//params << " { ";
	//params << "'name' : '" + ReportName + "',  ";
	//params << "'params' : [";

	params << "[";

	QueryStream ParamList;
	for (int i = 0; i < Params.size(); i++) {
		std::stringstream param;
		param << "'param" << i << "'";
		ParamList.Add(param.str());
	}

	params << ParamList.str();
	params << "]";

	std::string saida = "name1='"+ReportName+"';params=" + params.str() + ";script = document.location;container[0]=document.getElementById('report-cities-container');"
			"container[1]=document.getElementById('div2');"
			"container[2]=document.getElementById('div3');"
			"container[3]=document.getElementById('div4');"
			"container[4]=document.getElementById('div5');"
			"container[5]=document.getElementById('div6');"
			"container[6]=document.getElementById('div7');"
			"container[7]=document.getElementById('div8');"
			"container[8]=document.getElementById('div9');"
			"container[9]=document.getElementById('div10');"
			"container[10]=document.getElementById('div11');"
			"container[11]=document.getElementById('div12');"
			"container[12]=document.getElementById('div13');"
			"container[13]=document.getElementById('div14');"
			"container[14]=document.getElementById('div15');run();";

	return saida;
}

std::string Report::GetFormElement(std::string Name) {
	cgicc::form_iterator i = CGI->getElement(Name);
	if (i != CGI->getElements().end()) {
		return i->getValue();
	}
	return "";
}

std::string Report::operator()() {
	std::stringstream out;
	out << cgicc::div().set("id", Scope("container")).set("class", CSSClass)
			<< std::endl;
	out << cgicc::div() << std::endl;
	return out.str();
}

std::string Report::InputForm() {
	using namespace cgicc;

	std::stringstream out;


	if (Params.size() > 0) {
		out
				<< "<TABLE align=\"center\" border=\"0\" style=\"border-bottom-style:solid\">"
						"<tbody><TR><TD ALIGN=\"center\" colspan=\"2\" class='td_titulo_tabela'>Defina os filtros para gera&ccedil;&atilde;o do relat&oacute;rio</TD></TR>"
						"" << std::endl;

		//out << (table().set("align","center")).set("colspan","2");

//
		std::stringstream t;
		int ParamNum = 0;
		for (std::list<Param*>::iterator i = Params.begin(); i != Params.end();
				++i) {
			t.str("");
			t << "param" << ParamNum;
			(*i)->Name = t.str();
			out << tr();
			out << td();
			out << (*i)->Label;
			out << td();
			out << td();
			out << (*i)->GetInputHTML();
			out << td();
			out << tr();
			ParamNum++;
		}

		out << "</tbody></table>";
		out << "<center>";
		out << Button("output", "Gerar", "action('output');") << "</center>"
				<< std::endl;
	} else {
		out << OutputForm();
	}
	return out.str();
}

void Report::setParametros(std::vector<Parametro> par) {
	parametros = par;
}

std::string Report::OutputForm() {
	using namespace cgicc;

	std::stringstream out;

	out << "<center>";
	if (Params.size() > 0 || parametros.size() > 0)
		out << "<b>Dados de entrada</b><br>" << std::endl;

	int ParamNum1 = 0;
	std::stringstream t1;
	for (std::list<Param*>::iterator i = Params.begin(); i != Params.end();
			++i) {
		t1.str("");
		t1 << "param" << ParamNum1;
		std::string val = GetFormElement(Scope("param-") + t1.str());
		out << (*i)->Label << ": " << val << "<br>";

		ParamNum1++;
	}

	for (int i = 0; i < parametros.size(); i++)
		out << parametros[i].nome << ": " << parametros[i].valor << "<br>";

	pqxx::work w(*DBConnection);

	int ParamNum = 0;
	std::stringstream t;
	try {
		for (std::list<Param*>::iterator i = Params.begin(); i != Params.end();
				++i) {
			t.str("");
			t << "param" << ParamNum;
			std::string val = GetFormElement(Scope("param-") + t.str());

			std::stringstream ss; //create a stringstream
			ss << ParamNum + 1;
			Query.replace(Query.find("$" + ss.str()), 1 + ss.str().length(),
					"'" + val + "'");

			ParamNum++;
		}

		pqxx::result r = w.exec(Query + Query1);
		w.commit();
		//out<<r.size();

// put result
		std::stringstream Colspan;
		Colspan << r.columns();
		out << br();
		if(r.size() == 0){
			out<<"<Center>NENHUM RESULTADO ENCONTRADO</Center>";
		}else{
		out << table().set("width", "80%");
		out << tr();
		out << th().set("colspan", Colspan.str());
		out << " ";
		out << th();
		out << tr();
		out
				<< "<TR><TD ALIGN=\"center\" colspan=\"" + Colspan.str()
						+ "\" class='td_titulo_tabela'>Resultado</TD></TR>";

		out << tr().set("class", "impar_tr");
		for (int i = 0; i < r.columns(); i++) {
			out << td();
			out << "<b><center>";
			out << r.column_name(i);
			out << "</center></b>";
			out << td();
		}

		out << tr();
		bool par = true;

		for (unsigned int ind = 0; ind < r.size(); ind++) {
			if (par) {
				out
						<< (tr().set("class", "par_tr")).set("style",
								"cursor: default");
				par = false;
			} else {
				out << tr().set("class", "impar_tr");
				par = true;
			}
			for (unsigned int ind1 = 0; ind1 < r[ind].size(); ind1++) {
				out << td();
				out << r[ind][ind1].c_str();
				out << td();
			}
			out << tr();
		}

		out << table();
		}
		out
				<< "";
		if (Params.size() > 1110)
			out << Button("input", "Voltar", "action('input');") << ""
					<< std::endl;
		else
			out
					<< "";

	} catch (std::exception& ex) {
		out << ex.what();
	}
	out << "</center>";
	return out.str();
}

std::string Report::Button(std::string Id, std::string Text,
		std::string JSFunction) {
	using namespace cgicc;
	std::stringstream OnClick, out;

	if (Text == "") {
		Text = "&nbsp;";
	}

	OnClick << JSFunction;
	out << button().set("id", Scope(Id)).set("onclick", OnClick.str());
	out << span().set("id", Id + "-limg") << "&nbsp;" << span(); // left image span
	out << span().set("id", Id + "-text") << Text << span(); //text span
	out << span().set("id", Id + "-rimg") << "&nbsp;" << span(); //right image span
	out << button() << std::endl;

	return out.str();
}
;

bool Report::CatchAJAX() {
	if (GetFormElement(Scope("action")) != "") {
		std::string Action = GetFormElement(Scope("action"));

		if (Action == "input") {
			std::cout << InputForm();
		} else if (Action == "output") {
			std::cout << OutputForm();
		}

		return true;
	}
	return false;
}
