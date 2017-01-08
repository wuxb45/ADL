#!/bin/make
# build requirements:
# 'lex' or 'flex'
# 'yacc' or 'bison'
#

CC = gcc

MYOPTION = -Wall -g
#MYOPTION = -Wall -O2

LEX = flex
YACC = bison

FLEXOPTION = --yylineno
BISONOPTION = --debug --verbose

OBJECTS = main.o stage.o util.o lex.yy.o front.tab.o parse.o config.o pre.o common.o line.o module.o global.o code.o
#OBJECTS = main.o stage.o util.o lex.yy.o front.tab.o parse.o common.o module.o global.o code.o graph.o config.o verilog.o pre.o
LIBS = -lfl
TARGET = adlforsim
.PHONY : all clean indent wc test install install_lib



all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(MYOPTION) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(MYOPTION) -c $<

include Makefile.config

lex.yy.c : front.l front.tab.h
	$(LEX) $(FLEXOPTION) front.l
front.tab.h : front.y
	$(YACC) $(BISONOPTION) -d front.y
front.tab.c : front.y parse.h
	$(YACC) $(BISONOPTION) -d front.y

clean:
	rm -f  lex.yy.c front.tab.c front.tab.h front.output *~
	rm -f $(OBJECTS) $(TARGET)

indent:
	indent -kr -i4 -ts4 -sob -l90 -ss -bs -psl *.c
wc:
	wc -l *.c *.h *.l *.y
install:
	cp $(TARGET) /usr/local/bin/adlforsim
