#include "plugin.hpp"
#include <cstring>

class Ribbon : public Projector {
protected:
  Projector* child;
  float bw;
  float aspect;
  int repeat;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x][-s x][-r] [projectors]\n", argv[0] );
    fprintf( stderr, "Make a ribbon of a long image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 0.5\tSpecify aspect ratio of the original image.\n" ); 
    fprintf( stderr, "\t-s 8\tSpecify number of stories.\n" ); 
    fprintf( stderr, "\t-r\tRepeat.\n" ); 
    exit(1);
  }
  Ribbon(int argc, char* argv[])
  {
    aspect = 0.5;
    float story  = 8.0;
    repeat = 0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	if ( index( argv[c], '/' ) ){
	  float num,den;
	  sscanf(argv[c], "%f/%f", &num,&den);
	  aspect = num/den;
	}
	else
	  aspect = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-s" )){
	c++;
	story = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-r" )){
	c++;
	repeat = 1;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    bw = 2.0 /story;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    dsty += 1.0;
    int dan = (int)floor( dsty / bw);
    float loopwidth = bw * M_PI / 2.0;
    float danwidth = 2.0 - bw + loopwidth;
    float sx,sy;
    float h = bw / 2.0;
    if ( dan % 2 == 0 ){
      //even line
      sx = dan * danwidth + dstx;
      sy = dsty - dan*bw - h;
      //right end
      if ( dstx > 1.0 - h ){
	float delta = dstx - 1.0 + h;
	float a0 = asin( delta / h);
	float a1 = M_PI - a0;
	float c0 = cos(a0);
	float c1 = cos(a1);
	float y  = sy + 0.5*bw;
	float b0 = y - h*c0;
	float b1 = y - h*c1;
	a0 = a0 * h;
	if ( b1 < h ){
	  return 0;
	}
	else
	{
	  sx = sx - delta + a0;
	  sy -= h*(1.0-c0);
	}
      }
      //left end
      if ( dstx < -1.0 + h ){
	float delta = -(dstx + 1.0 - h);//width on the screen
	float a0 = asin( delta / h);
	float a1 = M_PI - a0; // width along the ribbon
	float c0 = cos(a0);
	float c1 = cos(a1);
	float y  = sy + h;
	float b0 = y - h*c0;
	float b1 = y - h*c1;
	//cout << delta << endl;
	a0 = a0 * h;
	a1 = a1 * h;
	if ( b0 > h ){
	  return 0;
	}
	else
	if ( b1 < h ){
	  //left; lower; turn
	  sx = sx + delta - a1;
	  sy += h*(1.0-c1);
	}
	else{
	  //left; lower
	  sx = sx + delta - a0;
	  sy += h*(1.0-c0);
	}
      }
    }
    else{
      //odd line
      sx = dan * danwidth - dstx;
      sy = dsty - dan * bw - 0.5*bw;
      //right end
      if ( dstx > 1.0 - h ){
	float delta = dstx - 1.0 + h;
	float a0 = asin( delta / h);
	float a1 = M_PI - a0;
	float c0 = cos(a0);
	float c1 = cos(a1);
	float y  = sy + 0.5*bw;
	float b0 = y  - h*c0;
	float b1 = y  - h*c1;
	a0 = a0 * h;
	a1 = a1 * h;
	if ( b0 > h ){
	  return 0;
	}
	else
	if ( b1 < h ){
	  //right; lower; turn
	  sx = sx + delta - a1;
	  sy += h*(1.0-c1);
	}
	else {
	  //right; lower end
	  sx = sx + delta - a0;
	  sy += h*(1.0-c0);
	}
      }
      //left end
      if ( dstx < -1.0 + h ){
	float delta = (dstx + 1.0 - h);
	float a0 = asin( delta / h);
	float a1 = M_PI - a0;
	float c0 = cos(a0);
	float c1 = cos(a1);
	float y  = sy + 0.5*bw;
	float b0 = y  - h*c0;
	float b1 = y  - h*c1;
	a0 = a0 * h;
	if ( b1 < h ){
	  return 0;
	}
	else
	{
	  //left; upper end
	  sx = sx + delta - a0;
	  sy -= h*(1.0-c0);
	}
      }
    }
    float stripw = bw / aspect;
    sx = sx - stripw / 2;
    sx += (danwidth - loopwidth)/2;
    if ( repeat==0 && ( sx < -stripw*0.5 || sx > stripw*0.5 ) )
      return 0;
    sx -= floor( sx / stripw + 0.5)*stripw;
    sx = sx*2.0 / stripw;
    sy = sy*2.0 / stripw;
    return child->map( sx, sy, srcx, srcy );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Ribbon(argc, argv);
}

extern "C" void destroy(Ribbon* p) {
    delete p;
}
