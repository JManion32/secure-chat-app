CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I../common

OBJ = src/main.o src/server.o ../common/protocol.o

../bin/server: $(OBJ)
	$(CXX) $(OBJ) -o ../bin/server -pthread

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../common/%.o: ../common/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o ../common/*.o ../bin/server
