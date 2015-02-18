
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
					var id = this.scope('field-' + this.fields[i]);
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
		searchable.run = function () {
			tableEdit.initFilter(this);
			simpleRun.call(this);
		}
		searchable.applySearch = function () {
		}
		return searchable;
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

tableEdit.filterPredicatChanged = function (id, value) {
	var filterInputContainer = $("." + id + ".filter-input-container");
	filterInputContainer.empty();
	switch (value) {
		case "text-eq":
		case "text-like":
			filterInputContainer.append('<input type="text" class="form-control filter-input ' + id + '">');
			return;
		case "num-eq":
		case "num-lt":
		case "num-eqlt":
		case "num-gt":
		case "num-eqgt":
			filterInputContainer.append('<input type="number" class="form-control filter-input ' + id + '" value="0">');
			return;
		case "date-eq":
		case "date-lt":
		case "date-lteq":
		case "date-gt":
		case "date-eqgt":
			filterInputContainer.append('<input type="date" class="form-control filter-input ' + id + '">');
			return;
		case "timestampz-eq":
		case "timestampz-lt":
		case "timestampz-lteq":
		case "timestampz-gt":
		case "timestampz-eqgt":
			filterInputContainer.append('<input type="datetime-local" class="form-control filter-input ' + id + '">');
			return;
		case "char-eq":
			filterInputContainer.append('<input type="text" class="form-control filter-input ' + id + '" maxlength=1 placeholder="input one character">');
			return;
		case "empty":
		case "bool-true":
		case "bool-false":
	}
};

tableEdit.initFilter = function (searchable) {
	var filterPanel = $("#" + searchable.scope("filter"));
	filterPanel.addClass("filter-panel");
	filterPanel.append('<table id="' + searchable.scope("filter-constraints") + '" class="filter-constraints"></table>');
	filterPanel.append('<div class="filter-button dropdown">'
			+ '<button class="btn btn-default dropdown-toggle" type="button" id="' + searchable.scope("filter-drop-down") + '" data-toggle="dropdown" aria-expanded="true">Add Field...<span class="caret"></span></button>'
			+ '<ul class="dropdown-menu" role="menu" aria-labelledby="' + searchable.scope("filter-drop-down") + '" id=' + searchable.scope("filter-drop-down-area") + '></ul>'
			+ '</div>'
			+ '<button type="button" class="filter-button btn btn-default" onclick="' + searchable.name + '.clearFilterClick()">Clear</button>'
			+ '<button type="button" class="filter-button btn btn-default" onclick="' + searchable.name + '.viewSqlClick()">View SQL</button>'
//			+ '<!--button type="button" class="filter-button btn btn-default" onclick="' + searchable.name + '.applyFilterClick()">Apply</button-->'
			);
	filterPanel.append('<div id="' + searchable.scope("sql-area") + '"></div>');

	searchable.clearFilterClick = function () {
		$('#' + this.scope("filter-constraints")).empty();
	};
	searchable.viewSqlClick = function () {
		var constraints = "";
		$('#' + this.scope("filter-constraints")).find("tr").each(function () {
			var fieldName = $(this).find(".filter-field").attr("field");
			var isInverse = $(this).find(".filter-inverse").is(':checked');
			var predicate = $(this).find(".filter-predicate").val();
			var value = $(this).find(".filter-input").val();
			var constraint;
			switch (predicate) {
				case "char-eq":
				case "text-eq":
					constraint = fieldName + ' = \'' + value + "'";
					break;
				case "text-like":
					constraint = fieldName + ' LIKE \'' + value + "'";
					break;
				case "num-eq":
					constraint = fieldName + ' = ' + value;
					break;
				case "num-lt":
					constraint = fieldName + ' < ' + value;
					break;
				case "num-eqlt":
					constraint = fieldName + ' <= ' + value;
					break;
				case "num-gt":
					constraint = fieldName + ' > ' + value;
					break;
				case "num-eqgt":
					constraint = fieldName + ' >= ' + value;
					break;
				case "date-eq":
					constraint = fieldName + ' = DATE \'' + value + "'";
					break;
				case "date-lt":
					constraint = fieldName + ' < DATE \'' + value + "'";
					break;
				case "date-lteq":
					constraint = fieldName + ' <= DATE \'' + value + "'";
					break;
				case "date-gt":
					constraint = fieldName + ' > DATE \'' + value + "'";
					break;
				case "date-eqgt":
					constraint = fieldName + ' >= DATE \'' + value + "'";
					break;

				case "timestampz-eq":
					constraint = fieldName + ' = TIMESTAMP \'' + value + "'";
					break;
				case "timestampz-lt":
					constraint = fieldName + ' < TIMESTAMP \'' + value + "'";
					break;
				case "timestampz-lteq":
					constraint = fieldName + ' <= TIMESTAMP \'' + value + "'";
					break;
				case "timestampz-gt":
					constraint = fieldName + ' > TIMESTAMP \'' + value + "'";
					break;
				case "timestampz-eqgt":
					constraint = fieldName + ' >= TIMESTAMP \'' + value + "'";
					break;

				case "empty":
					constraint = fieldName + ' IS NULL';
					break;
				case "bool-true":
					constraint = fieldName + ' = TRUE';
					break;
				case "bool-false":
					constraint = fieldName + ' = FALSE';
					break;
			}
			//TBD
			if (isInverse) {
				constraint = 'NOT (' + constraint + ")";
			}
			if (constraints !== "") {
				constraints += " AND ";
			}
			constraints += constraint;
		});
		$('#' + this.scope("sql-area")).text("SELECT * FROM " + this.name + " WHERE " + constraints);
	};
	var selectorByDataType = function (type) {
		switch (type) {
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
	searchable.addFilterFieldClick = function (field) {
		for (var i = 0; i < this.fields.length; i++) {
			if (field === this.fields[i].name) {
				field = this.fields[i];
			}
		}
		var constraints = $('#' + this.scope("filter-constraints"));
		var id = "filter-id-" + tableEdit.lastUniqueID++;
		constraints.append('<tr id="' + id + '">'
				+ '<td class="filter-field" field="' + field.name + '">' + field.name + '</td>'
				+ '<td><label><input type="checkbox" class="filter-inverse">Not</label></td>'
				+ '<td><select class="form-control filter-predicate" onchange="tableEdit.filterPredicatChanged(\'' + id + '\' , this.options[this.selectedIndex].value)">'
				+ selectorByDataType(field.type)
				+ '</select></td>'
				+ '<td class="filter-input-container"></td>'
				+ '<td><button type="button" class="btn btn-default" onclick="tableEdit.removeById(\'' + id + '\')"><span class="glyphicon glyphicon-remove-circle"></span></button></td>'
				+ '</tr>');
		$("#" + id).addClass(id);
		$("#" + id).find("*").addClass(id);
		tableEdit.filterPredicatChanged(id, $(".filter-predicate." + id).val());
	};
	var buttons = $('#' + searchable.scope("filter-drop-down-area"));
	for (var i = 0; i < searchable.fields.length; i++) {
		buttons.append('<li role = "presentation" > <a role = "menuitem" tabindex = "-1" href = "#" onclick="' + searchable.name + '.addFilterFieldClick(\'' + searchable.fields[i].name + '\')"> ' + searchable.fields[i].name + ' </a></li>');
	}
}

