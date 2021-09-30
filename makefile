TARGET:=out.bin

all:
	g++ src/*.cpp -std=c++11 -o $(TARGET)

clean:
	rm -rf $(TARGET) *.txt
