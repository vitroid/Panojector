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

#Other Examples
##Example 1
This converts the sample rectangular image to a swirl:
prompt% ./panojector -s 600 stereographic equirectangular tile2 -a 706/881 load sample.jpg

The first plugin “tile2” tiles the original image, second plugin “equirectangular” regards the image as a Mercator (conformal) panorama and convert it into equirectangular panorama image, and the last plugin “stereographic” converts the equirectangular panorama into stereographic (conformal) image.
This converts the inverted little planet image back to equirectangular panorama:
prompt% ./panojector -s 1000 slide -x 1.25 equirectangular rotate -a 90 slide -x -0.25 exponential load sample2.jpg

##Example 2
This converts a long train image to 6-story ribbon:
prompt% ./panojector -s 1000 ribbon -a 826/29999 -s 6 load sample3.jpg

##Example 3
This converts the same image into a swirl:
prompt% ./panojector -s 1000 stereographic equirectangular tile -a 826/29999 -s 8 load sample3.jpg
and a tilted swirl:
prompt% ./panojector -s 1000 rotate -a 45 stereographic -a 300 tilt -a 30 equirectangular tile -a 826/29999 -s 8 load sample3.jpg

##Example 4
You can even “Create Your Own Starbucks Tumbler.”
prompt% ./panojector -s 1000 tumblerfan -s 214,173,163 tile -a 826/29999 -s 8 load sample3.jpg
##Example 5
A small utility boxsize.py calculates the proper dimension of the box for your equirectangular panorama image. Here is an example image of a Japanese room:
1. Measure the dimensions of the images: picture width, positions of four corners, and ceiling and floor positions at the first corner. 
2. Put them as arguments of the boxsize.py program and run it. The program estimates the eye position in the box.
prompt% ./boxsize.py 4000 538 1432 2541 3445 772 1247
3. You get the optimal parameters to convert the image into a box development. Modify as you like, and run it.
prompt% ./panojector -s 2000 box -b 1,1.1724609375,0.609415798286 -e 0.5064453125,0.504081292687,0.521980455925 slide -x 0.269 load sample4.jpg

Currently, we provide only one rotation plugin, “tilt”, to give a rotation angle around the X axis to an equirectangular image.  In order to rotate the equirectangular image with YAW, PITCH, and ROLL angles in  PTgui and Hugin way, use the following parameters: (YAW, PITCH, and ROLL should be replaced by the values.)
`prompt% ./panojector -s 1000 swap tilt -a -YAW swap tilt -a PITCH swap tilt -a ROLL load sample4.jpg`
