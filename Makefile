compile:
	gcc trigger.c -o compressao.mac 

run:
	./compressao.mac imagens/imagem22x20.bmp

all: compile run