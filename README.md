#Panojector Manual
Panojector is a modular panorama image converter.
##Plugin specs
<table>
<tr>
<th></th>
<th>explanation</th>
<th>input</th>
<th>output</th>
<th colspan="2">options / argument</th>
</tr>

<tr>
<td rowspan="2">box</td>
<td rowspan="2">Develop a box</td>
<td rowspan="2">equirectangular</td>
<td rowspan="2">plain image</td>
<td>-b x,y,z</td>
<td>aspect ratio of the box</td>
</tr>
<tr>
<td>-e x,y,z</td>
<td>eye position</td>
</tr>

<tr>
<td rowspan="2">cone</td>
<td rowspan="2">Develop a cone</td>
<td rowspan="2">equirectangular</td>
<td rowspan="2">plain image</td>
<td>-r 0.3</td>
<td>radius of the base</td>
</tr>
<tr>
<td>-e 0.2</td>
<td>eye height to cone height</td>
</tr>

<tr>
<td>equirectangular</td>
<td>Mercator to equirect</td>
<td>Mercator</td>
<td>equirectangular</td>
<td colspan="2">No option</td>
</tr>

<tr>
<td>exponential</td>
<td>exponential</td>
<td>complex (plain image)</td>
<td>complex (plain image)</td>
<td colspan="2">No option</td>
</tr>

<tr>
<td>invert</td>
<td>invert</td>
<td>complex (plain image)</td>
<td>complex (plain image)</td>
<td colspan="2">No option</td>
</tr>

<tr>
<td>log</td>
<td>log</td>
<td>complex (plain image)</td>
<td>complex (plain image)</td>
<td colspan="2">No option</td>
</tr>

<tr>
<td>mercator</td>
<td>Equirect to Mercator</td>
<td>equirectangular</td>
<td>Mercator</td>
<td colspan="2">No option</td>
</tr>

<tr>
<td>power</td>
<td>power</td>
<td>complex (plain image)</td>
<td>complex (plain image)</td>
<td>-n 2</td>
<td>Power of n</td>
</tr>

<tr>
<td rowspan="5">prism</td>
<td rowspan="5">Develop a prism</td>
<td rowspan="5">equirectangular</td>
<td rowspan="5">plain image</td>
<td>-e 0.5</td>
<td>Eye height.</td>
</tr>
<tr>
<td>-h 0.5</td>
<td>Height of the prism.</td>
</tr>
<tr>
<td>-s 6</td>
<td>Number of sides.</td>
</tr>
<tr>
<td>-z 2</td>
<td>Specify where to attach the zenith cap.</td>
</tr>
<tr>
<td>-n 2</td>
<td>Specify where to attach the nadir cap.</td>
</tr>

<tr>
<td>ribbon</td>
<td>Convert a long image to a ribbon</td>
<td>plain image</td>
<td>plain image</td>
<td>-a 0.5 or -a 200/500</td>
<td>Aspect ratio of the input image.</td>
</tr>

<tr>
<td>rotate</td>
<td>Rotate an image</td>
<td>plain image</td>
<td>plain image</td>
<td>-a 0</td>
<td>Specify angle in degree.</td>
</tr>

<tr>
<td rowspan="4">scale</td>
<td rowspan="4">Scale an image</td>
<td rowspan="4">plain image</td>
<td rowspan="4">plain image</td>
<td>-x 1</td>
<td rowspan="4">Specify amounts</td>
</tr>
<tr>
<td>-y 1</td>
</tr>
<tr>
<td>-xy 1</td>
</tr>
<tr>
<td>-p</td>
</tr>

<tr>
<td rowspan="2">slide</td>
<td rowspan="2">Slide an image</td>
<td rowspan="2">plain image</td>
<td rowspan="2">plain image</td>
<td>-x 0</td>
<td rowspan="2">Specify amounts (in image coordinate)</td>
</tr>
<tr>
<td>-y 0</td>
</tr>

<tr>
<td>stereographic</td>
<td>Equirect to stereographic</td>
<td>equirectangular</td>
<td>plain image</td>
<td>-a 90</td>
<td>Field of view.</td>
</tr>

<tr>
<td>swap</td>
<td>Swap xyz axes</td>
<td>equirectangular</td>
<td>equirectangular</td>
<td>-n 1</td>
<td>Swap xyz axes multiple times.</td>
</tr>

<tr>
<td rowspan="2">tile</td>
<td rowspan="2">Slanted tiling of a long image. (incommensurate)</td>
<td rowspan="2">plain image</td>
<td rowspan="2">plain image (Mercator)</td>
<td>-a 0.5 or -a 200/500</td>
<td>Aspect ratio of the input image.</td>
</tr>
<tr>
<td>-s 8</td>
<td>Number of stories</td>
</tr>

<tr>
<td>tile2</td>
<td>Slanted tiling of a long image. (commensurate)</td>
<td>plain image</td>
<td>plain image (Mercator)</td>
<td>-a 0.5 or -a 200/500</td><td>Aspect ratio of the input image.</td>
</tr>

