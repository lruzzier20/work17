all: control.c write.c
	gcc -o control control.c
	gcc -o write write.c

create:
	./control Create

remove:
	./control Remove

run:
	./write

clean:
	rm control
	rm write
