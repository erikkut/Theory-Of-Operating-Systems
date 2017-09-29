How to run:
    Clone repository folder called "tokenizer" and type 'make' in console in
    order to compile the program using the makefile.

    The executable is called "test", to run type './test' in console in order
    run the program.

    Follow instructions from program. 2 Modes where added, pre-set input
    mode and user input mode.
    [Important!] Currently, user input mode prints out one token per line,
    this is because it calls mytoc() on each word seperately, which is due
    to the lack of a line scanner in c. A function to read the line could
    be made, but it would defeat the purpose of mytoc().
    However, one can continue typing inputs as desired.

Explanation:
	Comments where added in mytoc() to explain how it works and some of
	the reasoning behind the code.
	[Important!] test.c is merely meant to be the demo code, I did not
	add many comments because it is self explanatory and not part of the
	main assignment.

Results:
--------------------------------------------------------------

USING INPUT STRING "Hello my dog's name is Dawrin."

[student@localhost tokenizer]$ ./test
Enter 0 for preset string demo or a 1 for loop
0
Preset demo.

From allocated memory address- 145995804: Hello

From allocated memory address- 145995808: my

From allocated memory address- 145995812: dog's

From allocated memory address- 145995816: name

From allocated memory address- 145995820: is

From allocated memory address- 145995824: Darwin.

Single line form: Hello my dog's name is Darwin.

--------------------------------------------------------------

USING INPUT STRING "     Hello   World!   "

[student@localhost tokenizer]$ ./test
Enter 0 for preset string demo or a 1 for loop
0
Preset demo.

From allocated memory address- 154646556: Hello

From allocated memory address- 154646560: World!

Single line form: Hello World!

--------------------------------------------------------------

USING INPUT STRING "          "

[student@localhost tokenizer]$ ./test
Enter 0 for preset string demo or a 1 for loop
0
Preset demo.

Single line form:

--------------------------------------------------------------

USING LOOP PART OF PROGRAM

[student@localhost tokenizer]./test 
Enter 0 for preset string demo or a 1 for loop
1
Loop entered.
Enter 'exit' to exit loop, enter any other string, to test that string
$ hello, testing, this should turn into eight words
hello, 
testing, 
this 
should 
turn 
into 
eight 
words 

exit
exit 
Loop exited. Ending program.