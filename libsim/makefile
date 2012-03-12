
.PHONY : libsim install
CC = gcc
CFLAGS = -Wall -O2

AR = ar
ARFLAGS = -v -u -r
OBJECTS = operator.o bus.o trace.o vcd.o
LIBS = 

TARGET = libsim.a

libsim : $(TARGET)

$(TARGET) : $(OBJECTS)
	 $(AR) $(ARFLAGS) $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

include makefile.config


install :
	cp $(TARGET) /usr/local/lib/libsim.a
	mkdir --parents /usr/local/include/libsim
	cp bus.h /usr/local/include/libsim/bus.h
	cp operator.h /usr/local/include/libsim/operator.h
	cp sim.h /usr/local/include/libsim/sim.h
	cp simtype.h /usr/local/include/libsim/simtype.h
	cp trace.h /usr/local/include/libsim/trace.h
	cp vcd.h /usr/local/include/libsim/vcd.h
