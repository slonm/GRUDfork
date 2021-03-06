
var tableEdit = {
	Simple: function (params) {
		return new function () {
			this.name = params.name;
			this.fields = params.fields;

			this.run = function () {
				this.action('select');
			}

			this.action = function (str) {
				if (str == 'select') {
					this.replaceForm(this.send(this.select()));
				}
				if (str == 'insert') {
					this.replaceForm(this.send(this.insertForm()));
				}
				if (str == 'commit-insert') {
					this.replaceForm(this.send(this.commitInsert()));
					this.replaceForm(this.send(this.select()));
				}
				if (str == 'delete') {
					this.replaceForm(this.send(this.deleteForm(arguments[1])));
				}
				if (str == 'commit-delete') {
					this.replaceForm(this.send(this.commitDelete(arguments[1])));
					this.replaceForm(this.send(this.select()));
				}
				if (str == 'update') {
					this.replaceForm(this.send(this.updateForm(arguments[1])));
				}
				if (str == 'commit-update') {
					this.replaceForm(this.send(this.commitUpdate(arguments[1])));
					this.replaceForm(this.send(this.select()));
				}
			}

			this.select = function () {
				var params = this.scope('action') + '=read' + '&' + this.scope('page') + '=' + this.page;
				return params;
			}

			this.insertForm = function () {
				var params = this.scope('action') + '=insert';
				return params;
			}

			this.commitInsert = function () {
				var params = this.scope('action') + '=commit-insert';
				for (var i = 0; i < this.fields.length; i++) {
					var id = this.scope('field-' + this.fields[i].name);
					//alert(id);
					params += '&' + id + '=' + document.getElementById(id).value;
				}
				return params;
			}

			this.deleteForm = function (id) {
				var params = this.scope('action') + '=delete';
				params += '&' + this.scope('delete-id') + '=' + id;
				return params;
			}

			this.commitDelete = function (id) {
				var params = this.scope('action') + '=commit-delete';
				params += '&' + this.scope('delete-id') + '=' + id;
				return params;
			}

			this.updateForm = function (id) {
				var params = this.scope('action') + '=update';
				params += '&' + this.scope('update-id') + '=' + id;
				return params;
			}

			this.commitUpdate = function (id) {
				var params = this.scope('action') + '=commit-update';
				params += '&' + this.scope('update-id') + '=' + id;
				for (var i = 0; i < this.fields.length; i++) {
					var id = this.scope('field-' + this.fields[i].name);
					//alert(id);
					params += '&' + id + '=' + document.getElementById(id).value;
				}
				return params;
			}

			this.fieldByName = function (name) {
				for (var i = 0; i < this.fields.length; i++) {
					if (this.fields[i].name === name) {
						return this.fields[i];
					}
				}
			}

			this.send = function (str) {
				//alert(str,this.script);
				var req = new XMLHttpRequest();
				req.open("POST", this.script, false);

				req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
				//req.setRequestHeader("Content-length", params.length);
				//req.setRequestHeader("Connection", "close");

				req.send(str);

				return req.responseText;
			}

			this.scope = function (str) {
				return 'tableedit-' + this.name + '-' + str;
			}

			this.replaceForm = function (str) {
				if (this.container !== null) {
					this.container.innerHTML = str;
				}
			}

			this.nextPage = function () {
				this.page++;
				this.action('select');
			}

			this.prevPage = function () {
				this.page--;
				if (this.page <= 0) {
					this.page = 1;
				}
				this.action('select');
			}


			this.script = document.location;
			this.container = document.getElementById(this.scope("container"));
			this.page = 1;
		}
	},
	Searchable: function (params) {
		var searchable = tableEdit.Simple(params);
		var simpleRun = searchable.run;
		var simpleSelect = searchable.select;
		searchable.constraints = "";
		searchable.run = function () {
			tableEdit.initFilter(this);
			simpleRun.call(this);
		};
		searchable.select = function () {
			return simpleSelect.call(this) + this.constraints;
		};
		return searchable;
	},
	Constructor: function (params) {
		return new function () {
			this.params = params;
			this.name = this.params.name;
			this.scope = function (str) {
				return 'tableedit-' + this.params.name + '-' + str;
			}

			tableEdit.initConstructor(this);
		}
	},
	Browsable: function (params) {
		this.inheritFrom = tableEdit.Simple;
		this.inheritFrom(params);
	}

};

