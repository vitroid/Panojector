#include "plugin.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>

using namespace std;


Mat scan( int size, Projector& projector, int rgb[3] )
{
  Mat dst(size, size, CV_8UC3);
  const int super = 3;
  Mat subpix(super,super, CV_8UC3);
  for(int y=0;y<size;y++){
    for(int x=0;x<size;x++){
      for(int dy=0;dy<super;dy++){
	for(int dx=0;dx<super;dx++){
	  float rx = (super*x+dx)*2.0 / (size*super) - 1.0;
	  float ry = (super*y+dy)*2.0 / (size*super) - 1.0;
	  complex<float> r(rx,ry);
	  Vec3b pixel = projector.map(r);
	  //if ( pixel ){
	  subpix.at<Vec3b>(dy,dx) = pixel;
	    /*}
	  else{
	    for ( int ch=0; ch<3; ch++ ){
	      subpix.at(dy,dx)[ch] = rgb[ch];
	    }
	  }
	    */
	}
      }
      vector<Mat> channels;
      split(subpix, channels);
      dst.at<Vec3b>(y,x) = Vec3b(mean(channels[0])[0], mean(channels[1])[0], mean(channels[2])[0]);
    }
  }
  //cvReleaseImage( &subpix );
  return dst;
}





void usage( int argc, char* argv[] )
{
    fprintf( stderr, "Usage: %s [-s x][-b r,g,b][-o outputfilename] [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-s 400\tPicture size.\n" ); 
    fprintf( stderr, "\t-b 1,0.5,0.5\tBackground color in RGB intensities.\n" ); 
    fprintf( stderr, "\t-o pano.jpg\tFilename to be output.\n" ); 
    exit(1);
}

//first arg: origin file
//second: projection plugins


int main( int argc, char* argv[] ){
  int width=400;
  char dstfilename[1000];
  int rgb[] = {127,127,255,};//bgr

  strcpy( dstfilename, "pano.jpg" );

  std::ofstream to("pano.log");

  for ( int i=0; i<argc; i++ ){
    to << argv[i] << " ";
  }
  to << endl;
  to.close();

  int c = 1;
  while ( c < argc ){
    if ( 0 == strcmp( argv[c], "-s" )){
      c++;
      width = atoi( argv[c] );
      c++;
    }
    else if ( 0 == strcmp( argv[c], "-o" )){
      c++;
      strcpy( dstfilename, argv[c] );
      c++;
    }
    else if ( 0 == strcmp( argv[c], "-b" )){
      float r,g,b;
      c++;
      sscanf(argv[c], "%f,%f,%f", &r,&g,&b);
      rgb[0] = b * 255;
      rgb[1] = g * 255;
      rgb[2] = r * 255;
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
  Projector* projector = plugin_load( argc, argv );
  Mat dst = scan( width, *projector, rgb );
  imwrite( dstfilename, dst );
  //cvReleaseImage (&dst);
}
