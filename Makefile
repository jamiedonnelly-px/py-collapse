.PHONY: all clean rebuild

BUILD_DIR = build
BINARY = main.o

all:
	@mkdir -p build
	@cd build && cmake .. && make
	@mkdir -p bin
	@cp build/${BINARY} bin

clean:
	@rm -rf build
	@rm -rf bin
	@find . -name *.so | xargs -I {} rm -f {}
	@find . -name __pycache__ | xargs -I {} rm -rf {}
	@find . -name *.pyd | xargs -I {} rm -f {}
	@find . -name *.egg* | xargs -I {} rm -rf {}