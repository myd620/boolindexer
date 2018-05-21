CXX=g++
CFLAGS = -std=c++11 -O2 -Wall -fPIC
INC=-Iboolindexer
SRCS = $(wildcard boolindexer/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS) ) 
TARGET = index
LIB = -L ./boolindexer -lindexer

$(TARGET) : main.cpp boolindexer/libindexer.a
	$(CXX) -o $@  $^ $(CFLAGS) $(INC) $(LIB)

boolindexer/libindexer.a : $(OBJS) 
	ar rcs -o $@ $^

$(OBJS) : %.o : %.cpp
	$(CXX) -o $@ -c $^  $(CFLAGS) 



clean :
	rm  $(OBJS) $(TARGET)
