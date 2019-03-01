//make a coloerd box
//render it with width 1280 and develop it by
//./boxsize.py 1280 28 213 638 784 114 541
/*
     3
*******C*******
*
*    ^
*    |
*    |
*    *
7             R
G
*
*     
*
*
*******w*******





*/
#include "colors.inc"

#declare EYE=<1,3,5>;
#declare BOX=<3,7,11>;

camera {
    spherical
    location EYE
    look_at EYE + <0.3,0.6,0>
    up z
    sky z
}

box { 0 BOX
  texture{
    pigment {
      Red
    }
    finish {ambient 1}
  }
  translate <3,0,0>
}

box { 0 BOX
  texture{
    pigment {
      Green
    }
    finish {ambient 1}
  }
  translate <-3,0,0>
}

box { 0 BOX
  texture{
    pigment {
      White
    }
    finish {ambient 1}
  }
  translate <0,7,0>
}

box { 0 BOX
  texture{
    pigment {
      Cyan
    }
    finish {ambient 1}
  }
  translate <0,-7,0>
}

box { 0 BOX
  texture{
    pigment {
      Blue
    }
    finish {ambient 1}
  }
  translate <0,0,11>
}

box { 0 BOX
  texture{
    pigment {
      Yellow
    }
    finish {ambient 1}
  }
  translate <0,0,-11>
}
