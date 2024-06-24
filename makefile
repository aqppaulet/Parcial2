main: build/main.o build/page.o build/bufferpoolmanager.o build/buffer.o build/frame.o build/diskspacemanager.o
	g++ build/main.o build/page.o build/bufferpoolmanager.o build/buffer.o build/frame.o build/diskspacemanager.o -o main

build/main.o: main.cpp | build
	g++ -c main.cpp -o build/main.o

build/page.o: src/Page.cpp include/Page.h | build
	g++ -c src/Page.cpp -o build/page.o

build/buffer.o: src/BufferManager.cpp include/BufferManager.h | build
	g++ -c src/BufferManager.cpp -o build/buffer.o

build/bufferpoolmanager.o: src/Buffer_Pool.cpp include/BufferPool.h | build
	g++ -c src/Buffer_Pool.cpp -o build/bufferpoolmanager.o

build/frame.o: src/Frame.cpp include/Frame.h | build
	g++ -c src/Frame.cpp -o build/frame.o

build/diskspacemanager.o: src/diskSpaceManager.cpp include/diskSpaceManager.h | build
	g++ -c src/diskSpaceManager.cpp -o build/diskspacemanager.o

build:
	mkdir -p build

run: main
	./main

clean:
	rm -rf build main
