# RF_RandomForest - Makefile
# Author: Martin Simon <martiinsiimon@gmail.com>

SRC=src/
DOC=doc/
UTILS=utils/

SRCFILES=$(SRC)*.cpp $(SRC)*.h $(SRC)Makefile
UTILSFILES=$(UTILS)*.cpp $(UTILS)*.h $(UTILS)Makefile

ARCHIVE=xsimon14.zip
PDFFILE=$(DOC)pov-semantic-segmentation.pdf

RM=rm -rf
AR=zip


.PHONY: all src doc

all: src doc utils

src:
	make -C $(SRC)

doc:
	make -C $(DOC)

utils:
	make -C $(UTILS)

run:
	make -C $(UTILS) run

clean: clean-src clean-doc
	$(RM) $(ARCHIVE)

clean-src:
	make -C $(SRC) clean

clean-doc:
	make -C $(DOC) clean

clean-utils:
	make -C $(UTILS) clean

pack: doc src utils clean
	$(AR) $(ARCHIVE) Makefile README INSTALL LICENSE $(SRCFILES) $(UTILSFILES) $(PDFFILE)
