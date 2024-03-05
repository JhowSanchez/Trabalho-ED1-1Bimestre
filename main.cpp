#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <conio2.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

//TAD
#include "fila.h"
#define MAXCABINES 5
#define MULT_TEMPO_ATEND 1

//PROTOTIPOS
tpCarro lerCarro(FILE *ptr);
int menorCab(tpFila f[MAXCABINES]);
void exibirTudo(tpFila fila[MAXCABINES],int totP, int totM, int totC, int totSM, int totSC, int totSP, int somaTempoEspera, int somaTempoAtend, int unidTempo, int &l, int carroHora);
void quadro1();
void quadro2();
void limpar(int &l);
void nl(int &l);

//MAIN
int main (void){
    FILE *ptr = fopen("carros.txt","r");

    tpFila fila[MAXCABINES];
    tpCarro reg;
    int tempo[MAXCABINES], unidTempo = 0, clockInicial, aux, tempoInsereCarro, somaTempoEspera=0, somaTempoAtend=0;
    int totC =0, totM =0, totP =0, totSM = 0,totSC =0, totSP =0, l=0, carroHora = 0;
    float delay;
    char op, inserido = true;
    quadro1();
    textcolor(3);
    gotoxy(5,3); printf("SIMULACAO DE FILAS EM UM PEDAGIO");
    textcolor(15);
    nl(l); printf("Informe o tempo delay do funcionamento do programa: ");
    scanf("%f",&delay);
    nl(l); printf("Informe o tempo de espera de entrada de cada Carro: ");
    scanf("%d",&tempoInsereCarro);
    nl(l); printf("Informe quantas cabines estarao ativas :");
    scanf("%d",&aux);
    limpar(l);
    textcolor(3);
    quadro2();
    textcolor(15);

    // INICIALIZANDO FILAS
    for(int i = 0; i < MAXCABINES-1; i++)
    {
        if(i < aux)
        {
            fila[i].ativo = true;
        }
        else
        {
            fila[i].ativo = false;
        }
        tempo[i] = 0;
        inicializar(fila[i]);
    }

    // INICIALIZANDO FILA SEM PARAR !!!
    tempo[MAXCABINES-1] = 0;
    inicializar(fila[MAXCABINES-1]);
    fila[MAXCABINES-1].ativo = true;

    clockInicial = clock();
    do
    {
        //CLOCK FUNC PARA PEGAR SEGUNDOS RODADOS NO INICIO DO PROGRAMA
        do
        {
    
            if(clock() >= clockInicial + unidTempo * CLOCKS_PER_SEC*delay)
            {
                //UNIDTEMPO E O SEGUNDOS
                unidTempo++;
                for(int i=0;i<MAXCABINES-1;i++)
                {
                    if(!vazia(fila[i]) && unidTempo >= tempo[i] + primeiro(fila[i]).tempoAtend*MULT_TEMPO_ATEND)
                    {
                        somaTempoEspera += unidTempo - primeiro(fila[i]).tempoEntrada;
                        
                        somaTempoAtend += primeiro(fila[i]).tempoAtend*MULT_TEMPO_ATEND;

                        if(primeiro(fila[i]).categ == 'c')
                        {
                           totC++; 
                        } 
                        else if(primeiro(fila[i]).categ == 'p')
                        {
                            totP++; 
                        }
                        else if(primeiro(fila[i]).categ == 'm')
                        {
                        	totM++;   
                        }
                        carroHora++;
                        retirar(fila[i]).tempoEntrada;
                        if(!vazia(fila[i]))
                        {
                        	tempo[i] = unidTempo;
                        }
                    }
                }
                
                if(!vazia(fila[MAXCABINES-1]))
                {
                    if(primeiro(fila[MAXCABINES-1]).categ == 'c')
                    {
                        totSC++;
                    }
                    if(primeiro(fila[MAXCABINES-1]).categ == 'm')
                    {
                        totSM++;
                    }
                    if(primeiro(fila[MAXCABINES-1]).categ == 'p')
                    {
                        totSP++;
                    }
                	retirar(fila[MAXCABINES-1]);
                	carroHora++;
                    
                }
                
                if(feof(ptr))
                {
                	rewind(ptr);
                }
                
                if(unidTempo%tempoInsereCarro == 0)
                {
                    if(!feof(ptr) && inserido)
                    {
                        reg = lerCarro(ptr);
                        inserido = false;
                    }
                    
                    if(!inserido)
                    {
                        if(reg.priori == 's')
                        {
                            inserir(fila[MAXCABINES-1],reg);
                            inserido = true;
                        }
                        else
                        {
                            aux = menorCab(fila);

                            if(aux != -1)
                            {
                                reg.tempoEntrada = unidTempo;
                                if(vazia(fila[aux]))
                                {
                                	tempo[aux] = unidTempo;
                                }
                                inserir(fila[aux],reg);
                                inserido=true;
                            }
                        }
                    }
                }
                if(unidTempo%60 == 0)
                {
                	carroHora = 0;
                }
                exibirTudo(fila, totP, totM, totC, totSM, totSC, totSP, somaTempoEspera, somaTempoAtend, unidTempo, l, carroHora);
            }
        } while (!kbhit());
        
        op = getch();
        if(op >= '0' && op < '0'+MAXCABINES-1)
        {
            if(fila[op-'0'].ativo)
            {
                fila[op-'0'].ativo = false;
            }
            else
            {
                fila[op-'0'].ativo = true;
            }
        	exibirTudo(fila, totP, totM, totC, totSM, totSC, totSP, somaTempoEspera, somaTempoAtend, unidTempo, l, carroHora);
        }
    } while (op != 27);
    
    fclose(ptr);
    return 0;
}


