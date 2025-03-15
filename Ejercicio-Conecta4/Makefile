EXECUTABLE=test

all: $(EXECUTABLE)

$(EXECUTABLE): compile
	./run-tests

compile:
	gcc -Wall -Werror -o practica01 src/*.c

clean:
	rm -rf practica01

check: $(EXECUTABLE)
