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

struct RGB compacta(int len_x, int len_y, struct RGB *matriz[len_x][len_y]){

    int split_x = len_x / 2;
    int split_y = len_y / 2;

    if(len_x <= 3 || len_y <= 3)
        return *matriz[(int)(split_x)][(int)(split_y)];

    compacta(split_x, split_y, matriz[0][0]);
    compacta(split_x, split_y, &matriz[0][split_y]);
    compacta(split_x, split_y, &matriz[split_x][0]);
    compacta(split_x, split_y, &matriz[split_x][split_y]);
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

    image = &image[imagem.offset];

    struct RGB matriz[imagem.linhas][imagem.colunas];

    int padding = (4 - (imagem.linhas % 4)) % 4;
    for(int i = 0, j = 0; i < imagem.colunas; i++){
        for(int k = 0; k < (imagem.linhas); j += 3, k++){

            matriz[k][i].verde = image[j];
            matriz[k][i].azul = image[j + 1];
            matriz[k][i].vermelho = image[j + 2];

            printf("%d - Green: %02x (%d) ", k, image[j], i);
            printf(" Blue: %02x (%d) ", image[j + 1], i);
            printf(" Red: %02x (%d) %02X\n", image[j + 2], i, j);
        }
        j += padding;
    }

    
    for(int i = 0; i < imagem.linhas; i++){
        for(int j = 0; j < imagem.colunas; j++){
           printf("\"#%02X%02X%02X\",\n", matriz[i][j].vermelho, matriz[i][j].verde, matriz[i][j].azul); 
        }
    }

    return 0;
}
