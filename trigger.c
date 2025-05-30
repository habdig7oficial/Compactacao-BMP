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

struct Node {
    struct RGB  *pixel;
    struct Node *setor1;
    struct Node *setor2;
    struct Node *setor3;
    struct Node *setor4;
};

struct Node *compact_tree(int len_y, int len_x, struct RGB matriz[len_y][len_x], unsigned char *buffer){

    int split_x = len_x / 2;
    int split_y = len_y / 2;

    struct Node *v_atual = malloc(sizeof(struct Node));
    v_atual -> pixel = &matriz[split_x][split_y];

    printf("#%02X%02X%02X - <%d,%d>\n", v_atual -> pixel -> vermelho, v_atual -> pixel -> verde, v_atual -> pixel -> azul, split_x,split_y);

    if(len_x <= 3 && len_y <= 3){
        printf("\n-------------\n");
        //printf("where %02x\n", matriz[(int)(split_x)][(int)(split_y)].verde);
        //return matriz[(int)(split_x)][(int)(split_y)];
        // printf("Base: %p\n", buffer);
       // printf("#%02X%02X%02X - <%d,%d>\n", matriz[split_x][split_y].vermelho, matriz[split_x][split_y].verde, matriz[split_x][split_y].azul, split_x,split_y);
        return NULL;
    }

    //printf("Setor 1: \n");
    compact_tree(split_x, split_y, (struct RGB (*)[])&matriz , (buffer) + 3);

    
    // printf("Setor 3: \n");
    //compact_tree(split_x, split_y, (struct RGB (*)[split_x])&matriz[split_x], buffer - 3);

    //printf("Setor 2: \n");
    //compact_tree(split_x, split_y, (struct RGB (*)[split_y])&matriz[split_y], buffer - 3);

    //printf("Setor 4: \n");
    //compact_tree(split_x, split_y, (struct RGB (*)[])&matriz[split_x][split_y], buffer);

    return NULL;
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
        .linhas = (int)image[22],
        .colunas = (int)image[18],
        .offset = (int)image[10]
    };

    printf("\nNúmero de linhas: %d (0x%02X)\nNumero de colunas: %d (0x%02X)\n\nComeço da Imagem: %d (0x%02X)\n", imagem.linhas, imagem.linhas, imagem.colunas, imagem.colunas, imagem.offset, imagem.offset);

    unsigned char *compacted_arr = malloc(file_size);

    memcpy(compacted_arr, image, imagem.offset);

    image = &image[imagem.offset];

    struct RGB matriz[imagem.linhas][imagem.colunas];

    int padding = (4 - (imagem.colunas % 4)) % 4;
    for(int k = 0, j = 0; k < imagem.linhas; k++){
        for(int i = 0; i < (imagem.colunas); j += 3, i++){

            matriz[k][i].azul = image[j];
            matriz[k][i].verde = image[j + 1];
            matriz[k][i].vermelho = image[j + 2];

            printf("#%02X%02X%02X ", matriz[k][i].vermelho, matriz[k][i].verde, matriz[k][i].azul); 

           // printf("%d - Green: %02x (%d) ", k, image[j], i);
            //printf(" Blue: %02x (%d) ", image[j + 1], i);
           // printf(" Red: %02x (%d) %02X\n", image[j + 2], i, j);
        }
        j += padding;

        printf("\n");
    }


    compact_tree(imagem.linhas, imagem.colunas, matriz, &compacted_arr[imagem.offset]);

    for(int i = imagem.offset; i < 100; i++){
        //printf("%02X - %p \n", compacted_arr[i], &compacted_arr[i]); 
    }
    

    return 0;
}
