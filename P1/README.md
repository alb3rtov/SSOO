# Práctica 1 SSOO II
## Proyect url 
https://github.com/alb3rtov/SSOO

## Directories
- include: Contains header files
- obj: Object files
- exec: Executables files
- src: Contains the source C code of each process.
- test_models: Contains three test models pdf files.
- estudiantes: It is created during the execution.

## Compile
To compile you have to use Makefile and type `make`

## Execute
To execute you have to use Makefile and type `make solution`

## Clean directories
To clean the directory use Makefile and type `make clean`

## Background daemon process
All process will be manage for the manager process except the daemon 'backup' that it will be executing en background each 60 seconds. If you are going to clean the directory **make sure to kill this process first**.
