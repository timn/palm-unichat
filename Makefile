#
# File: Makefile
#
# Written by Tim Niemueller <tim@niemueller.de>
# Created on 2002/05/07
#

# ressource path. some people might want to change this to another
# directory than the source files
R = ./

include *.mk

CC = m68k-palmos-gcc
CFLAGS = -O2 -Wall

LANGUAGE ?= german
SHORTLANGUAGE := $(shell grep "$(LANGUAGE)" languages | awk -F : '{ print $$1 }')
PRINTLANGUAGE := $(shell echo "$(LANGUAGE)" | awk '{ print sprintf("%s%s", toupper(substr($$1, 1, 1)), substr($$1, 2)) }')

LANGUAGES := $(shell awk -F \: '{ print $$2 }' languages)
PILRC = pilrc
PFLAGS = -q -I $(R) -L $(LANGUAGE)


PILOTRATE=115200
PILOTXFER=/usr/bin/pilot-xfer

OBJECTS=$(shell ls *.c | sed -e "s/.c$$/.o/")

RSCBINS:=$(shell ls resources/*.rcp.in | sed -e "s/rcp.in/bin/g")
RSCBINSPRINT:=$(shell echo "$(RSCBINS)" | sed -e "s/resources\///g")

$(PROGNAME).prc: $(PROGNAME) $(RSCBINS) $(PROGNAME).mk
	@echo -e "\n\n================================================================================\nBUILDING:\n"
	build-prc $(PROGNAME).prc "$(PROGDESC)" $(APPID) $(PROGNAME) *.bin
	@echo -e "\n================================================================================\n"
	@echo "RSC BINS: $(RSCBINSPRINT)"
	@echo "OBJECTS : $(OBJECTS)"
	@echo "COMPILER: $(CC) $(CFLAGS)"
	@echo "LANGUAGE: $(PRINTLANGUAGE) [$(SHORTLANGUAGE):$(LANGUAGE)]"
	@ls -l $(PROGNAME).prc | awk '{ print sprintf("PRC size: %s", $$5) }'
	@echo -e "\n================================================================================\n\n"

all: clean $(PROGNAME).prc

$(PROGNAME): $(OBJECTS)


%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.bin: %.rcp $(PROGNAME).h
	$(PILRC) $(PFLAGS) $<

%.rcp: %.rcp.in
	sed -e 's/##VERSION##/$(VERSION)/g' -e 's/##LANGUAGE##/$(PRINTLANGUAGE)/' \
        -e '/##TRANSLATION##/{' \
	-e 'r resources/translation.$(SHORTLANGUAGE)' -e 's/##TRANSLATION##//' \
	-e '}' \
        -e 's/##APPID##/$(APPID)/g' -e 's/##APPNAME##/$(PROGDESC)/g' \
        < $< > $@

install: clean all
	@echo -e "No going to install $(PROGDESC) on PalmOS device\n"
	@echo -e "If you have a USB device press NOW the HotSync Button"
	@echo -e "and THEN press enter to continue.\n"
	@read foo </dev/tty
	PILOTRATE=$(PILOTRATE) $(PILOTXFER) -i $(PROGNAME).prc

clean:
	-rm $(PROGNAME) *.o *.bin

dist:
	mkdir -p $(PROGNAME)-$(VERSION)_dist
	for L in $(LANGUAGES); do LANGUAGE=$$L $(MAKE) package; done
	mv -f $(PROGNAME)-$(VERSION)-* $(PROGNAME)-$(VERSION)_dist

package: clean all
	@echo -e "\n\nPackaging $(PROGNAME) for language $(LANGUAGE) ($(SHORTLANGUAGE))\n\n"

	mkdir -p $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)
	cp -ar docs $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)
	cp $(PROGNAME).prc $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)
	tar cvfz $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE).tar.gz $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)
	zip -r $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE).zip $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)
	rm -rf $(PROGNAME)-$(VERSION)-$(SHORTLANGUAGE)

upload: dist
	echo -e "<?php\n$$version=\"$(VERSION)\";\n?>" > webpage/version.inc.php
	scp -r $(PROGNAME)-$(VERSION)_dist $(SSH_USER)@$(SSH_HOST):$(SSH_PATH)
	scp -r webpage/*.inc.php $(SSH_USER)@$(SSH_HOST):$(SSH_PATH)
	rm webpage/version.inc.php

