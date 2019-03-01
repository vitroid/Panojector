all: $(TARGETS)
clean:
	-rm $(TARGETS) *.o *~
%.er.jpg: %.jpg
	wid=`convert '$<' -format "%[w]" info:`; hei=`convert '$<' -format "%[h]" info:`; newh=`expr $$wid / 2`; yofs=`expr \( $$hei - $$newh \) / 2`; convert '$<' -crop $${wid}x$${newh}+0+$${yofs} '$@'
sample4.inv.jpg: sample4.jpg
	./panojector -s 4000 -o invert.jpg roomextender -i -b 1,1.1724609375,0.609415798286 -e 0.5064453125,0.504081292687,0.478019544075 slide -x 1.49451942877 interpolate sample4.jpg
testbox.inv.jpg:
	./panojector -s 1280 -o testbox.inv.jpg roomextender -i -b 1,2.3333,3.66666 -e 0.3333,0.4286,0.4545 slide -x 1.14603608768 interpolate testbox.png
