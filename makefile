TARGET:=out.bin

all:
	g++ src/*.cpp -o $(TARGET)

clean:
	rm -rf $(TARGET) *.txt
