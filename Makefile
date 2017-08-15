OUT=ThreadPool

CXX=g++
INCLUDE_DIR=`pwd`
INCLUDE_FLAGS=-I $(INCLUDE_DIR)
LD_FLAGS=-lpthread
OUT:*.o
	$(CXX) -o $(OUT) *.o $(LD_FLAGS)

*.o:*.cpp
	$(CXX) -c -g *.cpp $(INCLUDE_FLAGS) 

clean:
	rm *.o $(OUT)
	
