# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -Ilib/ftxui/include
LDFLAGS = -Llib/ftxui/build -lftxui-screen -lftxui-dom -lftxui-component -pthread
# Target executable
TARGET = interpreter.x

# Sources and objects
SOURCES = main.cpp commentDFA.cpp tokenizer.cpp parser.cpp table.cpp tree.cpp interpreter.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o *.txt