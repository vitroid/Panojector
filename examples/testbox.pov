//make a textured box from the ER image

#declare BOX=<1,2.3333,3.66666>;
#declare EYE=<0.3333,0.4286,0.4545>;


camera {
    location BOX/2 + <30, 30, 30>
    look_at BOX/2
    angle 8
}

box { -EYE*BOX (1-EYE)*BOX
  texture{
    pigment {
      image_map {
        jpeg "testbox.inv.er.jpg"
	map_type 1 //spherical
      }
      rotate <-90,0,90>
    }
    finish {ambient 1}
  }
}

