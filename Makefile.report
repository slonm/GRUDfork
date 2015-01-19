CC=g++
INCLUDES=-I./tableedit/include -I.
LIBDIRS=-L./tableedit/lib
LIBS=-lpqxx -lcgicc
DEPLOY_DIR=/var/www/egor10257.rvncerr.org/cgi-bin/
TARGET=rtest

all:
	$(CC) -c $(TARGET).cpp report.cpp $(INCLUDES)
	$(CC) $(TARGET).o report.o -o ./$(TARGET) $(LIBS)

clean:
	rm -f ./$(TARGET)
	rm -f ./*.o

run: all
	./$(TARGET)

deploy: all
	cp ./$(TARGET) $(DEPLOY_DIR)
	cp ./report.js $(DEPLOY_DIR)../www

	
	
