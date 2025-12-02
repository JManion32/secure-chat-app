CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -I../common
OBJ = src/main.o ../common/protocol.o

../bin/linux/server: $(OBJ)
	$(CXX) $(OBJ) -o ../bin/linux/server -pthread

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

../common/%.o: ../common/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o ../common/*.o ../bin/linux/server
