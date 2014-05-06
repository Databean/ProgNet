var width = 1280;
var height = 760;

var mod = 0.1;

var line = makeLine(0, 0, width, height);

for(var ang = 0.; ang < Math.PI * 2.; ang += mod) {
	var v = makeLine(
		width / 2. + (width / 2. * Math.cos(ang)), 
		height / 2. + (height / 2. * Math.sin(ang)), 
		width / 2. + (width / 2. * Math.cos(ang + mod)), 
		height / 2. + (height / 2. * Math.sin(ang + mod)));
}

while(true) {
	var d = new Date();
	var ms = d.getMilliseconds();
	//var ang = d / 1000 * Math.PI;
	var ang = Math.atan2(getMouseY() - (height / 2.), getMouseX() - (width / 2.));
	line[0].x = width / 2.;
	line[0].y = height / 2.;
	line[1].x = width / 2. + (width / 2. * Math.cos(ang));
	line[1].y = height / 2. + (height / 2. * Math.sin(ang));
	
	frameWait();
}