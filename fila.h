#include <stdio.h>

#define MAXFILA 7

struct tpCarro
{
    char placa[9], cor[9], fabric[16], categ, priori;
    int eixos, tempoAtend;
    int tempoEntrada;
};

struct tpFila
{
    int inicio, fim;
    char ativo;
    tpCarro pos[MAXFILA];
};

//Revisar
tpCarro retirar(tpFila &f)
{
    return f.pos[f.inicio++ % MAXFILA];
}

void inicializar(tpFila &f)
{
    f.inicio = 0;
    f.fim = -1;
}

void inserir(tpFila &f, tpCarro carro)
{
    f.pos[++f.fim % MAXFILA] = carro; 
}

char vazia(tpFila f)
{
    return f.fim - f.inicio + 1 <= 0;
}
char cheia(tpFila f)
{
    return f.fim - f.inicio + 1 >= MAXFILA;
}

//EXIBIR PRIMEIRO
tpCarro primeiro(tpFila f)
{
    return f.pos[f.inicio%MAXFILA];
}

void exibir(tpFila f)
{
    tpCarro reg;
    
    while(!vazia(f))
    {
        reg = retirar(f);

        if(reg.categ == 'm')
        {
            printf(" -");
        }else if(reg.categ == 'p')
        {
            printf(" =");
        }else if(reg.categ == 'c')
        {
            printf(" ==");
        }
    }
}

