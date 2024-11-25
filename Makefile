OBJ_FILES := build/main.o build/Automata.o build/State.o



all : build


build : $(OBJ_FILES)
	g++  $(OBJ_FILES) -o ./build/project.exe


build/%.o : src/%.cpp
	g++ -g -c $^ -o $@


clean:
	rm -rf build/*
