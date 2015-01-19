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

libtableedit.a:
	cd tableedit; make

run: all
	./$(TARGET)

deploy: all
	cp ./$(TARGET) $(DEPLOY_DIR)
	cp ./$(TABLEEDIT)/js/* $(DEPLOY_DIR)../www
	cp ./$(TABLEEDIT)/images/* $(DEPLOY_DIR)../www/images/

onlydeploy:
	cp ./$(TARGET) $(DEPLOY_DIR)
	cp ./$(TABLEEDIT)/js/* $(DEPLOY_DIR)../www
	cp ./$(TABLEEDIT)/images/* $(DEPLOY_DIR)../www/images/

	
	
