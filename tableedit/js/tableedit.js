
var tableEdit = {

	Simple : function (params) {
		this.name = params.name;
		this.fields = params.fields;

		this.run = function () {
			this.action('select');
		}

		this.action = function (str) {
			if ( str == 'select' ) {
				this.replaceForm(this.send(this.select()));
			}
			if ( str == 'insert' ) {
				this.replaceForm(this.send(this.insertForm()));
			}
			if ( str == 'commit-insert' ) {
				this.replaceForm(this.send(this.commitInsert()));
				this.replaceForm(this.send(this.select()));
			}
			if ( str == 'delete' ) {
				this.replaceForm(this.send(this.deleteForm(arguments[1])));
			}
			if ( str == 'commit-delete') {
				this.replaceForm(this.send(this.commitDelete(arguments[1])));
				this.replaceForm(this.send(this.select()));
			}
			if ( str == 'update') {
				this.replaceForm(this.send(this.updateForm(arguments[1])));
			}
			if ( str == 'commit-update') {
				this.replaceForm(this.send(this.commitUpdate(arguments[1])));
				this.replaceForm(this.send(this.select()));
			}
		}

		this.select = function() {
			var params = this.scope('action')+'=read'+ '&'+this.scope('page')+'='+this.page;
			return params;
		}

		this.insertForm = function() {
			var params = this.scope('action')+'=insert';
			return params;
		}

		this.commitInsert = function() {
			var params = this.scope('action')+'=commit-insert';
			for ( var i=0; i<this.fields.length; i++ ) {
		 		var id =  this.scope('field-'+this.fields[i]);
				//alert(id);
				params += '&' + id + '=' + document.getElementById(id).value;
			}
			return params;
		}

		this.deleteForm = function(id) {
			var params = this.scope('action')+'=delete';
			params += '&' + this.scope('delete-id')+'='+id;
			return params;
		}

		this.commitDelete = function(id) {
			var params = this.scope('action')+'=commit-delete';
			params += '&' + this.scope('delete-id')+'='+id;
			return params;
		}

		this.updateForm = function(id) {
			var params = this.scope('action')+'=update';
			params += '&' + this.scope('update-id')+'='+id;
			return params;
		}

		this.commitUpdate = function(id) {
			var params = this.scope('action')+'=commit-update';
			params += '&' + this.scope('update-id')+'='+id;
			for ( var i=0; i<this.fields.length; i++ ) {
		 		var id =  this.scope('field-'+this.fields[i]);
				//alert(id);
				params += '&' + id + '=' + document.getElementById(id).value;
			}
			return params;
		}

		this.send = function(str) {
			//alert(str,this.script);
			var req = new XMLHttpRequest();
			req.open("POST",this.script,false);

			req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			//req.setRequestHeader("Content-length", params.length);
			//req.setRequestHeader("Connection", "close");

			req.send(str);

			return req.responseText;
		}

		this.scope = function(str) {
			return 'tableedit-'+this.name+'-'+str;
		}

		this.replaceForm = function(str) {
			this.container.innerHTML = str;
		}

		this.nextPage = function() {
			this.page++;
			this.action('select');
		}

		this.prevPage = function() {
			this.page--;
			if (this.page <= 0) {
				this.page = 1;
			}
			this.action('select');
		}


		this.script = document.location;
		this.container = document.getElementById(this.scope("container"));
		this.page = 1;

	},

	Searchable : function (params) {
		this.inheritFrom = Simple;
		this.inheritFrom();

		this.applySearch = function() {
		}
	},

	Browsable : function (params) {
		this.inheritFrom = tableEdit.Simple;
		this.inheritFrom(params);

	}

};
