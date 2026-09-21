CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Wold-style-cast -O2
OBJDIR   = build
TARGET   = yafu

SRCDIR  = $(if $(wildcard src/*.cpp),src,.)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
