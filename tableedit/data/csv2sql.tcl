#!/usr/bin/tclsh8.4

package require csv

# id, country, fips104,iso2,iso3,ison,internet,capital,map_reference,nationality_singular,
# nationality_plural, currency, currency_code, population, title, comment

# id , population - integer

#countries
set ofh [open "insert.sql" "w"]

set ifh [open "countries.txt" "r"]

#puts $ofh "BEGIN;"
puts $ofh "DELETE FROM COUNTRIES;"

proc trim_val { val } {
}

set str ""
set n [gets $ifh str]

while { $n ne -1 } {
	set n [gets $ifh str]

	if { $str ne "" } {
		set l [ csv::split $str "," "\"" ]
 
		puts -nonewline $ofh "INSERT INTO countries (id,country,fips104,iso2,iso3,ison,internet,capital,\
			map_reference,nationality_singular,nationality_plural, currency,\
			currency_code, population, title, comment) VALUES ( "

		for { set i 0 } { $i < [llength $l] } { incr i } {
			set val [lindex $l $i]

			set val [string trim $val "\" "]
			if { $val eq "" } {
				set val "NULL"
			}

			set val [string map { "'" "''" } $val]
			
			if { [expr $i ne 0 ] && [expr $i ne 13 ] && $val ne "NULL" } {
				set val "'$val'"
			} else {
			}
			set l [lreplace $l $i $i $val]
		}

		foreach { id country fips104 iso2 iso3 ison internet capital map_reference 
			nationality_singular nationality_plural currency 
			currency_code population title comment } $l {
				puts $ofh "$id,$country,$fips104,$iso2,$iso3,$ison,$internet,$capital,\
				$map_reference,$nationality_singular,$nationality_plural,$currency,\
				$currency_code,$population,$title,$comment);"
			}
	}
}

#puts $ofh "END;"
close $ifh

#CREATE TABLE regions (
#        id integer,
#        id_country integer,
#        region  varchar(128),
#        code    varchar(2),
#       adm1code        varchar(4)
# );
set ifh [open "regions.txt" "r"]
puts $ofh "DELETE FROM REGIONS;"

set str ""
set n [gets $ifh str]

while { $n ne -1 } {
	set n [gets $ifh str]

	if { $str ne "" } {
		set l [ csv::split $str "," "\"" ]
 
		puts -nonewline $ofh "INSERT INTO regions (id,id_country,region,code,adm1code) VALUES("

		for { set i 0 } { $i < [llength $l] } { incr i } {
			set val [lindex $l $i]

			set val [string trim $val "\" "]
			if { $val eq "" } {
				set val "NULL"
			}

			set val [string map { "'" "''" } $val]
			
			if { [expr $i ne 0 ] && [expr $i ne 1 ] && $val ne "NULL" } {
				set val "'$val'"
			} else {
			}
			set l [lreplace $l $i $i $val]
		}

		foreach { id id_country region code adm1code } $l {
				puts $ofh "$id,$id_country,$region,$code,$adm1code);"
			}
	}
}

close $ifh

set ifh [open "cities.txt" "r"]
#id         | integer                |
#  id_country | integer                |
#  id_region  | integer                |
#  city       | character varying(128) |
#  longitude  | double precision       |
#  latitude   | double precision       |
#  timezone   | character varying(32)  |
#  code       | character varying(2)   |
puts $ofh "DELETE FROM CITIES;"

set str ""
set n [gets $ifh str]

while { $n ne -1 } {
	set n [gets $ifh str]

	if { $str ne "" } {
		set l [ csv::split $str "," "\"" ]
 
		puts -nonewline $ofh "INSERT INTO cities (id,id_country,id_region,city,latitude,longitude,timezone,code) VALUES("

		for { set i 0 } { $i < [llength $l] } { incr i } {
			set val [lindex $l $i]

			set val [string trim $val "\" "]
			if { $val eq "" } {
				set val "NULL"
			}

			set val [string map { "'" "''" } $val]
			
			if { [expr $i ne 0 ] && [expr $i ne 1 ] && [expr $i ne 2] && [expr $i ne 4] && [expr $i ne 5] && $val ne "NULL" } {
				set val "'$val'"
			} else {
			}
			set l [lreplace $l $i $i $val]
		}

		foreach { id id_country id_region city latitude longitude timezone dmaid code } $l {
				puts $ofh "$id,$id_country,$id_region,$city,$latitude,$longitude,$timezone,$code);"
			}
	}
}
close $ifh

close $ofh

# end of csv. now just some data for combobox test

