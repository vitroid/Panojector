#include "plugin.hpp"
#include <iostream> //for debug
class Box : public Projector {
protected:
  float boxx,boxy,boxz,eyex,eyey,eyez;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-e x,y,z][-b x,y,z] [projectors]\n", argv[0] );
    fprintf( stderr, "Convert equirectangular 2:1 image to a developed box.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-e 0.5,0.5,0.5\tSpecify eye point (center = 0.5).\n" ); 
    fprintf( stderr, "\t-b 1,1,1\tSpecify aspect ratio of the box.\n" ); 
    exit(1);
  }
  Box(int argc, char* argv[])
  {
    eyex = eyey = eyez = 0.5;
    boxx = boxy = boxz = 1;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-b" )){
	c++;
	sscanf(argv[c], "%f,%f,%f", &boxx, &boxy, &boxz );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-e" )){
	c++;
	sscanf(argv[c], "%f,%f,%f", &eyex, &eyey, &eyez );
	c++;
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
  uchar* map(float dstx, float dsty)
  {
    // in image coord (2(a+b) x 2(a+b))
    dstx *= (boxx+boxy);
    dsty *= (boxx+boxy);
    //dsty = dsty + (eye - 0.5) * boxz;
    float h,v;//angles
    //angles for the center of each face
    float h0 = atan( boxx*eyex/(boxy*eyey) ) - M_PI;
    float h1 = h0 + M_PI/2;
    float h2 = h1 + M_PI/2;
    float h3 = h2 + M_PI/2;
    if ( ( -boxz / 2 <= dsty ) && ( dsty <= boxz / 2 ) ){
      //in the horizontal belt
      if ( dstx < -boxy ){
	dsty = dsty + (eyez - 0.5) * boxz;
	//panel 0
	// x offset from the center of panel
	dstx += boxy + boxx*(1.0-eyex);
	h = atan( dstx / (boxy*eyey) );
	v = atan( dsty / sqrt( (boxy*eyey)*(boxy*eyey) + dstx*dstx ) );
	h += h0;
	dstx = h / M_PI;
	dsty = v / M_PI; 
      }
      else if ( dstx < 0 ){
	dsty = dsty + (eyez - 0.5) * boxz;
	//panel 1
	dstx += boxy*(1.0-eyey);
	h = atan( dstx / (boxx*(1-eyex)) );
	v = atan( dsty / sqrt( (boxx*(1-eyex))*(boxx*(1-eyex)) + dstx*dstx ) );
	h += h1;
	dstx = h / M_PI;
	dsty = v / M_PI; 
      }
      else if ( dstx < boxx ){
	dsty = dsty + (eyez - 0.5) * boxz;
	//panel 2
	dstx -= boxx*(1-eyex);
	h = atan( dstx / (boxy*(1-eyey)) );
	v = atan( dsty / sqrt( (boxy*(1-eyey))*(boxy*(1-eyey)) + dstx*dstx ) );
	h += h2;
	dstx = h / M_PI;
	dsty = v / M_PI; 
      }
      else{
	dsty = dsty + (eyez - 0.5) * boxz;
	//panel 3
	dstx -= (boxy*(1-eyey) + boxx);
	h = atan( dstx / (boxx*eyex) );
	v = atan( dsty / sqrt( (boxx*eyex)*(boxx*eyex) + dstx*dstx ) );
	h += h3;
	dstx = h / M_PI;
	dsty = v / M_PI; 
      }
    }
    else if ( ( boxz/2 < dsty ) && ( dsty < boxz/2+boxx ) &&
	      ( -boxy < dstx ) && (dstx < 0) ){
      //dsty = dsty + (eye - 0.5) * c;
      //panel 4 nadir
      dstx += boxy*(1-eyey);
      dsty -= (boxz/2+boxx*(1-eyex));
      h = atan(dsty / dstx);
      if (dstx < 0 ){
	h += M_PI;
      }
      float r = sqrt( dstx*dstx + dsty*dsty );
      v = atan( eyez*boxz / r );
      h += h2;
      dstx = h / M_PI;
      dsty = v / M_PI; 
    }
    else if ( ( -boxz/2 - boxx < dsty ) && ( dsty < -boxz/2 ) &&
	      ( -boxy < dstx ) && (dstx < 0) ){
      //dsty = dsty + (eye - 0.5) * boxz;
      //panel 5 zenith
      dstx += boxy*(1-eyey);
      dsty += boxz/2+boxx*(1-eyex);
      h = atan(dsty / dstx);
      if (dstx < 0 ){
	h += M_PI;
      }
      float r = sqrt( dstx*dstx + dsty*dsty );
      v = -atan( (boxz-eyez*boxz) / r );
      h = h2 - h;
      dstx = h / M_PI;
      dsty = v / M_PI; 
    }
    else{
      //return child->map( 0,0 );
      return 0;
    }
    return child->map( dstx, dsty );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Box(argc, argv);
}

extern "C" void destroy(Box* p) {
    delete p;
}
