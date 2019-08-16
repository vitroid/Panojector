UNAME := $(shell uname)
ifeq "Darwin" "$(UNAME)"
LDFLAGS=`pkg-config opencv4 --libs`
endif
ifeq "Linux" "$(UNAME)"
LDFLAGS=-lcv -lhighgui
endif
CXX=g++ -O3 -Isrc `pkg-config opencv4 --cflags` -std=c++11
SO=load.so rotate.so userfunc.so scale.so log.so invert.so slide.so exponential.so tile.so mer2equ.so equ2ste.so # grid4.so saru.so box.so power.so ribbon.so equ2mer.so tile2.so mer2equ.so tilt.so tumblerfan.so equ2ste.so prism.so cone.so swap.so interpolate.so er_ripple.so cyl2equ.so equ2cyl.so
TARGETS=panojector $(SO)
all: $(TARGETS)
clean:
	-rm $(TARGETS) *.o *~
memo:
	#svn copy https://trainscanner.svn.sourceforge.net/svnroot/trainscanner https://trainscanner.svn.sourceforge.net/svnroot/trainscanner/beta0.01 -m "Tagging the Beta 0.01 release of the trainscanner project."
#./tumblerfan -s 3000 -t 2440 -b 1890 -h 1430 -o -0.5  for sugiyama tumblers
#Crop 2:1 image from a square one.
%.er.jpg: %.jpg %.jpg.dimen
	. $<.dimen; newh=`expr $$wid / 2`; yofs=`expr \( $$hei - $$newh \) / 2`; convert '$<' -crop $${wid}x$${newh}+0+$${yofs} '$@'
%.jpg.dimen: %.jpg
	( sips -g pixelHeight -g pixelWidth pano.jpg | awk '/Height/{h=$$2}/Width/{w=$$2}END{print "wid="w;print "hei="h}' || convert '$<' -format "wid=%[w]\\nhei=%[h]" info: ) > $@
%.o: src/%.cpp
	$(CXX) -c $< -o $@
panojector: panojector.o plugin.o
	$(CXX) $(CXXFLAGS)  panojector.o plugin.o -o $@ $(LDFLAGS)
%.so: %.o
	$(CXX) -shared $< plugin.o -o $@ $(LDFLAGS)
