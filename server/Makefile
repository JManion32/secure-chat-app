CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I../common

OBJ = \
    src/main.o \
    src/client_thread.o \
    src/server.o \
    src/message_system.o \
    ../common/src/protocol.o \
    ../common/src/socket_handler.o \
    ../common/src/thread_handler.o

../bin/linux/server: $(OBJ)
	$(CXX) $(OBJ) -o ../bin/linux/server -pthread

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../common/src/%.o: ../common/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o ../common/src/*.o ../bin/linux/server
