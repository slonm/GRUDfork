
DROP TABLE IF EXISTS countries;
CREATE TABLE countries (
	id integer,
	country varchar(128),
	FIPS104 varchar(2),
	ISO2	varchar(2),
	ISO3	varchar(3),
	ISON	varchar(3),
	internet varchar(2),
	capital	varchar(25),
	map_reference	varchar(50),
	nationality_singular	varchar(35),
	nationality_plural	varchar(35),
	currency	varchar(30),
	currency_code	varchar(3),
	population	integer,
	title	varchar(50),
	comment	varchar(2048)
);

DROP TABLE IF EXISTS regions;
CREATE TABLE regions (
	id integer,
	id_country integer,
	region	varchar(128),
	code	varchar(2),
	adm1code	varchar(4)
);

DROP TABLE IF EXISTS cities;
CREATE TABLE cities (
	id integer,
	id_country integer,
	id_region integer,
	city	varchar(128),
	longitude	float,
	latitude	float,
	timezone	varchar(32),
	code	varchar(4)
);

DROP TABLE IF EXISTS people;
CREATE TABLE people (
	id serial,
	name varchar(32),
	lastname varchar(32),
	eye_color integer,
	gender integer,
	birthday date
);

DROP TABLE IF EXISTS colors;
CREATE TABLE colors (
	id serial,
	name varchar(30)
);

DROP TABLE IF EXISTS genders;
CREATE TABLE genders (
	id serial,
	name varchar(15)
);

