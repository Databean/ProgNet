var triangles = [];

var points = [[0, 760], [640, 0], [1280, 760]];
var maxdepth = 8;

//triangles[triangles.length] = makeTriangle(points[0][0], points[0][1], points[1][0], points[1][1], points[2][0], points[2][1]);

function sierpinski(x1, y1, x2, y2, x3, y3, depth) {
	if(depth < 1) {
		return;
	}
	var x1m = (x1 + x2)/2;
	var y1m = (y1 + y2)/2;
	
	var x2m = (x2 + x3)/2;
	var y2m = (y2 + y3)/2;
	
	var x3m = (x3 + x1)/2;
	var y3m = (y3 + y1)/2;
	
	var triangle = makeTriangle(x1m, y1m, x2m, y2m, x3m, y3m);
	triangles[triangles.length] = triangle;
	triangle.color.red = 255 - (255. * depth) / maxdepth;
	triangle.color.green = 255 - (255. * depth) / maxdepth;
	triangle.color.blue = 255 - (255. * depth) / maxdepth;
	sierpinski(x1, y1, x1m, y1m, x3m, y3m, depth - 1);
	sierpinski(x1m, y1m, x2, y2, x2m, y2m, depth - 1);
	sierpinski(x3m, y3m, x2m, y2m, x3, y3, depth - 1);
}

sierpinski(points[0][0], points[0][1], points[1][0], points[1][1], points[2][0], points[2][1], maxdepth);

while(true) {
	frameWait();
}