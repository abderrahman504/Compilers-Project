OBJ_FILES := build/main.o \
build/lexical_analysis/State.o \
build/lexical_analysis/Automata.o \
build/lexical_analysis/rules_parsing/FileParser.o \
build/lexical_analysis/automata_construction/NfaBuilder.o \
build/lexical_analysis/automata_construction/DFAConstructor.o \
build/lexical_analysis/Scanner.o \
build/lexical_analysis/ProgramAnalyzer.o \
build/Parser/Grammar.o \
build/Parser/GrammerFileReader.o \
build/Parser/ParsingTable.o \
build/Parser/Parser.o 




FOLDERS := build \
build/lexical_analysis \
build/lexical_analysis/rules_parsing build/lexical_analysis/automata_construction \
build/Parser

EXECUTABLE_NAME := project


all : make-folders exec_compile

make-folders: $(FOLDERS)

$(FOLDERS):
ifeq ($(OS),Windows_NT)
	mkdir $(subst /,\,$@)
else
	mkdir $@
endif

exec_compile : $(OBJ_FILES)
	g++ $(OBJ_FILES) -o build/$(EXECUTABLE_NAME).exe


build/%.o : src/%.cpp
	g++ -g -c $^ -o $@


clean:
ifeq ($(OS),Windows_NT)
	del /q /s build\*
else
	rm -rf build/*
endif