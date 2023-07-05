# Linux-Mini-Shell
This is a Linux Mini Shell that supports single or sequence commands(ls, sort, etc...), when these end with the character '&' they are executed asynchronously. 

A command sequence is executed using pipes, were each command is separated by '|'.

It also has add and mod functionality were you invoke 'mycalc' followed by the operation you want to do.

Also you can use 'mycp' to copy a file following the next format. MSH> mycp <original_file> <end_file>

If the execution of the mini shell raises an error you should try to indicate to the compiler were the library is using:

export LD_LIBRARY_PATH=/home/username/path:$LD_LIBRARY_PATH 

Were path indicates the location of the compiled msh file.
