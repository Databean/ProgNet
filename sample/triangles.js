//var myIndex = makeTriangle(1, 1, 100, 1, 1, 100);

var a = 50.;
var b = 100.;

var triangles = [];
var vx = [];
var vy = [];

var pushback = 0.1;

for(var i = 0; i < 10; i++) {
	triangles[i] = makeTriangle(0., -a/2., -b, a/2., b, a/2.);
	triangles[i].offset.x = Math.random() * 1280.;
	triangles[i].offset.y = Math.random() * 760.;
	triangles[i].rotation = Math.random() * 360.;
	vx[i] = (Math.random() - 0.5) * 5;
	vy[i] = (Math.random() - 0.5) * 5;
}
while(true) {
	for(var i = 0; i < 10; i++) {
		if(triangles[i].offset.x < 0) {
			vx[i] += pushback;
		}
		if(triangles[i].offset.y < 0) {
			vy[i] += pushback;
		}
		if(triangles[i].offset.x > 1280) {
			vx[i] -= pushback;
		}
		if(triangles[i].offset.y > 760) {
			vy[i] -= pushback;
		}
		triangles[i].offset.x = triangles[i].offset.x + vx[i];
		triangles[i].offset.y += vy[i];
	}
	
	frameWait();
}