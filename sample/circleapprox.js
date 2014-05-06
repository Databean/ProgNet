
var squareSize = 5;
var numSquares = 100;

function isColored(i, j) {
	
	i -= numSquares / 2.;
	j -= numSquares / 2.;
	
	var dist = Math.sqrt((i * i) + (j * j));
	if(dist >= (numSquares / 2.) - 0.5 && dist <= (numSquares / 2.) + 0.5) {
		return true;
	} else {
		return false;
	}
}

var squares = [];
for(var i = 0; i <= numSquares; i++) {
	squares[i] = [];
	for(var j = 0; j <= numSquares; j++) {
		squares[i][j] = makeQuad(
			i * squareSize, j * squareSize, 
			(i + 1) * squareSize, j * squareSize,
			(i + 1) * squareSize, (j + 1) * squareSize,
			i * squareSize, (j + 1) * squareSize);
		
		if(isColored(i, j)) {
			squares[i][j].color.red = 255;
		}
		
	}
}




while(true) {
	frameWait();
}