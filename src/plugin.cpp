#include <dlfcn.h>
#include <iostream>
#include "plugin.hpp"
using namespace std;

Projector* plugin_load(int argc, char* argv[]) {
  using std::cout;
  using std::cerr;
  
  // load the projector library
  char plugin_name[1000];
  if ( argc == 0 ){
    cerr << "Open chain." << endl;
    return NULL;
  }
  else{
    sprintf(plugin_name, "./%s.so", argv[0]);
  }
  void* projector = dlopen(plugin_name, RTLD_LAZY);
  if (!projector) {
    cerr << "Cannot load library: " << dlerror() << endl;
    return NULL;
  }
  
  // load the symbols
  create_t* create_projector = (create_t*) dlsym(projector, "create");
  destroy_t* destroy_projector = (destroy_t*) dlsym(projector, "destroy");
  if (!create_projector || !destroy_projector) {
    cerr << "Cannot load symbols: " << dlerror() << '\n';
    return NULL;
  }
  
  // create an instance of the class
  return create_projector(argc, argv);
}


/*
//Same coordinate as the screen. Origin is at the corner.
void getpixel( IplImage* const src, float x,float y, IplImage* dst, int dx, int dy )
{
  int sx = x * (float)src->width / 2.0 + src->width / 2.0;
  int sy = y * (float)src->width / 2.0 + src->height / 2.0;
  //cout << sx <<":"<<sy <<"/"<<dx<<":"<<dy<<endl;
  sx %= src->width;
  if ( sx < 0 )
    sx += src->width;
  sy %= src->height;
  if ( sy < 0 )
    sy += src->height;
  if ( //sx >= 0 && sx< src->width  &&
       //sy >= 0 && sy< src->height &&
       dx >= 0 && dx< dst->width  &&
       dy >= 0 && dy< dst->height ){
    for ( int ch=0; ch<3; ch++ ){
      dst->imageData[dst->widthStep * dy + dx * 3 + ch] = src->imageData[src->widthStep * sy + sx * 3 + ch];
    }
  }
}
*/

 /*
//Same coordinate as the screen. Origin is at the corner.
void white( IplImage* dst, int dx, int dy )
{
  if ( dx >= 0 && dx< dst->width  &&
       dy >= 0 && dy< dst->height ){
    for ( int ch=0; ch<3; ch++ ){
      dst->imageData[dst->widthStep * dy + dx * 3 + ch] = 255;
    }
  }
}
 */
 




void spherical2cartesian(float phi, float theta, float& x, float& y, float& z)
{
  x = cos(phi)*cos(theta);
  y = sin(phi)*cos(theta);
  z =          sin(theta);
}


void cartesian2spherical(float x, float y, float z, float& phi, float& theta)
{
  theta = asin(z);
  float costh = sqrt(1-z*z);
  phi   = asin(y/costh);
  if (x < 0 ){
    phi = M_PI-phi;
  }
}