<tr>
<td>tilt</td>
<td>Tilt an equirectangular image around the x axis.</td>
<td>equirectangular</td>
<td>equirectangular</td>
<td>-a 0</td>
<td>Specify angle in degree.</td>
</tr>

<tr>
<td>tumblerfan</td>
<td>Create your own tumbler</td>
<td>Mercator</td>
<td>plain image</td>
<td>-s wtop,wbot,height</td>
<td>Size of the fan. Wtop and wbot specifies the top and bottom width of the developed fan, and height specifies the fan height.</td>
</tr>

<tr>
<td>load</td>
<td>Load an image file</td>
<td>file</td>
<td>plain image</td>
<td colspan="2">file name</td>
</tr>

<tr>
<td>interpolate</td>
<td>Load an image file</td>
<td>file</td>
<td>plain image</td>
<td colspan="2">file name</td>
</tr>
</table>

##Panojector command
Panojector command accepts a few options.

###Usage:
    panojector [-s size][-o outfile][list of plugins.....] load filename

###Options:

    -s n	   Set image size to n x n.

    -o file  Specify the output file name.
##Notes
Plugin describes how to project the `(-1..+1)x(-1..+1)` plane onto another `(-1..+1)x(-1..+1)` plane. You can apply these projection plugins to the original image by giving plugin(s) as an argument(s) of the panojector command.

For example,

    ./panojector -s 400 mercator load original.jpg

converts the equirectangular image into mercator projection and output as a 400x400 image.

    ./panojector -s 600 box equirectangular load original.jpg

converts an Mercator panorama into equirectangular panorama and then develops as a box.

When multiple plugins are given, they work as a pipeline.  Each plugin accepts options as its argument. Note that x coordinate points to the right, while y coordinate directs downwards. For complex coordinate, real points to the right, and imaginary directs downwards.

##Other Examples
###Example 1
This converts the sample rectangular image to a swirl:

    ./panojector -s 600 stereographic equirectangular tile2 -a 706/881 load sample.jpg

The first plugin “tile2” tiles the original image, second plugin “equirectangular” regards the image as a Mercator (conformal) panorama and convert it into equirectangular panorama image, and the last plugin “stereographic” converts the equirectangular panorama into stereographic (conformal) image.
![ex1a.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex1a.jpg)

This converts the inverted little planet image back to equirectangular panorama:

    ./panojector -s 1000 slide -x 1.25 equirectangular rotate -a 90 slide -x -0.25 exponential load sample2.jpg

![ex1b.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex1b.jpg)

(The sample image is provided by Pedro Moura Pinheiro at http://www.flickr.com/photos/pedromourapinheiro/4929306871 under Creative Commons (CC BY-NC-SA 2.0) License.)
###Example 2
This converts a long train image to 6-story ribbon:

    ./panojector -s 1000 ribbon -a 826/29999 -s 6 load sample3.jpg

![ex2.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex2.jpg)
###Example 3
This converts the same image into a swirl:

    ./panojector -s 1000 stereographic equirectangular tile -a 826/29999 -s 8 load sample3.jpg

and a tilted swirl:

    ./panojector -s 1000 rotate -a 45 stereographic -a 300 tilt -a 30 equirectangular tile -a 826/29999 -s 8 load sample3.jpg

![ex3a.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex3a.jpg)
![ex3b.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex3b.jpg)
###Example 4
You can even “Create Your Own Starbucks Tumbler.”

    ./panojector -s 1000 tumblerfan -s 214,173,163 tile -a 826/29999 -s 8 load sample3.jpg

![ex4.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex4.jpg)
###Example 5
A small utility `boxsize.py` calculates the proper dimension of the box for your equirectangular panorama image. Here is an example image of a Japanese room. (The sample image is provided in a courtesy of Simon Sherwin. Original photo is available at [Flickr](https://www.flickr.com/photos/simons/4524005292).  Please download the [original image](https://farm5.staticflickr.com/4051/4524005292_5c0c99cffa_o.jpg) by yourself and rename it as `sample4.jpg`.)

1. Measure the dimensions of the images: picture width, positions of four corners, and ceiling and floor positions at the first corner. 

    ![ex5a.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex5a.jpg)
2. Put them as arguments of the boxsize.py program and run it. The program estimates the eye position in the box.

        ./boxsize.py 4000 538 1432 2541 3445 772 1247

3. You get the optimal parameters to convert the image into a box development. Modify as you like, and run it.

        ./panojector -s 2000 box -b 1,1.1724609375,0.609415798286 -e 0.5064453125,0.504081292687,0.521980455925 slide -x 0.269 load sample4.jpg

    ![ex5b.jpg](https://raw.githubusercontent.com/vitroid/Panojector/master/Samples/ex5b.jpg)

For now, we provide only one rotation plugin, “tilt”, to give a rotation angle around the X axis to an equirectangular image.  In order to rotate the equirectangular image with YAW, PITCH, and ROLL angles in  PTgui and Hugin way, use the following parameters: (YAW, PITCH, and ROLL should be replaced by the values.)

    ./panojector -s 1000 swap tilt -a -YAW swap tilt -a PITCH swap tilt -a ROLL load sample4.jpg
