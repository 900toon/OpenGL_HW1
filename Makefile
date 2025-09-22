all: build
	cd build && make && cd src && ./HW1

build: src/*.cpp src/header/*.h
	cmake -S . -B build 
run: 
	cd build && make && cd src && ./HW1
clean:
	rm -r build
