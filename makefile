incdir := ./include
srcdir := ./src
gainmatchsrcdir := $(srcdir)/fitter
calibratesrcdir := $(srcdir)/calibrate
objdir := ./objs
libdir := ./lib
bindir := ./bin

correlappname := $(bindir)/findCorrelations
gainmatchapp := $(bindir)/findGainMatchFactors
calibrateapp := $(bindir)/calibrator

CXX := g++
CXXFLAGS := -std=c++11 -O3 -fPIC -g $(shell root-config --cflags)
CPPFLAGS := -I./ -I$(incdir) -I$(shell root-config --incdir)
LDFLAGS := $(shell root-config --libs)

srcfiles := $(shell find $(srcdir) -name "*.cpp" | grep -v $(gainmatchsrcdir) | grep -v $(calibratesrcdir))
objects := $(patsubst %.cpp, %.o, $(srcfiles))
gainmatchsrc := $(shell find $(gainmatchsrcdir) -name "*.cpp")
gainmatchobj := $(patsubst %.cpp, %.o, $(gainmatchsrc))

calibratesrc := $(shell find $(calibratesrcdir) -name "*.cpp")
calibrateobj := $(patsubst %.cpp, %.o, $(calibratesrc))

dict := $(srcdir)/anasen_dict.cxx
dict_inputs := $(incdir)/DataStructs.h $(incdir)/GainMap.h $(incdir)/ChannelMap.h $(incdir)/LinkDef_evb.h
dictlib := $(libdir)/libEVBDict
dictobj := $(objdir)/anasen_dict.o

all: $(correlappname) $(gainmatchapp) $(calibrateapp)

$(correlappname): $(dictobj) $(objects)
	$(CXX) -o $@ $^ $(LDFLAGS)
	cp $(srcdir)/*.pcm $(bindir)

$(gainmatchapp): $(gainmatchobj) $(srcdir)/ChannelMap.o
	$(CXX) -o $@ $^ $(LDFLAGS)
$(gainmatchobj): $(gainmatchsrc)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^

$(calibrateapp): $(dictobj) $(calibrateobj) $(srcdir)/GainMap.o $(srcdir)/ChannelMap.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	cp $(srcdir)/*.pcm $(bindir)

$(calibrateobj): $(calibratesrc)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^

$(dictobj): $(dict)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^
	$(CXX) $(CXXFLAGS) $@ -shared -o $(dictlib).so $(LDFLAGS)

$(dict) : $(dict_inputs)
	rootcint -f $(dict) $(dict_inputs)


#$(dict2obj): $(dict2)
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^
#	$(CXX) $(CXXFLAGS) $@ -shared -o $(dict2lib).so $(LDFLAGS)

#$(dict2) : $(dict2_inputs)
#	rootcint -f $(dict2) $(dict2_inputs)

#$(dict3obj): $(dict3)
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $^
#	$(CXX) $(CXXFLAGS) $@ -shared -o $(dict3lib).so $(LDFLAGS)

#$(dict3) : $(dict3_inputs)
#	rootcint -f $(dict3) $(dict3_inputs)


clean:
	rm -f $(srcdir)/*.o $(objdir)/*.o ./bin/* $(srcdir)/*.cxx $(srcdir)/fitter/*.o $(srcdir)/calibrate/*.o
