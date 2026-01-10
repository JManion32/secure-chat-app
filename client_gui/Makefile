CXX = g++
QT = /home/jmanion32/Qt/6.10.1/gcc_64

CXXFLAGS = -std=c++17 -Wall \
    -Iinclude -I../shared \
    -I$(QT)/include \
    -I$(QT)/include/QtCore \
    -I$(QT)/include/QtGui \
    -I$(QT)/include/QtWidgets \
    -I$(QT)/include/QtNetwork

LDFLAGS = -L$(QT)/lib -lQt6Core -lQt6Gui -lQt6Widgets -lQt6Network -pthread

OBJ = \
    src/main.o \
    src/client.o \
    src/themes.o \
    ../shared/src/socket_handler.o \
    ../shared/src/thread_handler.o

../bin/linux/client: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../shared/src/%.o: ../shared/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o ../shared/src/*.o ../bin/linux/client
