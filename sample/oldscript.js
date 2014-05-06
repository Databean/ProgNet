var maxX = color.length;
var maxY = color[0].length;

var aliveNextIteration;

function squareValue(x, y) {
	if(x < 0 || x >= maxX || y < 0 || y >= maxY) {
		return 0;
	} else {
		return color[x][y].red > 120;
	}
}

function numAliveNeighbors(x, y) {
	var sum = 0;
	sum += squareValue(x-1,y-1);
	sum += squareValue(x,y-1);
	sum += squareValue(x+1,y-1);
	sum += squareValue(x+1,y);
	sum += squareValue(x+1,y+1);
	sum += squareValue(x,y+1);
	sum += squareValue(x-1,y+1);
	sum += squareValue(x-1,y);
	return sum;
}

for(var x = 0; x < maxX; x++) {
	for(var y = 0; y < maxY; y++) {
		color[x][y].red = Math.random() * 255;
		color[x][y].green = Math.random() * 255;
		color[x][y].blue = Math.random() * 255;
	}
}

while(true) {
	
	aliveNextIteration = new Array();
	
	for(var x = 0; x < maxX; x++) {
		aliveNextIteration[x] = new Array();
		for(var y = 0; y < maxY; y++) {
			var neighbors = numAliveNeighbors(x, y);
			if(squareValue(x, y) > 0) {
				aliveNextIteration[x][y] = neighbors > 1 && neighbors < 4;
			} else {
				aliveNextIteration[x][y] = neighbors == 3;
			}
		}
	}
	
	for(var x = 0; x < maxX; x++) {
		for(var y = 0; y < maxY; y++) {
			if(aliveNextIteration[x][y]) {
				color[x][y].red = 255;
				color[x][y].green = 255;
				color[x][y].blue = 255;
			} else {
				
				color[x][y].red = 0;
				color[x][y].green = 0;
				color[x][y].blue = 0;
			}
		}
	}
}