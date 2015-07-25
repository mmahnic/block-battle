.PHONY: builddir bot debug clean loadtest zip

CXX=g++
CXXFLAGS=-std=c++14
OUTDIR=./build

bot: builddir $(OUTDIR)/blockbattle

debug: CXXFLAGS += -ggdb -DDEBUG -DDEBUG_INTRFC
debug: bot

builddir: $(OUTDIR)
$(OUTDIR):
	@if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi

$(OUTDIR)/blockbattle: $(OUTDIR)/blockbattle.o $(OUTDIR)/myai.o
	g++ -o $(OUTDIR)/blockbattle $(OUTDIR)/blockbattle.o $(OUTDIR)/myai.o

$(OUTDIR)/blockbattle.o: blockbattle.cpp
	$(CXX) $(CXXFLAGS) -c blockbattle.cpp -o $@

$(OUTDIR)/myai.o: myai.cpp
	$(CXX) $(CXXFLAGS) -c myai.cpp -o $@

clean:
	@if [ -d $(OUTDIR) ]; then rm $(OUTDIR)/*.o; rm $(OUTDIR)/blockbattle; fi

loadtest: bot
	$(OUTDIR)/blockbattle < test/test.txt

ZIPFILES= \
	  blockbattle.cpp \
	  myai.cpp \
	  defines.h \
	  dumps.h \
	  game.h \
	  inputhandler.h \
	  myai.h \
	  parsers.h

zip:
	@if [ ! -d xdata ]; then mkdir -p xdata; fi
	zip xdata/bot.zip $(ZIPFILES)

