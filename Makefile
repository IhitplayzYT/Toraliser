all: 
	gcc Toraliser.c -o Toraliser.so -fPIC -shared -ldl -D_GNU_SOURCE