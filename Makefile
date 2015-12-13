UNAME := $(shell uname)
ifeq "Darwin" "$(UNAME)"
LDFLAGS=-L/usr/local/Cellar/opencv/2.4.12/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_imgproc
endif
ifeq "Linux" "$(UNAME)"
LDFLAGS=-lcv -lhighgui
endif
CXX=g++ -O3 -Isrc -I/usr/local/Cellar/opencv/2.4.12/include #-g
SO=grid4.so saru.so load.so slide.so box.so scale.so invert.so exponential.so power.so ribbon.so log.so mercator.so tile.so tile2.so equirectangular.so rotate.so tilt.so tumblerfan.so stereographic.so prism.so cone.so swap.so interpolate.so
TARGETS=panojector $(SO)
all: $(TARGETS)
clean:
	-rm $(TARGETS) *.o *~
memo:
	#svn copy https://trainscanner.svn.sourceforge.net/svnroot/trainscanner https://trainscanner.svn.sourceforge.net/svnroot/trainscanner/beta0.01 -m "Tagging the Beta 0.01 release of the trainscanner project."
#./tumblerfan -s 3000 -t 2440 -b 1890 -h 1430 -o -0.5  for sugiyama tumblers
%.o: src/%.cpp
	$(CXX) -c $< -o $@
panojector: panojector.o plugin.o
	$(CXX) $(CXXFLAGS)  panojector.o plugin.o -o $@ $(LDFLAGS)
%.so: %.o
	$(CXX) -shared $< plugin.o -o $@ $(LDFLAGS)
