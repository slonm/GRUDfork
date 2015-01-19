CC=g++
INCLUDES=-I./../include
SRCDIR=./src
LIBDIR=./lib
AR=ar


all: searchable.o browsable.o 
	cd $(SRCDIR); ar rcs ../$(LIBDIR)/libtableedit.a simple.o searchable.o browsable.o field.o util.o

searchable.o: simple.o
	cd $(SRCDIR); $(CC) -c searchable.cpp $(INCLUDES)

browsable.o: simple.o
	cd $(SRCDIR); $(CC) -c browsable.cpp $(INCLUDES)

simple.o: searchfield.o util.o
	cd $(SRCDIR); $(CC) -c simple.cpp $(INCLUDES)

searchfield.o: field.o
	cd $(SRCDIR); $(CC) -c searchfield.cpp $(INCLUDES)

field.o:
	cd $(SRCDIR); $(CC) -c field.cpp $(INCLUDES)

util.o:
	cd $(SRCDIR); $(CC) -c util.cpp $(INCLUDES)

clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(LIBDIR)/libtableedit.a
