UNAME := $(shell uname)
ifeq "Darwin" "$(UNAME)"
LDFLAGS=-L/usr/local/Cellar/opencv/2.4.12/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_imgproc #-lopencv_contrib 
endif
ifeq "Linux" "$(UNAME)"
LDFLAGS=-lcv -lhighgui
endif
CXX=g++ -O3 -Isrc -I/usr/local/Cellar/opencv/2.4.12/include #-g
SO=grid4.so saru.so load.so slide.so box.so scale.so invert.so exponential.so power.so ribbon.so log.so mercator.so tile.so tile2.so equirectangular.so rotate.so tilt.so tumblerfan.so stereographic.so prism.so cone.so swap.so interpolate.so er_ripple.so cylindrical_to_equirectangular.so
TARGETS=panojector $(SO)
all: $(TARGETS)
clean:
	-rm $(TARGETS) *.o *~
memo:
	#svn copy https://trainscanner.svn.sourceforge.net/svnroot/trainscanner https://trainscanner.svn.sourceforge.net/svnroot/trainscanner/beta0.01 -m "Tagging the Beta 0.01 release of the trainscanner project."
#./tumblerfan -s 3000 -t 2440 -b 1890 -h 1430 -o -0.5  for sugiyama tumblers
#Crop 2:1 image from a square one.
%.er.jpg: %.jpg
	wid=`convert '$<' -format "%[w]" info:`; hei=`convert '$<' -format "%[h]" info:`; newh=`expr $$wid / 2`; yofs=`expr \( $$hei - $$newh \) / 2`; convert '$<' -crop $${wid}x$${newh}+0+$${yofs} '$@'
%.o: src/%.cpp
	$(CXX) -c $< -o $@
panojector: panojector.o plugin.o
	$(CXX) $(CXXFLAGS)  panojector.o plugin.o -o $@ $(LDFLAGS)
%.so: %.o
	$(CXX) -shared $< plugin.o -o $@ $(LDFLAGS)
