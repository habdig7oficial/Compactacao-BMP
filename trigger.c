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
        .linhas = (int)image[18] + 1,
        .colunas = (int)image[22] + 1,
        .offset = (int)image[10] + 1
    };

    printf("\nNúmero de linhas: %d (0x%02X)\nNumero de colunas: %d (0x%02X)\n\nComeço da Imagem: %d (0x%02X)\n", imagem.linhas, imagem.linhas, imagem.colunas, imagem.colunas, imagem.offset, imagem.offset);

    image = &image[imagem.offset];

    struct RGB matriz[imagem.linhas][imagem.colunas];

    

    for(int i = 0, j = 0, l = 0; i < ((imagem.colunas) * (imagem.linhas)) * 3; i++){
        if(i % 3 == 0){
            matriz[l][j].vermelho = image[i];
           printf("%d - Red: %02x (%d) ", l, image[i], j);
        }
        else if(i % 3 == 1){
            matriz[l][j].verde = image[i];
            printf("Green: %02x (%d) ", image[i], j);
        }
        else{
            matriz[l][j].azul = image[i];
            printf("Blue: %02x (%d) \n", image[i], j);
            j++;
            if(j == imagem.colunas){
                j = 0;
                l++;
            }
        }   
    }

    for(int i = 0; i < imagem.colunas; i++){
        for(int j = 0; j < imagem.colunas; j++){
            //printf("#%02X%02X%02X\n", matriz[i][j].vermelho, matriz[i][j].verde, matriz[i][j].azul); 
        }
    }





    return 0;
}

/*

struct RGB compacta_lossy(int len_x, int len_y, unsigned char matriz[len_y][len_x], int soma_x, int soma_y){
    printf("\nEnd\n\n");

    int setor_x = len_x / 2;
    int setor_y = len_y / 2;
    if( setor_x >= 3 && setor_y / 2 >= 3){
      // compacta_lossy(setor_x, setor_y, matriz);
    }    //compacta_lossy(imagem.linhas, imagem.colunas, imagem_matriz);

        int i, j = 0;
        for(i = 0; i < 0x36; i++)
            printf("%02X ", image[i]);
        printf("\n");
        for(i=0; i < 20; i++)
        {
          for (j=0; j<22; j++)
            printf("%02X%02x%02x ", image[0x36+i*22*3+j*3], image[0x36+i*22*3+j*3+1], image[0x36+i*22*3+j*3+2]);
        printf ("\n");
        }


            unsigned char (*imagem_matriz)[imagem.linhas] = (unsigned char (*)[imagem.linhas])&image[imagem.offset];

}*/