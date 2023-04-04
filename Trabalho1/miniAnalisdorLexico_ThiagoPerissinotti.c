// THIAGO PERISSINOTTI
// TIA : 41903447

// Codigo indentado com ALT + SHIFT + F do VS Code
#include <stdio.h>
#include <ctype.h>  // isdigit
#include <stdlib.h> // free, calloc
#include <string.h> // strcmp

// Enum de possiveis tipos de atomos
typedef enum
{
    ERRO,
    NUMERO,
    IDENTIFICADOR,
    WHILE,
    EOS,
    ABRE_PAR,
    ADICAO,
    AND,
    BEGIN,
    BOOLEAN,
    CARACTERE,
    CHAR,
    COMENTARIO,
    DI,
    DIVISAO,
    DO,
    ELSE,
    END,
    FALSE,
    FECHA_PAR,
    IF,
    IG,
    MA,
    MAI,
    ME,
    MEI,
    MOD,
    MULTIPLICACAO,
    NOT,
    NUMBER,
    OR,
    PONTO,
    PONTO_VIRGULA,
    PROGRAM,
    READ,
    SET,
    SUBTRACAO,
    THEN,
    TO,
    TRUE,
    VIRGULA,
    WRITE
} TAtomo;

typedef struct
{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_identificador[15];
} TInfoAtomo;

// variaveis globais
char *cadeia;
int linha = 1;
// declaracao de funcoes
void reconhece_numero(TInfoAtomo *InfoAtomo);
void reconhece_identificador(TInfoAtomo *InfoAtomo);
void reconhece_comentario_varias_linhas(TInfoAtomo *InfoAtomo);
void reconhece_comentario_uma_linha(TInfoAtomo *InfoAtomo);
void reconhece_caracter(TInfoAtomo *InfoAtomo);

void leArquivo(char *nomeArquivo);
TInfoAtomo obter_atomo();

