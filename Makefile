CC = g++

CFLAGS = -c -Wall -Wextra -Werror

List: main.o List.o Reading_File.o Log_File.o
	$(CC) main.o List.o Reading_File.o Log_File.o -o List.out

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o

List.o: List.cpp
	$(CC) $(CFLAGS) List.cpp -o List.o

Reading_File.o: Reading_File.cpp
	$(CC) $(CFLAGS) Reading_File.cpp -o Reading_File.o

Log_File.o: Log_File.cpp
	$(CC) $(CFLAGS) Log_File.cpp -o Log_File.o

run:
	./List.out

clean:
	rm -rf *.o
	rm List.out