//FUNCOES
tpCarro lerCarro(FILE *ptr)
{
    tpCarro reg;
    fscanf(ptr,"%[^;];%[^;];%[^;];%d;%c;%c;%d\n",&reg.placa,&reg.cor,&reg.fabric,&reg.eixos,&reg.categ,&reg.priori,&reg.tempoAtend);
    return reg;
}

int menorCab(tpFila f[MAXCABINES])
{
    int menorFila = -1, menorQtdeCarros=MAXFILA;

    for(int i=0;i<MAXCABINES-1;i++)
    {
        if(f[i].ativo && !cheia(f[i]) && menorQtdeCarros > f[i].fim-f[i].inicio+1)
        {
            menorQtdeCarros = f[i].fim-f[i].inicio+1;
            menorFila = i;
        }
    }
    return menorFila;
}

void exibirTudo(tpFila fila[MAXCABINES],int totP, int totM, int totC, int totSM, int totSC, int totSP, int somaTempoEspera, int somaTempoAtend, int unidTempo, int &l, int carroHora)
{
	tpCarro r;
    limpar(l);
    nl(l); printf(" Tempo: %d",unidTempo);
    nl(l);
    for(int i=0;i<MAXCABINES;i++)
    {
    	if(i == MAXCABINES-1)
    	{
    		nl(l);
    	}
        if(!fila[i].ativo)
        {
            textcolor(4);
        }
    	nl(l); printf("  [%d] ",i);
        textcolor(15);
        exibir(fila[i]);
        
        if(!vazia(fila[i]))
        {
        	r = primeiro(fila[i]);
        	if(i < MAXCABINES-1)
        		gotoxy(37,i+7);
        	else
        	{
        		gotoxy(37,12);
        	}
        	printf("[%d] %s - %s - %s - eixos: %d, categ: %c, priori: %c, atend: %d",i,r.placa,r.cor,r.fabric,r.eixos,r.categ,r.priori,r.tempoAtend);
        }
    }
    nl(l);
    nl(l);
    nl(l);
    textcolor(15);
    if(totC+totM+totP > 0)
    {
        nl(l); printf(" B) O tempo medio de espera para saida do veiculo do pedagio: %.2f",(float)somaTempoEspera/(totC+totM+totP));
    }
    else
    {
        nl(l); printf(" B) O tempo medio de espera para saida do veiculo do pedagio: 0.00");
    }
    nl(l);

    if(totC+totM+totP > 0)
    {
        nl(l); printf(" C) O tempo medio de espera em atendimento: %.2f",(float) somaTempoAtend/(totC+totM+totP));  
    }
    else
    {
        nl(l); printf(" C) O tempo medio de espera em atendimento: 0"); 
    }
    nl(l);

    nl(l); printf(" D) Tipos de veiculos que pagam:  ");
    printf("     Motos: %04d",totM);
    printf("     Carros: %04d",totP);
    printf("     Caminhao: %04d",totC);
    
    nl(l);

    nl(l); printf(" E) Veiculos sem parar por hora:  ");
    if(unidTempo > 0)
    {    
        //nl(l); printf("Total de Veiculos por horas/minutos no Sem-Parar: %.2f",(float)(totSM+totSP+totSC)/unidTempo *60);

        printf("     Motos: %.2f",(float)totSM/unidTempo * 60);
        printf("     Carros: %.2f",(float)totSP/unidTempo * 60);
        printf("     Caminhao: %.2f",(float)totSC/unidTempo * 60);
    }
    else
    {
        //nl(l); printf("Total de Veiculos por horas/minutos no Sem-Parar: 0");

        nl(l); printf("     Motos Sem-Parar por hora: 0.00");
        nl(l); printf("     Total de Carros Sem-Para: 0.00");
        nl(l); printf("     Total de Caminhao Sem-Para: 0.00");
    }
    
    nl(l);

    //nl(l); printf("Total de Motos Sem-Para: %d",totSM);
    //nl(l); printf("Total de Carros Sem-Para: %d",totSP);
    //nl(l); printf("Total de Caminhao Sem-Para: %d",totSC);

    if(unidTempo > 0)
    {    
        nl(l); printf(" F) O numero de veiculos que passam pelo pedagio na hora %d: %d",unidTempo/60 ,carroHora);
    }
    else
    {
        nl(l); printf(" F) O numero de veiculos que passam pelo pedagio na hora 0: 0");
    }
    nl(l);
    nl(l);
    nl(l);
    nl(l); printf(" Moto: -     Carro: =     Caminhao: ==");
}