tableEdit.lastUniqueID = 0;
tableEdit.removeById = function (id) {
	$("." + id).remove();
};

tableEdit.selectorByField = function (field) {
	if (field.linkkey !== '') {
		return '<option value="link-eq">Equal to</option>'
				+ '<option value="link-like">Like</option>'
				+ '<option value="empty">Is empty</option>';
	}
	switch (field.type) {
		case 'text':
		case 'varchar':
			return '<option value="text-eq">Equal to</option>'
					+ '<option value="text-like">Like</option>'
					+ '<option value="empty">Is empty</option>';
		case 'int2':
		case 'int4':
		case 'int8':
		case 'float4':
		case 'float8':
			return '<option value="num-eq">Equal to</option>'
					+ '<option value="num-lt">Less than</option>'
					+ '<option value="num-eqlt">Less or equal than</option>'
					+ '<option value="num-gt">Greater than</option>'
					+ '<option value="num-eqgt">Greater or equal than</option>'
					+ '<option value="empty">Is empty</option>';
		case 'char':
			return '<option value="char-eq">Equal to</option>'
					+ '<option value="empty">Is empty</option>';
		case 'bool':
			return '<option value="bool-true">Is TRUE</option>'
					+ '<option value="bool-false">Is FALSE</option>'
					+ '<option value="empty">Is empty</option>';
		case 'date':
			return '<option value="date-eq">Equal to</option>'
					+ '<option value="date-lt">Less than</option>'
					+ '<option value="date-lteq">Less or equal than</option>'
					+ '<option value="date-gt">Greater than</option>'
					+ '<option value="date-eqgt">Greater or equal than</option>'
					+ '<option value="empty">Is empty</option>';
		case 'timestampz':
			return '<option value="timestampz-eq">Equal to</option>'
					+ '<option value="timestampz-lt">Less than</option>'
					+ '<option value="timestampz-lteq">Less or equal than</option>'
					+ '<option value="timestampz-gt">Greater than</option>'
					+ '<option value="timestampz-eqgt">Greater or equal than</option>'
					+ '<option value="empty">Is empty</option>';
	}
};

