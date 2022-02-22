exec = brain
sources = $(wildcard *.c)
objects = $(sources:.c=.o)
flags = -g -Wall -O0
cc = clang

$(exec): $(objects)
	$(cc) $(objects) $(flags) -o $(exec) 

%.o: %.c 
	$(cc) -c $(flags) $< -o $@ 

clean:
	-rm *.out
	-rm *.o

exec: $(exec)

all: clean exec
