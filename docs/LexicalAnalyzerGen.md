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
		-lastAcceptor : Pair~State[], int~
		-consumedInput : StringBuilder

		+reset()
		+next(char input)
		+getState() State[]
		+getLastAcceptor()
	}
	
	class State{
		+name : String
		-acceptor : boolean
		-acceptorPriority : int
		-transitions : HashMap~char, State[]~
		-isNull : boolean

		+addTransition(char input, State[] output)
		+getTransition(char input) State[]
		+isAcceptor() boolean
		+getAcceptorPriority() int
		+isNullState() boolean
	}

	Automata o-- State
	
	class FileParser{
		-difinitions : HashMap~String, String~
		-expressions : HashMap~String, String~
		-keywords : List~String~
		-punctuations : List~char~

		+readFile(String filePath)
		+getDefinitions() HashMap~String, String~
		+getExpressions() HashMap~String, String~
		-keywords : List~String~
		-punctuations : List~char~
	}

```


```mermaid
classDiagram
	class NFABuilder{

		+buildAutomata(definitions, expressions, keywords, punctuations) Automata
	}


	class DFABuilder{
		+buildAutomata(Automata NFAAutomata) Automata
	}


```