#include "stdio.h"
#include "stdlib.h"
#include "string.h"


struct Imagem {
    int linhas;
    int colunas;
    int offset;
};

struct RGB {
    unsigned char vermelho;
    unsigned char verde;
    unsigned char azul;
};

unsigned char *compacta(int len_x, int len_y, struct RGB matriz[len_x][len_y], unsigned char *buffer){

    int split_x = len_x / 2;
    int split_y = len_y / 2;

    printf("#%02X%02X%02X - <%d,%d>\n", matriz[split_x][split_y].vermelho, matriz[split_x][split_y].verde, matriz[split_x][split_y].azul, split_x,split_y);

    if(len_x <= 3 && len_y <= 3){
        printf("\n-------------\n");
        //return matriz[(int)(split_x)][(int)(split_y)];
        return 0;
    }

    //printf("Setor 1: \n");
    compacta(split_x, split_y, (struct RGB (*)[split_y])matriz , buffer);
    // printf("Setor 3: \n");
    compacta(split_x, split_y, (struct RGB (*)[split_x])&matriz[split_x], buffer);
    //printf("Setor 2: \n");
    compacta(split_x, split_y, (struct RGB (*)[split_y])&matriz[split_y], buffer);
    //printf("Setor 4: \n");
    compacta(split_x, split_y, (struct RGB (*)[])&matriz[split_x][split_y], buffer);
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

    FILE *imagem_original = fopen(path, "rb");
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

    unsigned char *compacted_arr = malloc(file_size);

    memcpy(compacted_arr, image, imagem.offset);

    image = &image[imagem.offset];

    struct RGB matriz[imagem.linhas][imagem.colunas];

    int padding = (4 - (imagem.linhas % 4)) % 4;
    for(int i = 0, j = 0; i < imagem.colunas; i++){
        for(int k = 0; k < (imagem.linhas); j += 3, k++){

            matriz[k][i].verde = image[j];
            matriz[k][i].azul = image[j + 1];
            matriz[k][i].vermelho = image[j + 2];

           // printf("%d - Green: %02x (%d) ", k, image[j], i);
            //printf(" Blue: %02x (%d) ", image[j + 1], i);
           // printf(" Red: %02x (%d) %02X\n", image[j + 2], i, j);
        }
        j += padding;
    }

    
    for(int i = 0; i < imagem.linhas; i++){
        for(int j = 0; j < imagem.colunas; j++){
          // printf("\"#%02X%02X%02X\",\n", matriz[i][j].vermelho, matriz[i][j].verde, matriz[i][j].azul); 
        }
    }

    compacta(imagem.linhas, imagem.colunas, matriz, compacted_arr);

    for(int i = 0; i < file_size; i++){
        printf("%02X ", compacted_arr[i]); 
    }

    return 0;
}
