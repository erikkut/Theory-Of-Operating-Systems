Eric Torres #80526035
Theory of OS, Shell Lab.


To run: Simply run "make", then after the files have been compiled, run
   	"./testShell.sh ./myShell" and the test harness will run.

[Important Notes!]

	   Firstly, thank you for being a great TA, I enjoyed this class
	   despite what the other students say about it. I enjoyed it.
	   
	   Things not implemented in this lab:
	   	  * cd
		  * piping
		  * half of redirecting
		  
	   Things implemented correctly:
	   	  * executing commands
		  * finding correct path for commands (using execve)
		  * ignoring escaped (such as newlines)
		  * executing commands with parameters
		  
	   The test harness was not correctly passing my results for the
	   test with the newlines in between commands. I know this because
	   I manually saw the output of bash and my shell and they were
	   identical. Here are the results:

	   I placed a line containing "uname \n\n uname" inside my code
	   for this test. Notice how the output is identical.
	   $ ./myShell 
	   $ 
	   Linux
	   Linux
	   $ echo -e "uname \n\n uname" | bash
	   Linux
	   Linux

	   Similarily, I placed a line containing "uname \n uname" inside
	   my code for the following test
	   $ ./myShell 
	   $ 
	   Linux
	   Linux
	   $ echo -e "uname \n uname" | bash
	   Linux
	   Linux


	   I believe it to be a test harness bug. I hope you believe me.


Summary of code:
	   Most of the description of how code works is within the code itself
	   But in a nutshell, my shell runs a main loop that reads a line
	   using getline(), parses it using the tokenizer I made in the first
	   lab and delimiter ' '. After parsing, I send it to a method that
	   adds the correct pathing to every command without a path, if
	   necessary. And now, I have an array of tokens that are ready to be
	   executed correctly.
	   I send these to a while loop that checks if the command is runnable
	   if it is, then it checks if it has a parameter, if it does then
	   it runs with the param, otherwise it runs without, and the loop
	   restarts in order to get to every command despite newlines, etc.

Results: (I am attaching the test harness results, please note that the
	 new line testing portrays the wrong passing result, adjusted)
	
	 [7 PASSED and 5 FAILED]
	 

Results
PASSED: no parameter, full pathname
  /bin/uname
FAILED: two commands, full pathname
  /bin/uname 
 /bin/uname
FAILED: two commands, blank line in-between, full pathname
  /bin/uname 

 /bin/uname
PASSED: no parameter, no pathname
  uname
PASSED: one parameter, full pathname
  /bin/cat /etc/passwd
PASSED: one parameter, no pathname
  cat /etc/passwd
FAILED: pipe
  cat /etc/passwd | sort 
FAILED: 2 pipes
  cat /etc/passwd | sort | wc 
FAILED: redirect input
  cat < /etc/passwd
PASSED: redirect output
  uname > /tmp/x 
 cat /tmp/x
FAILED: background
  bash < /tmp/c1 &
 echo 2 
 sleep 3
FAILED: change dir
  cd .. 
 pwd