int main(void)
{
    char *iniBuffer;
    leArquivo("entrada.txt");
    iniBuffer = cadeia;

    while (1) // repeticao infinita
    {
        TInfoAtomo InfoAtomo = obter_atomo();

        if (InfoAtomo.atomo == EOS)
        {
            printf("\nfim de analise lexica\n");
            break; // quebra o laco
        }

        printf("\nlinha %d: ", InfoAtomo.linha);

        // Determina o tipo de atomo retornado
        if (InfoAtomo.atomo == AND)
            printf("AND");
        else if (InfoAtomo.atomo == BEGIN)
            printf("BEGIN");
        else if (InfoAtomo.atomo == BOOLEAN)
            printf("BOOLEAN");
        else if (InfoAtomo.atomo == CHAR)
            printf("CHAR");
        else if (InfoAtomo.atomo == DO)
            printf("DO");
        else if (InfoAtomo.atomo == ELSE)
            printf("ELSE");
        else if (InfoAtomo.atomo == END)
            printf("END");
        else if (InfoAtomo.atomo == FALSE)
            printf("FALSE");
        else if (InfoAtomo.atomo == IF)
            printf("IF");
        else if (InfoAtomo.atomo == MOD)
            printf("MOD");
        else if (InfoAtomo.atomo == NOT)
            printf("NOT");
        else if (InfoAtomo.atomo == NUMBER)
            printf("NUMBER");
        else if (InfoAtomo.atomo == OR)
            printf("OR");
        else if (InfoAtomo.atomo == PROGRAM)
            printf("PROGRAM");
        else if (InfoAtomo.atomo == READ)
            printf("READ");
        else if (InfoAtomo.atomo == SET)
            printf("SET");
        else if (InfoAtomo.atomo == THEN)
            printf("THEN");
        else if (InfoAtomo.atomo == TO)
            printf("TO");
        else if (InfoAtomo.atomo == TRUE)
            printf("TRUE");
        else if (InfoAtomo.atomo == WHILE)
            printf("WHILE");
        else if (InfoAtomo.atomo == WRITE)
            printf("WRITE");
        else if (InfoAtomo.atomo == IDENTIFICADOR)
            printf("IDENTIFICADOR - atributo: %s", InfoAtomo.atributo_identificador);
        else if (InfoAtomo.atomo == ABRE_PAR)
            printf("abre parenteses");
        else if (InfoAtomo.atomo == FECHA_PAR)
            printf("fecha parenteses");
        else if (InfoAtomo.atomo == PONTO)
            printf("ponto");
        else if (InfoAtomo.atomo == PONTO_VIRGULA)
            printf("ponto e virgula");
        else if (InfoAtomo.atomo == VIRGULA)
            printf("virgula");
        else if (InfoAtomo.atomo == SUBTRACAO)
            printf("SUBTRACAO");
        else if (InfoAtomo.atomo == ADICAO)
            printf("ADICAO");
        else if (InfoAtomo.atomo == DIVISAO)
            printf("DIVISAO");
        else if (InfoAtomo.atomo == MULTIPLICACAO)
            printf("MULTIPLICACAO");
        else if (InfoAtomo.atomo == ME)
            printf("OP_RELACIONAL - atributo(enumeracao) menor: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == MEI)
            printf("OP_RELACIONAL - atributo(enumeracao) menor igual: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == IG)
            printf("OP_RELACIONAL - atributo(enumeracao) igual: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == DI)
            printf("OP_RELACIONAL - atributo(enumeracao) diferente: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == MA)
            printf("OP_RELACIONAL - atributo(enumeracao) maior: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == MAI)
            printf("OP_RELACIONAL - atributo(enumeracao) maior igual: %d", InfoAtomo.atomo);
        else if (InfoAtomo.atomo == COMENTARIO)
            printf("comentario");
        else if (InfoAtomo.atomo == NUMERO)
            printf("NUMERO - atributo %s", InfoAtomo.atributo_identificador);
        else if (InfoAtomo.atomo == CARACTERE)
            printf("CARACTERE - atributo: %s", InfoAtomo.atributo_identificador);
        else if (InfoAtomo.atomo == ERRO)
        {
            printf("ERRO LEXICO\n");
            break;
        }
    }
    free(iniBuffer);
    return 0;
}

// Chama as outras funcoes e detecta o tipo de atomo ou fim de arquivo
TInfoAtomo obter_atomo()
{
    TInfoAtomo InfoAtomo;
    InfoAtomo.atomo = ERRO;
    // descarta delimitadores
    while (isspace(*cadeia) || *cadeia == 0)
    {
        if (*cadeia == 0)
        {
            InfoAtomo.atomo = EOS; // finaliza o programa
            InfoAtomo.linha = linha;
            return InfoAtomo;
        }
        if (*cadeia == '\n')
            linha++;
        cadeia++;
    }
    InfoAtomo.linha = linha;
    if (isdigit(*cadeia))
        reconhece_numero(&InfoAtomo);
    else if (isalpha(*cadeia))
    {
        reconhece_identificador(&InfoAtomo);
    }
    else if (*cadeia == '{')
    {
        reconhece_comentario_varias_linhas(&InfoAtomo);
    }
    else if (*cadeia == '(')
    {
        InfoAtomo.atomo = ABRE_PAR;
        cadeia++;
    }
    else if (*cadeia == ')')
    {
        InfoAtomo.atomo = FECHA_PAR;
        cadeia++;
    }
    else if (*cadeia == '.')
    {
        InfoAtomo.atomo = PONTO;
        cadeia++;
    }
    else if (*cadeia == ';')
    {
        InfoAtomo.atomo = PONTO_VIRGULA;
        cadeia++;
    }
    else if (*cadeia == ',')
    {
        InfoAtomo.atomo = VIRGULA;
        cadeia++;
    }
    else if (*cadeia == '<')
    {
        cadeia++;
        if (*cadeia != '=')
        {
            InfoAtomo.atomo = ME;
        }
        else
        {
            InfoAtomo.atomo = MEI;
            cadeia++;
        }
    }
    else if (*cadeia == '=')
    {
        InfoAtomo.atomo = IG;
        cadeia++;
    }
    else if (*cadeia == '>')
    {
        cadeia++;
        if (*cadeia != '=')
        {
            InfoAtomo.atomo = MA;
        }
        else
        {
            InfoAtomo.atomo = MAI;
            cadeia++;
        }
    }
    else if (*cadeia == '-')
    {
        InfoAtomo.atomo = SUBTRACAO;
        cadeia++;
    }
    else if (*cadeia == '+')
    {
        InfoAtomo.atomo = ADICAO;
        cadeia++;
    }
    else if (*cadeia == '/')
    {
        cadeia++;
        if (*cadeia == '/')
            reconhece_comentario_uma_linha(&InfoAtomo);
        else if (*cadeia == '=')
        {
            cadeia++;
            InfoAtomo.atomo = DI;
        }
        else
        {
            InfoAtomo.atomo = DIVISAO;
        }
    }
    else if (*cadeia == '*')
    {
        InfoAtomo.atomo = MULTIPLICACAO;
        cadeia++;
    }
    else if (*cadeia == '\'')
    {
        reconhece_caracter(&InfoAtomo);
    }
    return InfoAtomo;
}

// funcao reconhece identificdor
// cada rotulo representa um estado na minha funcao
void reconhece_identificador(TInfoAtomo *InfoAtomo)
{
    int i = 0;
    InfoAtomo->atributo_identificador[i] = tolower(*cadeia);
    cadeia++;
q0:
    if ((isalnum(*cadeia) || *cadeia == '_') && i < 13)
    {
        i++;
        InfoAtomo->atributo_identificador[i] = tolower(*cadeia);
        cadeia++;
        goto q0;
    }
    if (isspace(*cadeia) || *cadeia == ';' || *cadeia == ',' || *cadeia == '.' || *cadeia == ')' || *cadeia == '(')
    {
        i++;
        InfoAtomo->atributo_identificador[i] = 0;
        // procura se e uma palavra reservada
        if (strcmp(InfoAtomo->atributo_identificador, "and") == 0)
        {
            InfoAtomo->atomo = AND;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "begin") == 0)
        {
            InfoAtomo->atomo = BEGIN;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "boolean") == 0)
        {
            InfoAtomo->atomo = BOOLEAN;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "char") == 0)
        {
            InfoAtomo->atomo = CHAR;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "do") == 0)
        {
            InfoAtomo->atomo = DO;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "else") == 0)
        {
            InfoAtomo->atomo = ELSE;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "end") == 0)
        {
            InfoAtomo->atomo = END;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "false") == 0)
        {
            InfoAtomo->atomo = FALSE;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "if") == 0)
        {
            InfoAtomo->atomo = IF;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "mod") == 0)
        {
            InfoAtomo->atomo = MOD;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "not") == 0)
        {
            InfoAtomo->atomo = NOT;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "number") == 0)
        {
            InfoAtomo->atomo = NUMBER;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "or") == 0)
        {
            InfoAtomo->atomo = OR;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "program") == 0)
        {
            InfoAtomo->atomo = PROGRAM;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "read") == 0)
        {
            InfoAtomo->atomo = READ;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "set") == 0)
        {
            InfoAtomo->atomo = SET;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "then") == 0)
        {
            InfoAtomo->atomo = THEN;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "to") == 0)
        {
            InfoAtomo->atomo = TO;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "true") == 0)
        {
            InfoAtomo->atomo = TRUE;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "while") == 0)
        {
            InfoAtomo->atomo = WHILE;
        }
        else if (strcmp(InfoAtomo->atributo_identificador, "write") == 0)
        {
            InfoAtomo->atomo = WRITE;
        }
        else // determina que nao e reservada e retorna IDENTIFICADOR
            InfoAtomo->atomo = IDENTIFICADOR;
    }
    return;
}

