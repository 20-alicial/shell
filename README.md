This is an implementation of a simple interactive command-line interpreter, made for a school project during Spring 2022. Three built-in commands have been implemented: cd, ls and pwd. 

Once the program is run, the shell will await a command from the user. Once the user presses enter, it runs and then awaits another command. 

Multiple Commands:
The shell can accept and parse multiple commands in a single line: e.g if the user enters "ls;cd ..", the shell will first run ls and then cd.

White Spaces:
Any number of white spaces in a command is fine. E.g, "ls;ls" is the same as "ls; ls" and ls ;  ls".

The shell also supports redirection and advanced redirection. This means that instead of printing the result (e.g of "ls") to a console, it can redirect the result to a new or existing file.





