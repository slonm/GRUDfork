CC=g++
INCLUDES=-I./tableedit/include/
LIBDIRS=-L./tableedit/lib -L/usr/pgsql-9.3/lib/ 
LIBS=-ltableedit -lpqxx -lpq -lcgicc
DEPLOY_DIR=/var/www/cgi-bin/
TARGET=test
TABLEEDIT=./tableedit

all: $(TARGET).cpp

test.cpp: libtableedit.a
	$(CC) -c test.cpp $(INCLUDES)
	$(CC) $(TARGET).o -o ./$(TARGET) $(LIBDIRS) $(LIBS)

clean:
	rm -f ./$(TARGET)
	rm -f ./$(TARGET).o
	cd tableedit; make clean

libtableedit.a:
	cd tableedit; make

run: all
	./$(TARGET)

deploy: all
	cp ./$(TARGET) $(DEPLOY_DIR)
	cp -r ./$(TABLEEDIT)/js/* $(DEPLOY_DIR)../html
	cp ./$(TABLEEDIT)/images/* $(DEPLOY_DIR)../html/images/

onlydeploy:
	cp ./$(TARGET) $(DEPLOY_DIR)
	cp -r ./$(TABLEEDIT)/js/* $(DEPLOY_DIR)../html
	cp ./$(TABLEEDIT)/images/* $(DEPLOY_DIR)../html/images/

	
	
