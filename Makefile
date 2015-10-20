all:
	gcc -g  main.c str.c str.h scanner.c  scanner.h -o scanner

clean:
	rm -rf scanner
