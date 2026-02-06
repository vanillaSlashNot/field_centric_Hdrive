# This project has been converted to PROS with EZ Template
# Install PROS CLI and open this project with: prosv5 conduct
# Or use "prosv5 make" to build

.PHONY: all clean download

build:
	prosv5 make -j4

clean:
	prosv5 make clean

download: build
	prosv5 upload

all: build
