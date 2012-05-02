UNAME := $(shell uname)
ifeq "Darwin" "$(UNAME)"
LDFLAGS=-L/usr/local/Cellar/opencv/2.2/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_imgproc 
endif
ifeq "Linux" "$(UNAME)"
LDFLAGS=-lcv -lhighgui
endif
CXX=g++ -O6 #-Isrc #-g
TARGETS=panojector
all: $(TARGETS)
clean:
	-rm $(TARGETS) *.o *~ *.so
memo:
	#svn copy https://trainscanner.svn.sourceforge.net/svnroot/trainscanner https://trainscanner.svn.sourceforge.net/svnroot/trainscanner/beta0.01 -m "Tagging the Beta 0.01 release of the trainscanner project."
#./tumblerfan -s 3000 -t 2440 -b 1890 -h 1430 -o -0.5  for sugiyama tumblers

SO=terminate.so slide.so box.so scale.so invert.so exponential.so power.so ribbon.so log.so mercator.so tile.so tile2.so equirectangular.so rotate.so tilt.so tumblerfan.so stereographic.so prism.so cone.so swap.so
OBJS=$(patsubst %.so, %.o, $(SO))
panojector: panojector.o plugin.o $(SO) $(OBJS)
	$(CXX) $(CXXFLAGS)  panojector.o plugin.o -o $@ $(LDFLAGS)
%.so: %.o
	$(CXX) -shared $< plugin.o -o $@
