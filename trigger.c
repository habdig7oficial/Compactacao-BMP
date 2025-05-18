#include "stdio.h"
#include "stdlib.h"
#include "string.h"


struct Imagem {
    int linhas;
    int colunas;
    int offset;
};

struct RGB {
    short int vermelho;
    short int verde;
    short int azul;
};

struct RGB compacta_lossy(int len_x, int len_y, unsigned char matriz[len_y][len_x]){
    for(int i = 0; i < len_y; i++){
        for(int j = 0; j < len_x; j++){
            printf("%02X ", matriz[i][j]);
        }
        printf("\n");
    } 
}

int main(int argc, char *argv[]){

    char path[30];
    if(argv[1] != NULL){
        strcpy(path, argv[1]);
    }
    else {
        printf("Nenhum arquivo fornecido!\n");
        return -1;
    }

    FILE *imagem_original = fopen(path, "r");
    if(imagem_original == NULL){
        printf("Esta Imagem Não existe.\n");
        return -2;
    }

    fseek(imagem_original, 0, SEEK_END);

    int file_size = ftell(imagem_original);

    printf("O Arquivo tem %d bytes\nalocando memoria...\n\n", file_size);

    rewind(imagem_original);

    unsigned char *image = malloc(file_size);

    fread(image, file_size, 1, imagem_original);


    struct Imagem imagem = {
        .linhas = (int)image[18],
        .colunas = (int)image[22],
        .offset = (int)image[10]
    };

    printf("\nNúmero de linhas: %d (0x%02X)\nNumero de colunas: %d (0x%02X)\n\nComeço da Imagem: %d (0x%02X)\n", imagem.linhas, imagem.linhas, imagem.colunas, imagem.colunas, imagem.offset, imagem.offset);


    unsigned char (*imagem_matriz)[imagem.linhas] = (unsigned char (*)[imagem.linhas])&image[imagem.offset];

    compacta_lossy(imagem.linhas, imagem.colunas, imagem_matriz);

    return 0;
}