tableEdit.initConstructor = function (constructor) {

	constructor.constructorTableChanged = function (id, value) {
		var select = $("#" + id).find('.constructor-this-fields');
		if (select.length) {
			var table = this.tableByName(value);
			select.html(this.selectFieldHtml(id, "constructor-this-fields", table));
		}
	};

	constructor.constructorFieldChanged = function (id, value) {

	};

	constructor.constructorJoinChanged = function (id, value) {

	};

	constructor.selectTableHtml = function (id) {
		var result = '<select class="constructor-tables" onchange="' + this.params.name + '.constructorTableChanged(\'' + id + '\' , this.options[this.selectedIndex].value)">';
		//result += '<option value="-">&lt;Not selected&gt;</option>';
		for (var i = 0; i < this.params.tables.length; i++) {
			result += '<option value="' + this.params.tables[i].name + '">' + this.params.tables[i].name + '</option>';
		}
		return result + '</select>';
	};

	constructor.tableByRowId = function (id) {
		var tname = $("#" + id).find('.constructor-tables').val();
		if (tname === '-')
			return;
		for (var i = 0; i < this.params.tables.length; i++) {
			if (this.params.tables[i].name === tname) {
				return this.params.tables[i];
			}
		}
	};

	constructor.tableByName = function (name) {
		for (var i = 0; i < this.params.tables.length; i++) {
			if (this.params.tables[i].name === name) {
				return this.params.tables[i];
			}
		}
	};

	constructor.selectFieldHtml = function (id, css, table) {
		var result = '<select class="' + css + '" onchange="' + this.params.name + '.constructorFieldChanged(\'' + id + '\' , this.options[this.selectedIndex].value)">';
		//result += '<option value="-">&lt;Not selected&gt;</option>';
		if (typeof table !== 'undefined') {
			for (var i = 0; i < table.fields.length; i++) {
				result += '<option value="' + table.fields[i].name + '">' + table.fields[i].name + '</option>';
			}
		}
		return result + '</select>';
	};

	constructor.removeClick = function (id, parentId) {
		$("#" + parentId).find('.constructor-join-button-container').css({display: "inline"});
		$("#" + id).remove();
	};

	constructor.newRow = function (parentId, level) {

		var id = "constructor-id-" + tableEdit.lastUniqueID++;
		var newRow = '<tr id="' + id + '" level="' + (parseInt(level) + 1) + '">'
				+ '<td class="constructor-tree">';
//		for (var i = 0; i <= level; i++) {
//			if (i === level) {
//				newRow += '+';
//			} else {
//				newRow += '|';
//			}
//		}
		newRow += '</td>'
				+ '<td>' + constructor.selectTableHtml(id) + '</td>';
		if (level > 0) {
			var joinSelect = '<select class="constructor-joins" onchange="' + this.params.name + '.constructorJoinChanged(\'' + id + '\' , this.options[this.selectedIndex].value)">';
			joinSelect += '<option value="inner join">inner</option>';
			joinSelect += '<option value="left outer join">left outer</option>';
			joinSelect += '<option value="right outer join">right outer</option>';
			joinSelect += '<option value="full outer join">full outer</option>';
			joinSelect += '</select>';
			newRow += '<td>' + joinSelect + '</td>'
					+ '<td>on:</td><td align=right><button class="constructor-that-table" disabled">' + this.tableByRowId(parentId).name + "</button>&nbsp;.&nbsp;" + this.selectFieldHtml(id, "constructor-that-fields", this.tableByRowId(parentId)) + '</td>'
					+ '<td>= ' + this.selectFieldHtml(id, "constructor-this-fields", this.tableByName(this.params.tables[0].name)) + '</td>';
		}
		newRow += '<td><span class="constructor-join-button-container">'
				+ '<button type="button" class="constructor-button" onclick="' + this.params.name + '.joinTableClick(\'' + id + '\')">Join table</button>&nbsp;'
				//+ '<button type="button" class="constructor-button" onclick="' + this.params.name + '.joinTreeClick(\'' + id + '\')">Join to tree</button>&nbsp;'
				+ '</span>';
		if (level > 0) {
			newRow += '<button type="button" class="constructor-button" onclick="' + this.params.name + '.removeClick(\'' + id + '\',\'' + parentId + '\')"><span class="glyphicon glyphicon-remove-circle"></span></button>';
		}
		newRow += '</td>'
				+ '<tr>';
		return newRow;
	};

	constructor.joinTableClick = function (id) {
		var level = $("#" + id).attr("level");
		$("#" + this.scope("constructor-from")).append(this.newRow(id, level));
		//$(this.newRow(id, level)).insertAfter($("#" + id));
		//hide parent join button
		//$("#" + id).find('.constructor-join-button-container').css({display: "none"});
	};

	constructor.joinTreeClick = function (id) {
		var level = $("#" + id).attr("level");
		var found = false;
		var nextTr;
		$("#" + this.scope("constructor-from")).find("tr[level=" + level + "]").each(function (index) {
			if ($(this).id === id) {
				found = true;
			} else if (found === true && typeof (nextTr) === 'undefined') {
				nextTr = $(this);
			}
		});
		var newRow = $(this.newRow(id, level));
		if (typeof (nextTr) === 'undefined') {
			newRow.insertBefore(nextTr);
		} else {
			$("#" + this.scope("constructor-from")).append(newRow);
		}
		//hide parent join button
		$("#" + id).find('.constructor-join-button-container').css({display: "none"});
	};

	constructor.predicate = function (strPredicate) {
		var pred = {};
		if (strPredicate == "like") {
			pred.isBinary = true;
			pred.predicate = "LIKE";
		} else if (strPredicate === "eq") {
			pred.isBinary = true;
			pred.predicate = "=";
		} else if (strPredicate === "lt") {
			pred.isBinary = true;
			pred.predicate = "<";
		} else if (strPredicate === "eqlt") {
			pred.isBinary = true;
			pred.predicate = "<=";
		} else if (strPredicate === "gt") {
			pred.isBinary = true;
			pred.predicate = ">";
		} else if (strPredicate === "eqgt") {
			pred.isBinary = true;
			pred.predicate = ">=";
		} else if (strPredicate === "true") {
			pred.isBinary = false;
			pred.predicate = "= TRUE";
		} else if (strPredicate === "false") {
			pred.isBinary = false;
			pred.predicate = "= FALSE";
		} else if (strPredicate === "empty") {
			pred.isBinary = false;
			pred.predicate = "IS NULL";
		} else if (strPredicate === "n-like") {
			pred.isBinary = true;
			pred.predicate = "NOT LIKE";
		} else if (strPredicate === "n-eq") {
			pred.isBinary = true;
			pred.predicate = "<>";
		} else if (strPredicate === "n-lt") {
			pred.isBinary = true;
			pred.predicate = ">=";
		} else if (strPredicate === "n-eqlt") {
			pred.isBinary = true;
			pred.predicate = ">";
		} else if (strPredicate === "n-gt") {
			pred.isBinary = true;
			pred.predicate = "<=";
		} else if (strPredicate === "n-eqgt") {
			pred.isBinary = true;
			pred.predicate = "<";
		} else if (strPredicate === "n-true") {
			pred.isBinary = false;
			pred.predicate = "<> TRUE";
		} else if (strPredicate === "n-false") {
			pred.isBinary = true;
			pred.predicate = "<> FALSE";
		} else if (strPredicate === "n-empty") {
			pred.isBinary = false;
			pred.predicate = "IS NOT NULL";
		}
		return pred;
	};

	constructor.updateSqlArea = function () {
		var sqlArea = $('#' + this.scope("sql-area"));
		var text = 'select * from ';
		var constructorTable = $("#" + constructor.scope("constructor-from"));
		constructorTable.find("tr").each(function () {
			var cnt = $(this);
			if (typeof cnt.attr('id') !== 'undefined') {
				if (cnt.find(".constructor-joins").length !== 0) {
					text += " " + cnt.find(".constructor-joins").val();
					text += " " + cnt.find(".constructor-tables").val() + " on";
					text += " " + cnt.find(".constructor-that-table").text() + "." + cnt.find(".constructor-that-fields").val() + " =";
					text += " " + cnt.find(".constructor-tables").val() + "." + cnt.find(".constructor-this-fields").val();
				} else {
					text += cnt.find(".constructor-tables").val();
				}
			}
		});
		this.prepareFilter();
		var where = "";
		for (var i = 0; i < this.constraints.length; i++) {
			if (where === "") {
				where = " where";
			}
			var pred = this.predicate(this.constraints[i].predicate);
			where += " " + this.constraints[i].tableName + "." + this.constraints[i].fieldName + " " + pred.predicate;
			if (pred.isBinary) {
				where += " " + this.constraints[i].value;
			}
		}
		text += where + ";";
		sqlArea.text(text);
	};

	constructor.viewSqlClick = function (button) {
		var sqlArea = $('#' + this.scope("sql-area"));
		sqlArea.css('display', 'block');
		this.updateSqlArea();
	};

	constructor.clearFilterClick = function () {
		$('#' + this.scope("filter-constraints")).empty();
	};

	constructor.constraints = [];
	constructor.prepareFilter = function () {
		constructor.constraints = [];
		var constraints = constructor.constraints;
		$('#' + this.scope("filter-constraints")).find("tr").each(function () {
			var constraint = {};
			constraint.tableName = $(this).find(".filter-table").attr("table");
			constraint.fieldName = $(this).find(".filter-field").attr("field");
			var isInverse = $(this).find(".filter-inverse").is(':checked');
			var pred = $(this).find(".filter-predicate").val();
			var pos = pred.indexOf("-");
			var pred1 = pred;
			if (pos > -1) {
				pred1 = pred.slice(pos + 1);
			}
			constraint.predicate = (isInverse ? "n-" : "") + pred1;
			constraint.value = $(this).find(".filter-input").val();
			if (pred === "link-eq") {
				var option = $(this).find(".filter-input").find("option[value=" + constraint.value + "]");
				if (option.length) {
					constraint.value = option.text();
				}
			}
			i++;
			constraints.push(constraint);
		});
	};

	constructor.applySearch = function () {
		//this.action('select');
		this.updateSqlArea();
	};

	constructor.fieldByName = function (tname, name) {
		for (var t = 0; t < tables.length; t++) {
			if (tname === tables[t].name) {
				for (var i = 0; i < tables[t].fields.length; i++) {
					if (tables[t].fields[i].name === name) {
						return tables[t].fields[i];
					}
				}
			}
		}
	};

	constructor.send = function (str) {
		//alert(str,this.script);
		var req = new XMLHttpRequest();
		req.open("POST", document.location, false);

		req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		//req.setRequestHeader("Content-length", params.length);
		//req.setRequestHeader("Connection", "close");

		req.send(str);

		return req.responseText;
	};

	constructor.filterPredicatChanged = function (id, tname, fieldName, value) {
		var filterInputContainer = $("." + id + ".filter-input-container");
		filterInputContainer.empty();
		var field = this.fieldByName(tname, fieldName);
		if (value === "link-like" && field.linkkey !== '') {
			filterInputContainer.append(this.send('tableedit-' + field.linktable + '-action' + '=input&field=' + field.linkvalue));
			return;
		}
		switch (value) {
			case "empty":
			case "bool-true":
			case "bool-false":
				break;
			default:
				filterInputContainer.append(this.send(this.scope('action') + '=input&field=' + fieldName));
		}
	};

	constructor.addFilterFieldClick = function (tableName, field) {
		var tableObject = this.tableByName(tableName);
		for (var i = 0; i < tableObject.fields.length; i++) {
			if (field === tableObject.fields[i].name) {
				field = tableObject.fields[i];
			}
		}
		var constraints = $('#' + this.scope("filter-constraints"));
		var id = "filter-id-" + tableEdit.lastUniqueID++;
		constraints.append('<tr id="' + id + '">'
				+ '<td class="filter-table" table="' + tableName + '">' + tableName + '</td>'
				+ '<td class="filter-field" field="' + field.name + '">' + field.label + '</td>'
				+ '<td><label style="white-space: nowrap;"><input type="checkbox" class="filter-inverse">Not</label></td>'
				+ '<td><select class="filter-predicate" onchange="' + this.name + '.filterPredicatChanged(\'' + id + '\' , \'' + tableName + '\' , \'' + field.name + '\' , this.options[this.selectedIndex].value)">'
				+ tableEdit.selectorByField(field)
				+ '</select></td>'
				+ '<td class="filter-input-container"></td>'
				+ '<td><button type="button" onclick="tableEdit.removeById(\'' + id + '\')"><span class="glyphicon glyphicon-remove-circle"></span></button></td>'
				+ '</tr>');
		$("#" + id).addClass(id);
		$("#" + id).find("*").addClass(id);
		this.filterPredicatChanged(id, tableName, field.name, $(".filter-predicate." + id).val());
	};

	var constructorPanel = $("#" + constructor.scope("constructor"));
	constructorPanel.addClass("constructor-panel");
	constructorPanel.append('<table id="' + constructor.scope("constructor-from") + '" class="constructor-from">'
			+ constructor.newRow(0, 0)
			+ '</table>');
	constructorPanel.append('<table id="' + constructor.scope("filter-constraints") + '" class="filter-constraints"></table>');
	constructorPanel.append('<button type="button" class="constructor-button" onclick="' + constructor.params.name + '.viewSqlClick(this)">View SQL</button>');
	constructorPanel.append('<div class="filter-button dropdown">'
			+ '<button class="filter-button dropdown-toggle" type="button" id="' + constructor.scope("filter-drop-down") + '" data-toggle="dropdown" aria-expanded="true">Add Filter...<span class="caret"></span></button>'
			+ '<ul class="dropdown-menu multi-level" role="menu" aria-labelledby="' + constructor.scope("filter-drop-down") + '" id=' + constructor.scope("filter-drop-down-area") + '></ul>'
			+ '</div>'
			+ '<button type="button" class="filter-button" onclick="' + constructor.name + '.clearFilterClick()">Clear</button>'
			+ '<button type="button" class="filter-button" onclick="' + constructor.name + '.viewSqlClick(this)">View SQL</button>'
			+ '<button type="button" class="filter-button" onclick="' + constructor.name + '.applySearch()">Apply</button>'
			);
	constructorPanel.append('<pre id="' + constructor.scope("sql-area") + '" style="display: none;"></pre>');
	var buttons = $('#' + constructor.scope("filter-drop-down-area"));
	var tables = [];
	if (typeof constructor.params !== 'undefined') {
		tables = constructor.params.tables;
	} else {
		tables[0] = {};
		tables[0].name = constructor.name;
		tables[0].field = constructor.fields;
	}
	var btnTxt = "";
	for (var t = 0; t < tables.length; t++) {
		btnTxt += '<li class="dropdown-submenu" > <a tabindex = "-1" href = "#"> ' + tables[t].name + ' </a><ul class="dropdown-menu">';
		for (var i = 0; i < tables[t].fields.length; i++) {
			btnTxt += '<li> <a role = "menuitem" tabindex = "-1" href = "#" onclick="' + constructor.name + '.addFilterFieldClick(\'' + tables[t].name + '\' , \'' + tables[t].fields[i].name + '\')"> ' + tables[t].fields[i].name + ' </a></li>';
		}
		btnTxt += '</ul></li>';
	}
	buttons.append(btnTxt);
};

