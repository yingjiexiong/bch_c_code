CC=gcc
prom = rfid_test
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")

obj = main.o bch.o 


all: $(prom) clear_obj

$(prom): $(obj)
	$(CC) -o $(prom) $(obj)


%.o: %.c
	$(CC) -c $< -o $@

clear: 
	rm -f $(obj) $(prom)

clear_obj:
	rm -f $(obj) 

windows:
	i686-w64-mingw32-gcc -o rfid_test.exe main.c bch.c bch.h