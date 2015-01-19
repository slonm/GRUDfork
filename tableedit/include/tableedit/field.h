#ifndef _tableedit_field_h_
#define _tableedit_field_h_

#include <string>

#include <tableedit/simple.h>
#include <tableedit/fieldtypes.h>


namespace TableEdit {

	class Simple;

	enum SearchTypes { stMatch, stRange };

	class Field {
		protected:
			Simple *Table;
			std::string Name;
			std::string Label;
			bool PrimaryKey;
			bool OrderKey;
			bool ReadOnly;
			bool Hidden;
			int CharLength;

			FieldType Type;

			bool Link;
			std::string LinkTable;
			std::string LinkKey;
			std::string LinkValue;
			std::string LinkOrder;
			bool LinkAscending;

			bool Searchable;
			SearchTypes SearchType;


		public:
			Field(std::string Name);
			void SetTableEdit(Simple *T) { Table = T; };

			Field* SetLabel(std::string L) { Label = L; return this; };
			Field* SetPrimaryKey() { PrimaryKey = true; return this; };
			Field* SetOrderKey() { OrderKey = true; return this; };
			Field* SetReadOnly() { ReadOnly = true; return this; };
			Field* SetHidden() { Hidden = true; return this; };
			Field* SetLink(std::string TableName, std::string KeyField, std::string ValueField, std::string OrderField, bool OrderAsc);

			void SetCharLength(int L) { CharLength = L; };
			void SetType(FieldType T) { Type = T;};

			bool IsPrimaryKey() { return PrimaryKey; };
			bool IsOrderKey() { return OrderKey; };
			bool IsHidden() { return Hidden; };
			bool IsLink() { return Link; };
			bool IsReadOnly() { return ReadOnly; };

			std::string GetName() { return Name; };
			std::string GetLabel() { return Label; };
			std::string GetLinkTable() { return LinkTable; };
			std::string GetLinkValue() { return LinkValue; };
			std::string GetLinkKey() { return LinkKey; };
			FieldType GetType() { return Type; };

			std::string GetInputHTML(std::string Value="",std::string Id = "", std::string OnChange = "");
	};
};

#endif
