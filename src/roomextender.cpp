#include "plugin.hpp"
#include <iostream> //for debug
class Box2Er : public Projector {
protected:
  float boxx,boxy,boxz,eyex,eyey,eyez;
  int repx, repy, repz;
  int inverse;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-e x,y,z][-b x,y,z][-r x,y,z] [projectors]\n", argv[0] );
    fprintf( stderr, "Extend the equirectangular 2:1 room image, then restore the equirectangular again.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-e 0.5,0.5,0.5\tSpecify eye point (center = 0.5).\n" ); 
    fprintf( stderr, "\t-b 1,1,1\tSpecify aspect ratio of the box.\n" ); 
    fprintf( stderr, "\t-r 1,1,1\tRepeat the box.\n" ); 
    exit(1);
  }
  Box2Er(int argc, char* argv[])
  {
    eyex = eyey = eyez = 0.5;//eyez==0: on the floor
    boxx = boxy = boxz = 1;
    repx = 1;
    repy = 1;
    repz = 1;
    inverse = 0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-b" )){
	c++;
	sscanf(argv[c], "%f,%f,%f", &boxx, &boxy, &boxz );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-i" )){
        inverse += 1;
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-e" )){
	c++;
	sscanf(argv[c], "%f,%f,%f", &eyex, &eyey, &eyez );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-r" )){
	c++;
	sscanf(argv[c], "%d,%d,%d", &repx, &repy, &repz );
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
  uchar* map(float dst2x, float dst2y)
  {
    //dst2x : -1 .. +1
    //dst2y : -0.5 .. +0.5;
    //to direction vector
    float theta = dst2y * M_PI;
    float phi   = dst2x * M_PI-M_PI/2;
    //room corner is on the edge of ER image
    //float phi0 = atan2(boxy*eyey, boxx*eyex);// - M_PI;
    //phi += phi0;
    //unit look vector
    float tx,ty,tz,tr;
    tz = sin(theta);
    tr = cos(theta);
    tx = cos(phi) * tr;
    ty = sin(phi) * tr;
    //where the look vector intersects the ceiling.
    float Dz =-boxz*eyez - boxz*(repz-1)/2;
    float Dx = tx/tz*Dz;
    float Dy = ty/tz*Dz;
    //relative vector from the corner of the face
    //max: repx,repy,repz
    float Cx = Dx/boxx + eyex + (repx-1)/2;
    float Cy = Dy/boxy + eyey + (repy-1)/2;
    if ( ( 0 <= Cx ) && ( Cx < repx ) &&
         ( 0 <= Cy ) && ( Cy < repy ) && ( tz < 0 ) ){
      //on the ceiling
      //corresponding location on the unit image
      float dx = (Cx - floor(Cx) - eyex)*boxx;
      float dy = (Cy - floor(Cy) - eyey)*boxy;
      float dz =                  -eyez *boxz;
      if ( inverse )
        dz = boxz - eyez*boxz;
      //float rr = dx*dx+dy*dy+dz*dz;
      //rr = sqrt(rr);
      //printf("%f %f %f  %f %f %f\n", tx,ty,tz,dx/rr,dy/rr,dz/rr);
      //if ( inverse ) dz = boxz - boxz*eyez;
      float dr = sqrt(dx*dx+dy*dy);
      float h  = atan2( dy, dx )+M_PI/2;
      if ( h < -M_PI ) h += 2*M_PI;
      float v  = atan2( dz, dr );
      //printf("%f %f  %f %f\n", theta,phi,v,h);
      return child->map( h / M_PI, v / M_PI );
    }
    else {
      //Project to the extended box
      float Dz = boxz*(1-eyez) + boxz*(repz-1)/2;
      float Dx = tx/tz*Dz;
      float Dy = ty/tz*Dz;
      float Cx = Dx + boxx*eyex + boxx*(repx-1)/2;
      float Cy = Dy + boxy*eyey + boxy*(repy-1)/2;
      float Cz = Dz + boxz*eyez + boxz*(repz-1)/2;
      if ( ( 0 <= Cx ) && ( Cx < boxx*repx ) &&
           ( 0 <= Cy ) && ( Cy < boxy*repy ) && ( tz > 0 ) ){
        //on the floor
        //position on the box is (cx,cy,cz)
        //now reconvert it to the angles
        float dx = Cx - floor(Cx/boxx)*boxx - boxx*eyex;
        float dy = Cy - floor(Cy/boxy)*boxy - boxy*eyey;
        float dz =                     boxz - boxz*eyez;
        if ( inverse )
          dz = - eyez*boxz;
        float dr = sqrt(dx*dx+dy*dy);
        float h = atan2( dy, dx ) + M_PI/2;
        if ( h < -M_PI ) h += 2*M_PI;
        float v = atan2( dz, dr );
        return child->map( h / M_PI, v / M_PI );
      }
      else {
        //side panels
        float Dx = -boxx*eyex - boxx*(repx-1)/2;
        float Dy = ty/tx*Dx;
        float Dz = tz/tx*Dx;
        float Cx = Dx + boxx*eyex + boxx*(repx-1)/2;
        float Cy = Dy + boxy*eyey + boxy*(repy-1)/2;
        float Cz = Dz + boxz*eyez + boxz*(repz-1)/2;
        if ( ( 0 <= Cy ) && ( Cy < boxy*repy ) &&
             ( 0 <= Cz ) && ( Cz < boxz*repz ) && ( tx < 0 ) ){
          //on the wall 1
          //position on the box is (cx,cy,cz)
          //now reconvert it to the angles
          float dx =                          - boxx*eyex;
          float dy = Cy - floor(Cy/boxy)*boxy - boxy*eyey;
          float dz = Cz - floor(Cz/boxz)*boxz - boxz*eyez;
          if ( inverse )
            dx = boxx - eyex*boxx;
          float dr = sqrt(dx*dx+dy*dy);
          float h = atan2( dy, dx ) + M_PI/2;
          if ( h < -M_PI ) h += 2*M_PI;
          float v = atan2( dz, dr );
          return child->map( h / M_PI, v / M_PI );
        }
        else {
          //side panels
          float Dx = boxx-boxx*eyex + boxx*(repx-1)/2;
          float Dy = ty/tx*Dx;
          float Dz = tz/tx*Dx;
          float Cx = Dx + boxx*eyex + boxx*(repx-1)/2;
          float Cy = Dy + boxy*eyey + boxy*(repy-1)/2;
          float Cz = Dz + boxz*eyez + boxz*(repz-1)/2;
          if ( ( 0 <= Cy ) && ( Cy < boxy*repy ) &&
               ( 0 <= Cz ) && ( Cz < boxz*repz ) && ( tx > 0 ) ){
            //on the wall 3
            //position on the box is (cx,cy,cz)
            //now reconvert it to the angles
            float dx =                     boxx - boxx*eyex;
            float dy = Cy - floor(Cy/boxy)*boxy - boxy*eyey;
            float dz = Cz - floor(Cz/boxz)*boxz - boxz*eyez;
            if ( inverse )
              dx = - eyex*boxx;
            float dr = sqrt(dx*dx+dy*dy);
            float h = atan2( dy, dx ) + M_PI/2;
            if ( h < -M_PI ) h += 2*M_PI;
            float v = atan2( dz, dr );
            return child->map( h / M_PI, v / M_PI );
          }
          else {
            //side panels
            float Dy =-boxy*eyey - boxy*(repy-1)/2;
            float Dx = tx/ty*Dy;
            float Dz = tz/ty*Dy;
            float Cx = Dx + boxx*eyex + boxx*(repx-1)/2;
            float Cy = Dy + boxy*eyey + boxy*(repy-1)/2;
            float Cz = Dz + boxz*eyez + boxz*(repz-1)/2;
            if ( ( 0 <= Cx ) && ( Cx < boxx*repx ) &&
                 ( 0 <= Cz ) && ( Cz < boxz*repz ) && ( ty < 0 ) ){
              //on the wall 2
              //position on the box is (cx,cy,cz)
              //now reconvert it to the angles
              float dx = Cx - floor(Cx/boxx)*boxx - boxx*eyex;
              float dy =                          - boxy*eyey;
              float dz = Cz - floor(Cz/boxz)*boxz - boxz*eyez;
              if ( inverse )
                dy = boxy - eyey*boxy;
              float dr = sqrt(dx*dx+dy*dy);
              float h = atan2( dy, dx ) + M_PI/2;
              if ( h < -M_PI ) h += 2*M_PI;
              float v = atan2( dz, dr );
              return child->map( h / M_PI, v / M_PI );
            }
            else {
              //side panels
              float Dy = boxy-boxy*eyey + boxy*(repy-1)/2;
              float Dx = tx/ty*Dy;
              float Dz = tz/ty*Dy;
              float Cx = Dx + boxx*eyex + boxx*(repx-1)/2;
              float Cy = Dy + boxy*eyey + boxy*(repy-1)/2;
              float Cz = Dz + boxz*eyez + boxz*(repz-1)/2;
              if ( ( 0 <= Cx ) && ( Cx < boxx*repx ) &&
                   ( 0 <= Cz ) && ( Cz < boxz*repz ) && ( ty > 0 ) ){
                //on the wall 4
                //position on the box is (cx,cy,cz)
                //now reconvert it to the angles
                float dx = Cx - floor(Cx/boxx)*boxx - boxx*eyex;
                float dy =                     boxy - boxy*eyey;
                float dz = Cz - floor(Cz/boxz)*boxz - boxz*eyez;
                if ( inverse )
                  dy = - eyey*boxy;
                float dr = sqrt(dx*dx+dy*dy);
                float h = atan2( dy, dx ) + M_PI/2;
                if ( h < -M_PI ) h += 2*M_PI;
                float v = atan2( dz, dr );
                return child->map( h / M_PI, v / M_PI );
              }
            }
          }
        }
      }
    }
    return 0;
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Box2Er(argc, argv);
}

extern "C" void destroy(Box2Er* p) {
    delete p;
}
