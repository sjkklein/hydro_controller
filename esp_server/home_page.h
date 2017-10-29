const char* root_page = 
"\
<h1>Tower Info</h1>\
<h3>Time: %dh: %dm: %ds</h3>\
<h3>PH: %s</h3>\
<h3>Air temp: %s</h3>\
<h3>Water temp: %s</h3>\
<h3>Humidity: %s</h3>\
<h3>main water level: %s</h3>\
<h3>backup water level: %s</h3>\
</br>\
<h1>Tower Controls</h1>\
<form action=\"/set_time\" method=\"POST\">\
<input type=\"text\" name=\"hour\" placeholder=\"hour\">\
<input type=\"text\" name=\"minute\" placeholder=\"minute\">\
<input type=\"text\" name=\"second\" placeholder=\"second\"></br>\
<input type=\"submit\" value=\"Set time\"></form>\
<p>Try 'John Doe' and 'password123' ...</p>\
";
