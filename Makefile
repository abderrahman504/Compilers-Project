OBJ_FILES := build/main.o build/Automata.o build/State.o build/NfaBuilder.o build/RulesParsing/FileParser.o

EXECUTABLE_NAME := project


all : exec_compile


exec_compile : $(OBJ_FILES)
	g++  $(OBJ_FILES) -o ./build/$(EXECUTABLE_NAME).exe


build/%.o : src/%.cpp
	g++ -g -c $^ -o $@


clean:
	rm -rf build/*
