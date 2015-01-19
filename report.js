
Report = function ( params ) {

	this.name = params.name;
	this.params = params.params;

	this.run = function () {
		this.action('input');
	}

	this.scope = function(str) {
		return 'report-'+this.name+'-'+str;
	}


	this.inputForm = function () {
		var params = this.scope('action')+'=input';
		return params;
	}

	this.outputForm = function () {
		var params = this.scope('action')+'=output';

		for ( var i=0; i<this.params.length; i++ ) {
			var id =  this.scope('param-'+this.params[i]);
			params += '&' + id + '=' + document.getElementById(id).value;
		}
		return params;
	}

	this.action = function (str) {
		if ( str == 'input' ) {
			this.replaceForm(this.send(this.inputForm()));
		}
		if ( str == 'output' ) {
			this.replaceForm(this.send(this.outputForm()));
		}
	}

	this.send = function(str) {
		var req = new XMLHttpRequest();

		req.open("POST",this.script,false);
		req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		req.send(str);

		return req.responseText;
	}

	this.replaceForm = function(str) {
		this.container.innerHTML = str;
	}

	this.script = document.location;
	this.container = document.getElementById(this.scope("container"));
}
