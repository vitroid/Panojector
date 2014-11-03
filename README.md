#Panojector
Panojector command accepts a few options.

###Usage:
 `panojector [-s size][-o outfile][list of plugins.....] load filename`

###Options:

`	-s n	   Set image size to n x n.`

`      -o file  Specify the output file name.`
#Notes
Plugin describes how to project the `(-1..+1)x(-1..+1)` plane onto another `(-1..+1)x(-1..+1)` plane. You can apply these projection plugins to the original image by giving plugin(s) as an argument(s) of the panojector command.

For example,
`prompt% panojector -s 400 mercator load original.jpg`
converts the equirectangular image into mercator projection and output as a 400x400 image.
`prompt% panojector -s 600 box equirectangular load original.jpg`
converts an Mercator panorama into equirectangular panorama and then develops as a box.

When multiple plugins are given, they work as a pipeline.  Each plugin accepts options as its argument. Note that x coordinate points to the right, while y coordinate directs downwards. For complex coordinate, real points to the right, and imaginary directs downwards.
