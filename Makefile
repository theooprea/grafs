build: build-p1 build-p2 build-p3 build-p4 build-bonus
build-p1:
	gcc -std=c99 schimbare.c -o p1 -Wall
build-p2:
	gcc -std=c99 reprezentanti.c -o p2 -Wall
build-p3:
	gcc -std=c99 drumuri.c -o p3 -Wall
build-p4:
	gcc -std=c99 labirint.c -o p4 -Wall
build-bonus:
	gcc -std=c99 bonus.c -o bonus -Wall
run-p1:
	./p1
run-p2:
	./p2
run-p3:
	./p3
run-p4:
	./p4
run-p5:
	./bonus
clean:
	rm p1 p2 p3 p4 bonus
