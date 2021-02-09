# ed/edX editor

ed/edX editor, final project for the 'Algorithms and Principles of Computer Science' course. 

The project consists in an implementation of a simple text editor.

The documents are made by a sequence of lines, numbered starting from one. 

The interface of the editor resembles the original "ed" editor, with textual commands.

Each command ends with an "end line" character, and can be followed by a portion of text - made by one or more lines - ending with a "." that will appear as the only character on the last line.

Some commands have integers that are line identifiers. An n-th line identifier represents the address of the n-th line, with the first one having "1" as its address.

The supported commands are the following, with the convention that ind1<=ind2, where ind1 and ind2 are the line identifiers. 
(parenthesis are added for readability, they are not part of the program)



-  (ind1, ind2)c

    Changes the text between the lines ind1 and ind2. The following text should have ind2-ind1+1 lines, and ind1 should be an identifier for a line that is actually present in the editor, or the first free line.
    
    
-  (ind1, ind2)d

    Deletes the lines between ind1 and ind2, shifting upwards lines after ind2, if any. Deletion of a non-existent line has no effect.
   
   
-  (ind1, ind2)p

    Prints lines from ind1 to ind2, included. If there is no line present, will print "." followed by a new line.
  
  
-   (n)u


    Undos a number n commands (c or d). A sequence of "undo" commands undos a number of instructions equal to the sum of the specified number of each. The execution of a c or d after an undo deletes the command history.
    
    
-   (n)r

    Nullifies the effect of an undo for a number of commands equal to n. The latter should be greater than 0. 
    A sequence such as 
    
    10u
    5r
    
    is equivalent to 5u.
  
  
  
-   q

    Quits from the editor. 
    
 
