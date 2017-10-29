const char* root_page = 
"\
<h1>Tower Info</h1>\
<h3>    Time: %dh: %dm: %ds</h3>\
<h3>    PH: %f</h3>\
<h3>    Air temp: %f</h3>\
<h3>    Water temp: %f</h3>\
<h3>    Humidity: %f</h3>\
<h3>    main water level: %s</h3>\
<h3>    backup water level: %s</h3>\
</br>\
<h1>Tower Controls</h1>\
<form action=\"/login\" method=\"POST\">\
<input type=\"text\" name=\"username\" placeholder=\"Username\"></br>\
<input type=\"password\" name=\"password\" placeholder=\"Password\"></br>\
<input type=\"submit\" value=\"Login\"></form>\
<p>Try 'John Doe' and 'password123' ...</p>\
";
