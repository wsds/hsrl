var ctx =canvas.getContext("2d");
var context=ctx;
var img = new Image();

window.pos_x = 0;
window.pos_y = 0;


img.src = 'images.jpg';
img.load=function(){
color='#00ff00';
}

img.error=function(){
color='#ffffff';
}

var w = window.innerWidth;
var h = window.innerHeight;
var w2 = w/2;
var h2 = h/2;

var curves = [];
for( var i = 0; i < 0; i++ ) {
	curves.push({
		current: Math.random() * 1000,
		inc: Math.random() * 0.005 + 0.002,
		color: '#'+(Math.random()*0xFFFFFF<<0).toString(16) // Random color
	});
}

var move_x=0;
var move_y=0;
var color='#88c0dd ';

var p = [0,0, 0,0, 0,0, 0,0];
var animate = function() {
	// Clear the screen - note that .globalAlpha is still honored,
	// so this will only "darken" the sceen a bit
	
	ctx.globalCompositeOperation = 'source-over';
ctx.fillStyle = '#0099cd';
ctx.fillRect( 0, 0, w, h );

ctx.fillStyle = color;
ctx.fillRect( move_x, move_y, 50,50 );

	// Use the additive blend mode to draw the bezier curves
	ctx.globalCompositeOperation = 'lighter';

	// Calculate curve positions and draw
	for( var i = 0; i < maxCurves; i++ ) {
		var curve = curves[i];
		curve.current += curve.inc;
		for( var j = 0; j < p.length; j+=2 ) {
			var a = Math.sin( curve.current * (j+3) * 373 * 0.0001 );
			var b = Math.sin( curve.current * (j+5) * 927 * 0.0002 );
			var c = Math.sin( curve.current * (j+5) * 573 * 0.0001 );
			p[j] = (a * a * b + c * a + b) * w * c + w2;
			p[j+1] = (a * b * b + c - a * b *c) * h2 + h2;
		}

		ctx.beginPath();
		ctx.moveTo( p[0], p[1] );
		ctx.bezierCurveTo( p[2], p[3], p[4], p[5], p[6], p[7] );
		ctx.strokeStyle = curve.color;
		ctx.stroke();
	}
	if(window.pos_x>200)window.pos_x=0;
	if(window.pos_y>200)window.pos_y=0;
	window.pos_x++;
	window.pos_y++;
	ctx.drawImage(img, 50, 50);
	//setTimeout(animate, 100);
      context.fillText('Hello World!', 150, 100);
};

context.font = 'italic 40pt Calibri';

      context.fillText('Hello World!', 150, 100);
//context.font="italic small-caps bold 12px arial";

// The vertical touch position controls the number of curves;
// horizontal controls the line width
var maxCurves = 70;
document.addEventListener( 'touchmove', function( ev ) {
move_x=ev.touches[0].pageX;
move_y=ev.touches[0].pageY;
	//ctx.lineWidth = (ev.touches[0].pageX/w) * 20;
	//maxCurves = Math.floor((ev.touches[0].pageY/h) * curves.length);
}, false );

document.addEventListener( 'touchstart', function( ev ) {
move_x=ev.touches[0].pageX;
move_y=ev.touches[0].pageY;
	//ctx.lineWidth = (ev.touches[0].pageX/w) * 20;
	//maxCurves = Math.floor((ev.touches[0].pageY/h) * curves.length);
}, false );


ctx.fillStyle = '#0099cd';
ctx.fillRect( 0, 0, w, h );

ctx.fillStyle = '#88c0dd ';
ctx.fillRect( 0, 0, 10,11 );



ctx.globalAlpha = 1.0;
ctx.lineWidth = 2;
setInterval( animate, 16 );

