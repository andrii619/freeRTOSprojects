

all: clean
	cmake -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	cmake --build ./Release


debug: clean
	cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./Debug


clean:
	rm -rf ./Debug 
	rm -rf ./Release


clangtidy:
	rm -f ./clangtidy.out
	cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
	cmake --build ./Debug --target nucleo_f746zg_led_effect_clangtidy > clangtidy.out


#cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug