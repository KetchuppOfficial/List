CC = gcc

CFLAGS = -c -Wall -Wextra -Werror

MY_LIB = ../My_Lib/My_Lib.a

List: main.o List.o
	$(CC) main.o List.o $(MY_LIB) -o List.out

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

List.o: List.c
	$(CC) $(CFLAGS) List.c -o List.o

run:
	./List.out

clean:
	rm -rf *.o
	rm List.out

clean_log:
	rm *.log