tableEdit.initFilter = function (searchable) {
	var filterPanel = $("#" + searchable.scope("filter"));
	filterPanel.addClass("filter-panel");
	filterPanel.append('<table id="' + searchable.scope("filter-constraints") + '" class="filter-constraints"></table>');
	filterPanel.append('<div class="filter-button dropdown">'
			+ '<button class="filter-button dropdown-toggle" type="button" id="' + searchable.scope("filter-drop-down") + '" data-toggle="dropdown" aria-expanded="true">Add Filter...<span class="caret"></span></button>'
			+ '<ul class="dropdown-menu" role="menu" aria-labelledby="' + searchable.scope("filter-drop-down") + '" id=' + searchable.scope("filter-drop-down-area") + '></ul>'
			+ '</div>'
			+ '<button type="button" class="filter-button" onclick="' + searchable.name + '.clearFilterClick()">Clear</button>'
			+ '<button type="button" class="filter-button" onclick="' + searchable.name + '.viewSqlClick(this)">View SQL</button>'
			+ '<button type="button" class="filter-button" onclick="' + searchable.name + '.applySearch()">Apply</button>'
			);
	filterPanel.append('<pre id="' + searchable.scope("sql-area") + '" style="display: none;"></pre>');

	searchable.clearFilterClick = function () {
		$('#' + this.scope("filter-constraints")).empty();
	};

	searchable.prepareFilter = function () {
		this.constraints = "";
		var constraints = {
			fieldName: {},
			predicate: {},
			value: {}
		};
		var i = 0;
		$('#' + this.scope("filter-constraints")).find("tr").each(function () {
			constraints.fieldName[i] = $(this).find(".filter-field").attr("field");
			var isInverse = $(this).find(".filter-inverse").is(':checked');
			var pred = $(this).find(".filter-predicate").val();
			var pos = pred.indexOf("-");
			var pred1 = pred;
			if (pos > -1) {
				pred1 = pred.slice(pos + 1);
			}
			constraints.predicate[i] = (isInverse ? "n-" : "") + pred1;
			constraints.value[i] = $(this).find(".filter-input").val();
			if (pred === "link-eq") {
				var option = $(this).find(".filter-input").find("option[value=" + constraints.value[i] + "]");
				if (option.length) {
					constraints.value[i] = option.text();
				}
			}
			i++;
		});
		constraints = $.param(constraints);
		if (constraints === "")
			return;
		this.constraints = "&" + constraints;
		this.action('select');
	};

	searchable.applySearch = function () {
		this.prepareFilter();
		this.action('select');
		this.updateSqlArea();
	};

	searchable.updateSqlArea = function () {
		var sqlArea = $('#' + searchable.scope("sql-area"));
		if (sqlArea.css('display') !== 'none') {
			sqlArea.text(this.send(this.scope('action') + '=sql' + this.constraints));
		}
	};

	searchable.viewSqlClick = function (button) {
		var sqlArea = $('#' + searchable.scope("sql-area"));
		if (sqlArea.css('display') === 'none') {
			sqlArea.css('display', 'block');
			$(button).html('Hide SQL');
			this.prepareFilter();
			this.updateSqlArea();
		} else {
			sqlArea.css('display', 'none');
			$(button).html('View SQL');
		}
	};

	searchable.filterPredicatChanged = function (id, fieldName, value) {
		var filterInputContainer = $("." + id + ".filter-input-container");
		filterInputContainer.empty();
		var field = this.fieldByName(fieldName);
		if (value === "link-like" && field.linkkey !== '') {
			filterInputContainer.append(this.send('tableedit-' + field.linktable + '-action' + '=input&field=' + field.linkvalue));
			return;
		}
		switch (value) {
			case "empty":
			case "bool-true":
			case "bool-false":
				break;
			default:
				filterInputContainer.append(this.send(this.scope('action') + '=input&field=' + fieldName));
		}
	};

	searchable.addFilterFieldClick = function (field) {
		for (var i = 0; i < this.fields.length; i++) {
			if (field === this.fields[i].name) {
				field = this.fields[i];
			}
		}
		var constraints = $('#' + this.scope("filter-constraints"));
		var id = "filter-id-" + tableEdit.lastUniqueID++;
		constraints.append('<tr id="' + id + '">'
				+ '<td class="filter-field" field="' + field.name + '">' + field.label + '</td>'
				+ '<td><label style="white-space: nowrap;"><input type="checkbox" class="filter-inverse">Not</label></td>'
				+ '<td><select class="filter-predicate" onchange="' + this.name + '.filterPredicatChanged(\'' + id + '\' , \'' + field.name + '\' , this.options[this.selectedIndex].value)">'
				+ tableEdit.selectorByField(field)
				+ '</select></td>'
				+ '<td class="filter-input-container"></td>'
				+ '<td><button type="button" onclick="tableEdit.removeById(\'' + id + '\')"><span class="glyphicon glyphicon-remove-circle"></span></button></td>'
				+ '</tr>');
		$("#" + id).addClass(id);
		$("#" + id).find("*").addClass(id);
		this.filterPredicatChanged(id, field.name, $(".filter-predicate." + id).val());
	};
	var buttons = $('#' + searchable.scope("filter-drop-down-area"));
	for (var i = 0; i < searchable.fields.length; i++) {
		if (searchable.fields[i].hidden === false) {
			buttons.append('<li role = "presentation" > <a role = "menuitem" tabindex = "-1" href = "#" onclick="' + searchable.name + '.addFilterFieldClick(\'' + searchable.fields[i].name + '\')"> ' + searchable.fields[i].label + ' </a></li>');
		}
	}
};

