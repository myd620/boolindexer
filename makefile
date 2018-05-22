CXX=g++
CFLAGS = -std=c++11 -O2 -Wall -fPIC
INC=-Iboolindexer
SRCS = $(wildcard boolindexer/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS) ) 
TARGET = index
LIBDIR = lib
LIB = -L ./$(LIBDIR) -lindexer

$(TARGET) : main.cpp $(LIBDIR)/libindexer.a
	$(CXX) -o $@  $^ $(CFLAGS) $(INC) $(LIB)

$(LIBDIR)/libindexer.a : $(OBJS) 
	mkdir -p lib
	ar rcs -o $@ $^

$(OBJS) : %.o : %.cpp
	$(CXX) -o $@ -c $^  $(CFLAGS) 



clean :
	rm  $(OBJS) $(TARGET)
