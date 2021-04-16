# Práctica 2 SSOO II
## Proyect url 
https://github.com/alb3rtov/SSOO

## Directories
- include: Contains header files
- exec: Executables files
- src: Contains the source C++ code of each process.
- books: Contains the txt files where program gets the text.

## Compile
To compile you have to use Makefile and type `make`

## Execute
In order to modify the parameters of file, word and the number of threads, you need to edit Makefile.
In the `line 18` you can change the paremets:   

  `./exec/manager FILENAME WORD NUMBER_OF_THREADS`

To execute you have to use Makefile and type `make ssooiigle`

## Clean directories
To clean the directory use Makefile and type `make clean`

In order to add more txt files to read, you must to include it in the **books directory**.