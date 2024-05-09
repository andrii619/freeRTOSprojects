

all: clean
	cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release


debug: clean
	cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug


clean:
	rm -rf ./Debug 
	rm -rf ./Release

#cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug