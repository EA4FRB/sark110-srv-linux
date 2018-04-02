#SARK-110 Basic Linux client

#Object files
OBJ = main.o sark_rem_client.o hid.o

#Header files
DEPS = sark_cmd_defs.h sark_rem_client.h hidapi.h

#Libraries
LIBS = -ludev

#Compiler flags
CFLAGS = -lrt

#Set the compiler you are using ( gcc for C or g++ for C++ )
CC = gcc

#Set the filename extension of your C files (e.g. .c or .cpp )
EXTENSION = .c

#define a rule that applies to all files ending in the .o suffix, which says that the .o file depends upon the .c version of the file and all the .h files included in the DEPS macro.  Compile each object file
%.o: %$(EXTENSION) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Combine them into the output file
#Set your desired exe output file name here
sark110-srv.a: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Cleanup
.PHONY: clean

clean:
	rm -f *.o *~ core *~