void quadro1()
{
    for(int i=4;i<118;i++)
    {
        gotoxy(i,2);
        printf("%c",196);
        gotoxy(i,4);
        printf("%c",196);
        gotoxy(i,29);
        printf("%c",196);
    }
    for(int i=3;i<29;i++)
    {
    	gotoxy(3,i);
    	printf("%c",179);
    	gotoxy(118,i);
    	printf("%c",179);
    }
    
    gotoxy(118,2);
    printf("%c",191);
    
    gotoxy(118,29);
    printf("%c",217);
    
    gotoxy(3,2);
    printf("%c",218);
    
    gotoxy(3,29);
    printf("%c",192);
    
    gotoxy(3,4);
    printf("%c",195);
    
    gotoxy(118,4);
    printf("%c",180);
    
    gotoxy(120,30);
}

void quadro2()
{
	for(int i=5;i<117;i++)
    {
        gotoxy(i,14);
        printf("%c",196);
        gotoxy(i,27);
        printf("%c",196);
    }
    
    for(int i=5;i<14;i++)
	{
		gotoxy(35,i);
		printf("%c",179);
	}
	
	gotoxy(35,14);
    printf("%c",193);
}

void limpar(int &l)
{
	for(int i=5;i<l+5;i++)
	{
		
		if (i != 14 && i != 27)
		{
			gotoxy(5,i);
			if(i < 14)
			{
				printf("                              ");
				gotoxy(36,i);
				printf("                                                                                 ");
			}
			else
			{
				printf("                                                                                                                ");
			}
		}
	}
	l = 0;
}

void nl(int &l)
{
	gotoxy(5,l+5);
	l++;
}
