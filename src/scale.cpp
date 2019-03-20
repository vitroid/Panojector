#include "plugin.hpp"

class Scale : public Projector {
protected:
  float x,y;
  int pad;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-x x][-y y][-xy x] [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-x 1.0\tSpecify scale factor for width.\n" ); 
    fprintf( stderr, "\t-y 1.0\tSpecify scale factor for height.\n" ); 
    fprintf( stderr, "\t-xy 1.0\tSpecify scale factor.\n" ); 
    fprintf( stderr, "\t-p\tPad.\n" ); 
    exit(1);
  }
  Scale(int argc, char* argv[])
  {
    x=1;
    y=1;
    pad = 0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-x" )){
	c++;
	x = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-y" )){
	c++;
	y = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-xy" )){
	c++;
	y = atof( argv[c] );
	x = y;
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-p" )){
	c++;
	pad = 1;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map( complex<float> dst )
  {
    float dstx=dst.real() / x;
    if ( pad ){
      if ( dstx < -1.0 )
	dstx = -1.0;
      else if ( dstx > 1.0 )
	dstx = 1.0;
    }
    float dsty = dst.imag() / y;
    return child->map( complex<float>(dstx,dsty) );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Scale(argc, argv);
}

extern "C" void destroy(Scale* p) {
    delete p;
}
