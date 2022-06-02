gcc -c forth.c
gcc -c StackPrimitives.c
gcc -c SystemPrimitives.c
gcc -c Utilities.c
gcc -c VFM_Build_Primitives.c
gcc -o 4th.exe forth.o StackPrimitives.o  SystemPrimitives.o Utilities.o VFM_Build_Primitives.o
