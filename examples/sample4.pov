//make a textured box from the ER image

#declare BOX=<1,1.1724609375,0.609415798286>;
#declare EYE=<0.5064453125,0.504081292687,0.478019544075>;


camera {
    spherical
    location  0 //BOX/2 + <10, 10, 10>
    look_at y // BOX/2
    right x
    sky -z
    //angle 8
}

#declare room=object{
box { -EYE*BOX (1-EYE)*BOX
  texture{
    pigment {
      image_map {
        jpeg "sample4.inv.er.jpg"
	map_type 1 //spherical
      }
      rotate <-90,0,90>
    }
    finish {ambient 1}
  }
}};

object {room 
  translate BOX*x
}
object {room 
  translate BOX*y*2
}
object {room 
  translate BOX*y-BOX*z
}
object {room
  translate BOX*x+BOX*y
}
object {room
  translate BOX*x-BOX*z
}
object {room
  translate BOX*-z*2
}