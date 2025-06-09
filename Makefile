all: servidor cliente

servidor: servidor.c
	gcc servidor.c -o servidor

cliente: cliente.c
	gcc cliente.c -o cliente

clean:
	rm -f servidor cliente
