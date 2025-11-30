CXX = g++
QT = /home/jmanion32/Qt/6.10.1/gcc_64

CXXFLAGS = -std=c++17 -Wall \
    -Iinclude -I../common \
    -I$(QT)/include -I$(QT)/include/QtCore -I$(QT)/include/QtGui -I$(QT)/include/QtWidgets -I$(QT)/include/QtNetwork

LDFLAGS = -L$(QT)/lib -lQt6Core -lQt6Gui -lQt6Widgets -lQt6Network

OBJ = src/main.o src/client.o src/protocol_qt.o ../common/protocol.o

../bin/client: $(OBJ)
	$(CXX) $(OBJ) -o ../bin/client $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../common/%.o: ../common/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o ../common/*.o ../bin/client
