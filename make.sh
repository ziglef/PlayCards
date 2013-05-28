gcc -g -Wall -c dataStructs.c -lrt
gcc -g -Wall main.c dataStructs.o -o main -lrt -pthread
chmod 777 main
