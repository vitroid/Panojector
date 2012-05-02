#include "plugin.hpp"
#include <iostream>
#include <dlfcn.h>

using namespace std;


IplImage* scan( int size, IplImage* const src, const Projector& projector )
{
  IplImage* dst = cvCreateImage( cvSize(size, size), IPL_DEPTH_8U, 3 );
  IplImage* subpix = cvCreateImage( cvSize(2,2), IPL_DEPTH_8U, 3 );
  for(int y=0;y<size;y++){
    for(int x=0;x<size;x++){
      for(int dy=0;dy<2;dy++){
	for(int dx=0;dx<2;dx++){
	  float rx = 2.0*(x+dx) / size - 1.0;
	  float ry = 2.0*(y+dy) / size - 1.0;
	  float sx,sy;
	  if ( projector.map(rx,ry,sx,sy) )
	    getpixel(src,sx,sy,subpix,dx,dy);
	  else
	    white(subpix,dx,dy);
	}
      }
      average(subpix, dst,x,y);
    }
  }
  cvReleaseImage( &subpix );
  return dst;
}





void usage( int argc, char* argv[] )
{
    fprintf( stderr, "Usage: %s [-s x] inputfilename [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-s 400\tPicture size.\n" ); 
    exit(1);
}

//first arg: origin file
//second: projection plugins


int main( int argc, char* argv[] ){
  int width=400;

  int c = 1;
  while ( c < argc ){
    if ( 0 == strcmp( argv[c], "-s" )){
      c++;
      width = atoi( argv[c] );
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
  char dstfilename[1000];
  sprintf( dstfilename, "%s_proj.jpg", *argv );
  IplImage* src = cvLoadImage( *argv, CV_LOAD_IMAGE_COLOR );
  fprintf( stderr, "%s\n", *argv );
  argv += 1;
  argc -= 1;
  Projector* projector = plugin_load( argc, argv );
  IplImage* dst = scan( width, src, *projector );
  cvSaveImage( dstfilename, dst );
  cvReleaseImage (&src);
  cvReleaseImage (&dst);
}
