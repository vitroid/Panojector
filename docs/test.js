//const config = { }
//const math = create(all, config)



function equi2mer(v)
{
    var tx = v.re;
    var ty = 2.0*Math.atan(Math.exp(v.im*math.pi))/math.pi - 0.5;
    return math.complex(tx, ty);
}

function mer2equi(v)
{
    var theta = v.im * math.pi;
    var tx    = v.re;
    var ty    = log( tan(math.pi/4.0 + theta/2.0) ) / math.pi;
    return math.complex(tx, ty);
}


var slide;

class Slide
{
  constructor(x, y){
    this.displace = math.complex(-x,-y);
  }
  func(dst)
  {
    return math.add(dst, this.displace);
  } 
}


var scaling;

class Scaling
{
  constructor(scalex, scaley){
    this.scalex = scalex;
    this.scaley = scaley;
  }
  func(dst)
  {
      var dstx=dst.re / this.scalex;
      //padding
    /*if ( dstx < -1.0 )
	  dstx = -1.0;
    else if ( dstx > 1.0 )
	  dstx = 1.0;
    */
    var dsty = dst.im / this.scaley;
    return math.complex(dstx,dsty);
  } 
}

var equi2ste;

class Equi2Ste
{
  constructor(fov){
    this.lmax = Math.tan(fov * math.pi / (180.0*4.0));
  }
  func(dst)
  {
    var L = 2.0*this.lmax*math.abs(dst);
    var theta =  0.5 - 2.0 * atan( L / 2.0 ) / math.pi;
    var phi;
    if ( dst.re == 0 ){
      phi = 0.25; //math.pi / 2.0;
      if ( dst.im < 0.0 ){
	    phi += 0.5; //math.pi;
      }
    }
    else{
      phi = atan2( dst.im, dst.re ) / (2.0*math.pi);
    }
    phi -= floor( phi + 0.5 ); // * (2.0*math.pi);
    phi *= 2; ///= math.pi;
    return math.complex(phi, theta);
  }
}

var tile;

class Tile
{
  constructor(aspect, story){
    this.aspect = aspect;
    var bw = 2.0 / story;
    this.L = sqrt(2.0*2.0 + bw*bw); // a period of the slant image
    this.h = 2.0*bw / this.L;            // height of the slant image
    this.offset = (this.L-sqrt(this.L*this.L-4*this.h*this.h))/2.0;
    this.e1x = 2.0/this.L;
    this.e1y = bw/this.L;
    this.e2x = -bw/this.L;
    this.e2y = 2.0/this.L;
  }
  func(dst){
    var dstx = dst.re;
    var dsty = dst.im;
    var sx = dstx*this.e1x + dsty*this.e1y;
    var sy = dstx*this.e2x + dsty*this.e2y;
    var dy = floor( sy / this.h + 0.5 );
    sy -= this.h*dy;
    sx += (this.L-this.offset)*dy;
    sx = sx / this.h * this.aspect * 2.0;
    sy = sy / this.h * this.aspect * 2.0;
    sx -= floor( sx/2.0 + 0.5 )*2.0;
    return math.complex(sx, sy);
  }
};

var ctile;

class CTile
{
  constructor(aspect){
    this.aspect = aspect;
    var bw = 2*aspect;
    this.L = sqrt(2.0*2.0 + bw*bw); // a period of the slant image
    this.h = 2.0*bw / this.L;            // height of the slant image
    this.offset = (this.L-sqrt(this.L*this.L-4*this.h*this.h))/2.0;
      //printf("%f %f %f\n", L,h,offset);
    this.e1x = 2.0/this.L;
    this.e1y = bw/this.L;
    this.e2x = -bw/this.L;
    this.e2y = 2.0/this.L;
  }
  func(dst){
    var dstx = dst.re;
    var dsty = dst.im;
    var sx = dstx*this.e1x + dsty*this.e1y;
    var sy = dstx*this.e2x + dsty*this.e2y;
    var dy = floor( sy / this.h + 0.5 );
    sy -= this.h*dy;
    sx += (this.L - this.offset)*dy;
    sx = sx / this.h * this.aspect * 2.0;
    sy = sy / this.h * this.aspect * 2.0;
    sx -= floor( sx/2.0 + 0.5 )*2.0;
    return math.complex(sx, sy);
  }
};


var userfunc = function(v){
  //return mer2equi(equi2mer(v));
  //return equi2ste.func(v);
    return tile.func(scaling.func(mer2equi(equi2ste.func(v))));
  //return scaling.func(v);
  //return slide.func(v);
};

let img;
                    
function preload()
{
  img = loadImage("./sample3.jpg");
}


function setup() {
  const cw = 600;
  const ch = 600;
  createCanvas(cw, ch);
  // = loadImage("/color-checker-board.png");
  //img = createImage(1000,400);
  img.loadPixels();
  var w = img.width;
  var h = img.height;
  console.log(w,h);
  ctile    = new CTile(h/w);
  tile     = new Tile(h/w*3, 3);
  equi2ste = new Equi2Ste(90.0);
  scaling  = new Scaling(0.2, 0.2);
  slide    = new Slide(0., 0.);
//}

//function draw() {
  var canvas = new p5.Image(cw, ch);
  canvas.loadPixels();
  
  background(220);
  // scale w to 1
  var aspect = h / w * 2;
  //console.log(aspect,w,h);
  var wh = w / 2;
  let x,y,d;
  for (y = 0; y < ch; y++) {
    var im = (y - ch/2) / (ch/2);
    for (x = 0; x < cw; x++) {
      var re = (x - (cw/2)) / (cw/2);
      const v = math.complex(re, im);
      const t = userfunc(v); //math.multiply(math.log(v), 1/6.28);//v.inverse();
      var sre = t.re;
      var sim = t.im;
      sre -= floor(sre / 2.0 + 0.5) * 2.0;
      sim -= floor(sim / aspect + 0.5) * aspect;
      const sx = (int)(sre * wh + wh);
      const sy = (int)(sim * wh + h / 2);
      if ((0 <= sx) && (sx < w) && (0 <= sy) && (sy < h)) {
        for(d=0;d<4;d++){
          canvas.pixels[(y * cw + x)*4+d] = img.pixels[(sy * w + sx)*4+d];
        }
      } else {
        canvas.pixels[(y * cw + x)*4+0] = 100;
        canvas.pixels[(y * cw + x)*4+1] = 100;
        canvas.pixels[(y * cw + x)*4+2] = 0;
        canvas.pixels[(y * cw + x)*4+3] = 100;
      }
    }
  }
  canvas.updatePixels();
  image(canvas, 0, 0);
}

