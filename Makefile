.PHONY: all clean run

BUILD_DIR = build
BINARY = main.o

build:
	@mkdir -p build
	@cd build && cmake .. && make
	@mkdir -p bin
	@cp build/${BINARY} bin

run:
	@echo "Running main..."
	@./bin/main.o

all: clean build run

clean:
	@rm -rf build
	@rm -rf bin
	@find . -name *.so | xargs -I {} rm -f {}
	@find . -name __pycache__ | xargs -I {} rm -rf {}
	@find . -name *.pyd | xargs -I {} rm -f {}
	@find . -name *.egg* | xargs -I {} rm -rf {}