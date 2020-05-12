build: build-p1 build-p2
build-p1:
	gcc -std=c99 schimbare.c -o p1 -Wall
build-p2:
	gcc -std=c99 reprezentanti.c -o p2 -Wall
run-p1:
	./p1
run-p2:
	./p2
clean:
	rm p1 p2 p3 p4 bonus
