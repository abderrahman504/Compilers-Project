# Lexical rules representation
The implemented generator will be given the rules of the language in a file. An example file is present in [Phase 1](./Phase%201.pdf)  
Here is an example:  
letter = a-z | A-z  
digit = 0-9  
id: letter (letter|digit)*  
digits = digit+
{boolean float int}
num: digit+ | digit+ . digits(\L | E digits)


# Class Diagram

```mermaid
classDiagram
	class Automata{
		-initialState : State
		-currentState : State[]
		-lastAcceptor : State[]
		-lastAcceptorIdx : int
		-acceptorEncountered : bool
		-inputIdx : int
		-inputStream : String

		+Automata(State initialState)
		+~Automata()
		-reset()
		+setInput(String input)
		+nextToken() String
		+getLastAcceptor() State
		+getLastTokenEnd() int
		+isFinished() boolean
	}
	
	class State{
		+name : String
		-acceptor : boolean
		-acceptorPriority : int
		-transitions : HashMap~char, State[]~
		-isNull : boolean

		+State(String name, bool isAcceptor, int acceptorPriority, bool isNull)
		+~State()
		+addTransition(char input, State[] output)
		+getTransition(char input) State[]
		+isAcceptor() boolean
		+getAcceptorPriority() int
		+isNullState() boolean
	}

	Automata o-- State
	
	class FileParser{
		-definitions : HashMap~String, String~
		-expressions : HashMap~String, String~
		-keywords : List~String~
		-punctuations : List~char~

		+readFile(String filePath)
		+getDefinitions() HashMap~String, String~
		+getExpressions() HashMap~String, String~
		+getKeywords() List~String~
		+getPunctuations() List~char~
	}

```


```mermaid
classDiagram
	class NFABuilder{

		+buildAutomata(definitions, expressions, keywords, punctuations) Automata
	}


	class DFABuilder{
		+buildAutomata(Automata NFA) Automata
	}


```