// reconhece comentario multi-linhas
void reconhece_comentario_varias_linhas(TInfoAtomo *InfoAtomo)
{
    cadeia++;
q0:
    if (*cadeia == '-')
    {
        cadeia++;
        goto q1;
    }
    return;
q1:
    if (*cadeia == '-')
    {
        cadeia++;
        goto q2;
    }
    if (*cadeia == '\n')
    {
        linha++;
    }
    else if (*cadeia == 0)
    {
        return;
    }
    cadeia++;
    goto q1;

q2:
    if (*cadeia == '}')
    {
        cadeia++;
        goto q3;
    }
    cadeia++;
    goto q1;
q3:
    InfoAtomo->atomo = COMENTARIO;
}

// reconhece comentario comum
void reconhece_comentario_uma_linha(TInfoAtomo *InfoAtomo)
{
q0:
    if (*cadeia == '\n')
    {
        linha++;
        cadeia++;
        goto q1;
    }
    cadeia++;
    goto q0;
q1:
    InfoAtomo->atomo = COMENTARIO;
}

// reconhece numero ou notacao
void reconhece_numero(TInfoAtomo *InfoAtomo)
{
    int i = 0;
    InfoAtomo->atributo_identificador[i] = *cadeia;
    i++;
    cadeia++;

q0:
    if (isdigit(*cadeia))
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q0;
    }
    else if (*cadeia == 'e')
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q1;
    }
    else if (isalpha(*cadeia))
        return;

    InfoAtomo->atributo_identificador[i] = 0;
    InfoAtomo->atomo = NUMERO;
    return;

q1:
    if (*cadeia == '+' || *cadeia == '-')
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q2;
    }
    if (isdigit(*cadeia))
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q3;
    }
    return;
q2:
    if (isdigit(*cadeia))
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q3;
    }
    return;
q3:
    if (isdigit(*cadeia))
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        cadeia++;
        i++;
        goto q3;
    }
    else if (isalpha(*cadeia))
        return;
    InfoAtomo->atributo_identificador[i] = 0;
    InfoAtomo->atomo = NUMERO;
    return;
}
// reconhece strings de caracteres
void reconhece_caracter(TInfoAtomo *InfoAtomo)
{
    int i = 0;
    cadeia++;
q0:
    if (*cadeia == '\'')
    {
        InfoAtomo->atributo_identificador[i] = 0;
        cadeia++;
        InfoAtomo->atomo = CARACTERE;
        return;
    }
    else
    {
        InfoAtomo->atributo_identificador[i] = *cadeia;
        i++;
        cadeia++;
        goto q0;
    }
}

// rotina le um arquivo e armazena no buffer cadeia
void leArquivo(char *nomeArquivo)
{
    int tamArquivo;
    // abre o arquivo
    FILE *fp;
    fp = fopen(nomeArquivo, "r");
    if (!fp)
    {
        printf("falha na abertura de arquivo.\n");
        exit(1);
    }

    // move ponteiro para o final
    fseek(fp, 0, SEEK_END);
    tamArquivo = ftell(fp); // pega o numero de bytes deslocas
    fseek(fp, 0, SEEK_SET); // move o ponteiro para o inicio
    // aloca um buffer com a quantide de bytes
    cadeia = (char *)calloc(tamArquivo + 1, sizeof(char));

    // le todos os caracteres do arquivo
    fread(cadeia, 1, tamArquivo, fp);
    // printf("%s",cadeia);
    fclose(fp);
}
