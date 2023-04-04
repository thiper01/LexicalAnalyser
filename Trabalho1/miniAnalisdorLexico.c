/*
Implementacao do automato para a express�o regular
(+|-|vazio)(0|1|3|4|5|6|7|8|9)+
*/
#include <stdio.h>
#include <ctype.h> // isdigit
#include <stdlib.h> // free, calloc
#include <string.h> // strcmp

typedef enum{
    ERRO,NUMERO_INTEIRO,IDENTIFICADOR,WHILE,EOS
}TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_identificador[15];
}TInfoAtomo;

// variaveis globais
char *cadeia;
int linha=1;
// declaracao de funcoes
void reconhece_numero(TInfoAtomo *InfoAtomo);
void reconhece_identificador(TInfoAtomo *InfoAtomo);

void leArquivo(char *nomeArquivo);
TInfoAtomo obter_atomo();

int main(void){

    char *iniBuffer;
    leArquivo("entrada.txt");
    iniBuffer = cadeia;
    // ESSA PARTE SER SUBSTITUIDA NA IMPLEMENTACAO DO ANALISADO SINTATICO
    while( 1 ){ // repeticao infinita
        TInfoAtomo InfoAtomo = obter_atomo();
        // TENTEM IMPRIMIR A CADEIA ANALISADA
        if( InfoAtomo.atomo == NUMERO_INTEIRO)
            printf("\nlinha %d: atomo = NUMERO INTEIRO | atributo %d.", InfoAtomo.linha, InfoAtomo.atributo_numero);
        else if( InfoAtomo.atomo == IDENTIFICADOR)
            printf("\nlinha %d: atomo = IDENTIFICADOR | atributo %s.", InfoAtomo.linha, InfoAtomo.atributo_identificador);
        else if( InfoAtomo.atomo == WHILE)
            printf("\nlinha %d: atomo = WHILE ", InfoAtomo.linha);
        else if(InfoAtomo.atomo == ERRO){
            printf("\nlinha %d: ERRO LEXICO\n",InfoAtomo.linha);
            break;
        }
        else if(InfoAtomo.atomo == EOS){
            printf("\nfim de analise lexica\n");
            break; // quebra o laco
        }
    }
    free(iniBuffer);
    return 0;
}

TInfoAtomo obter_atomo(){

    TInfoAtomo InfoAtomo;
    InfoAtomo.atomo = ERRO;
    // descarta delimitadores
    while(  *cadeia == 0 ||  *cadeia == ' ' || *cadeia == '\n' || *cadeia == '\r'){
        if( *cadeia == 0 ){
            InfoAtomo.atomo= EOS; // finaliza o programa
            InfoAtomo.linha = linha;
            return InfoAtomo;
        }
        if( *cadeia == '\n')
            linha++;
        cadeia++;

    }

    InfoAtomo.linha = linha;
    if( isdigit(*cadeia))
        reconhece_numero(&InfoAtomo);
    if( islower(*cadeia))
        reconhece_identificador(&InfoAtomo);
    return InfoAtomo;
}

// funcao reconhece identificdor
// cada rotulo representa um estado na minha funcao
void reconhece_identificador(TInfoAtomo *InfoAtomo){
    int i=0;
q0:
   if(islower(*cadeia)){
        InfoAtomo->atributo_identificador[i++]= *cadeia;
        // consome o simbolo
        cadeia++;
        // vai para q1
        goto q1;
   }
   // retorna erro caso venha um simbolo desconhecido
   return;
q1:
   if(isalpha(*cadeia)){
        InfoAtomo->atributo_identificador[i++]= *cadeia;
        // consome o simbolo
        cadeia++;
        // vai para q1
        goto q1;
   }
   if( isdigit(*cadeia))
        // retorna erro caso venha um simbolo desconhecido
        return ;
   
   // copia o lexema do IDENTIFICADOR
   InfoAtomo->atributo_identificador[i]= 0;
   // testa se o lexema eh igual a while
   if( strcmp(InfoAtomo->atributo_identificador, "while")==0)
        InfoAtomo->atomo = WHILE;
   else
        InfoAtomo->atomo = IDENTIFICADOR; // reconhece IDENTIFICADOR
   
   
   return;

}

// funcao reconhece numeros inteiros
// cada rotulo representa um estado na minha funcao
void reconhece_numero(TInfoAtomo *InfoAtomo){
    char strNum[20];
    int i=0;
q0:
   //if(*cadeia == '0'|| *cadeia == '1'...)
   if(isdigit(*cadeia)){
        strNum[i++]= *cadeia;
        // consome o simbolo
        cadeia++;
        // vai para q1
        goto q1;
   }
   // retorna erro caso venha um simbolo desconhecido
   return;
q1:
   if(isdigit(*cadeia)){
        strNum[i++]= *cadeia;
        // consome o simbolo
        cadeia++;
        // vai para q1
        goto q1;
   }
   if( isalpha(*cadeia))
        // retorna erro caso venha um simbolo desconhecido
        return ;
   
   InfoAtomo->atomo = NUMERO_INTEIRO; // reconhece numero inteiro
   // converte a lexema que gerou o atomo para inteiro
   strNum[i] = 0;
   InfoAtomo->atributo_numero = atoi(strNum);
   
   return;

}


// rotina le um arquivo e armazena no buffer cadeia
void leArquivo(char *nomeArquivo){
    int tamArquivo;
    // abre o arquivo
    FILE *fp;
    fp = fopen(nomeArquivo,"r");
    if( !fp ){
        printf("falha na abertura de arquivo.\n");
        exit(1);
    }
    
    // move ponteiro para o final
    fseek(fp, 0, SEEK_END);
    tamArquivo=ftell(fp); // pega o numero de bytes deslocas
    fseek (fp, 0, SEEK_SET);// move o ponteiro para o inicio
    // aloca um buffer com a quantide de bytes
    cadeia = (char*)calloc(tamArquivo+1,sizeof(char));
    
    // le todos os caracteres do arquivo
    fread(cadeia,1,tamArquivo,fp);
    //printf("%s",cadeia);
    fclose(fp);
}
