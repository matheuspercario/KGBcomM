/* 	
*	UNICAMP - Universidade Estadual de Campinas
*	FT - Faculdade de Tecnologia
*	Limeira - SP
*	
*	GRUPO - KGBcomM
* 	
*	Matheus Percario Bruder		RA: 222327
*	Kevin Barrios 				RA: 219643
*	Gustavo Tarrafil			RA: 159740
*	
*	Maio/2019
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/* ----------------------------------------------------------------- */
/* Estrutura da informcoes necessaria para referenciar a thread ---- */ 
	typedef struct informacoes
	{
		int ID;
		int Dimensao;
		int num_Threads;
		int Posicao;
		double ** matriz;
		double ** diag1;
		double ** diag2;
		
	} INFORMACOES;

/* ----------------------------------------------------------------- */
/* Variaveis globais: ---------------------------------------------- */ 
	// Pode ser criados até 16 threads 
	pthread_t vThread[16];
	// 16 Struct de cada thread
	INFORMACOES info[16];

/* ----------------------------------------------------------------- */
/* Funcoes globais: retornar linha e coluna ------------------------ */ 
	int linha (int Dimens, int Posic)
	{
		int ID_Linha = Posic/Dimens;
		return ID_Linha;
	}

	int coluna (int Dimens, int Posic)
	{
		int ID_Coluna = Posic % Dimens;	
		return ID_Coluna;
	}

/* ----------------------------------------------------------------- */
/* Funcao DividirMatriz da thread ---------------------------------- */
	/* ----------------------------------------------------------------- */
	void * threadDividirMatriz(void * Argumentos){
		
		INFORMACOES * info = Argumentos;
		
		while (info->Posicao < (info->Dimensao * info->Dimensao)){
			int LIN = linha(info->Dimensao, info->Posicao);
			int COL = coluna(info->Dimensao, info->Posicao);
			
			double Posicao = info->matriz[LIN][COL];
			
			if 	(COL >= LIN) 	{ info->diag2[LIN][COL] = info->matriz[LIN][COL]; info->diag1[LIN][COL] = 0.0; }
				else 		{ info->diag1[LIN][COL] = info->matriz[LIN][COL]; info->diag2[LIN][COL] = 0.0; }
				
			//printf("Posicao Atual thread[%d]: %d\n", info->ID, info->Posicao);
			info->Posicao = info->num_Threads + info->Posicao;
			//printf("Proxima pos. thread[%d]: %d\n", info->ID, info->Posicao);		

		}
	}

/* ----------------------------------------------------------------- */
/* Funcao Principal ------------------------------------------------ */
/* ----------------------------------------------------------------- */
	int main (int argc, char * argv[])
	{
		int i, j, N, T;

		N = atoi(argv[1]);
		T = atoi(argv[2]);

        clock_t tempoInicial;
        clock_t tempoFinal;
        double tempoExecucao;

		printf("Dimensão da matriz: %d\n", N);
		printf("Numero de threads: %d\n", T);
		printf("Arquivo: %s\n\n", argv[3]);
		
		if(T==2 || T==4 || T==8 || T==16)
		{
		/* ----------------------------------------------------------------- */
		/* Alocar TODAS matrizes dinamicamente ----------------------------- */ 
			//OBS.: matrizes foram alocadas dinamicamente para facilitar
			//a passagem por parametro dentro da struct (alem de evitar
			//estouro de pilha)!	
		
			// Malloc - matriz principal
			double **matriz_p;
			matriz_p = (double **) malloc (N * sizeof(double *));
			for (i = 0; i < N; i++)
			{
				matriz_p[i] = (double *) malloc (N * sizeof(double));
			} 
		
			// Malloc - matriz diag1
			double **diag1;
			diag1 = (double **) malloc (N * sizeof(double *));
			for (i = 0; i < N; i++)
			{
				diag1[i] = (double *) malloc (N * sizeof(double));
			} 
		
			// Malloc - matriz diag2
			double **diag2;
			diag2 = (double **) malloc (N * sizeof(double *));
			for (i = 0; i < N; i++)
			{
				diag2[i] = (double *) malloc (N * sizeof(double));
			} 
		
		/* ----------------------------------------------------------------- */
		/* Escanear Matriz.txt gerada aleatoriamente! ---------------------- */ 	
			FILE * arq;
			arq = fopen(argv[3], "r");
			if (arq == NULL) { printf("ERRO ao abrir o arquivo!\n\n"); }
		
			for (i = 0; i < N; i++)
			{
				for (j = 0; j < N; j++)
				{
					fscanf(arq, "%lf", &matriz_p[i][j]);
				}
			}
			fclose(arq);

        /* ----------------------------------------------------------------- */
		/* Criacao das threads! -------------------------------------------- */ 
            //Sinalização do começo	
            tempoInicial = clock();    
			// Criar numero de threads necessarias e passar
			//informacoes para funcao threads
			for (i = 0; i < T; i++)
			{
				//info struct criada em cima
				info[i].ID = i;
				info[i].Dimensao = N;
				info[i].num_Threads = T;
				info[i].Posicao = i;
				info[i].matriz = matriz_p;
				info[i].diag1 = diag1;
				info[i].diag2 = diag2;
			
				// Criando a thread e passando referencias
				//referencias:
				//	&vThread[i]: Qual é a thread dentre o vetor de threads
				//	threadDividirMatriz: Funcao da thread
				//	(void *)&info[i]: Endereco da struct referente a thread 
				pthread_create(&vThread[i], NULL, threadDividirMatriz, (void *)&info[i]);
			}
		
		
			for (i = 0; i < T; i++)
			{
				// "pthread_join" faz que a main espere que
				//todas threads acabarem
				pthread_join(vThread[i], NULL);
			}
            //Sinalizacao do final
            tempoFinal = clock();
            //Tempo total de execucao
            tempoExecucao = (tempoFinal - tempoInicial) * 1000.0 / CLOCKS_PER_SEC;
            printf("\nTempo de execucao: %.3lf ms\n", tempoExecucao);

		/* ----------------------------------------------------------------- */
		/* Printar matriz diag2 no arquivo txt! ---------------------------- */
			arq = fopen("Matriz.diag2", "w");
			if (arq == NULL) { printf("ERRO ao abrir o arquivo!\n"); }
			
			for (i = 0; i < N; i++) 
			{
				for (j = 0; j < N; j++) 
				{
					fprintf(arq, "%lf\t", diag1[i][j]);
				}
				fprintf(arq, "\n");
			}
			fclose(arq);

		/* ----------------------------------------------------------------- */
		/* Printar matriz diag1 no arquivo txt! ---------------------------- */
			arq = fopen("Matriz.diag1", "w");
			if (arq == NULL) { printf("ERRO ao abrir o arquivo!\n"); }
			
			for (i = 0; i < N; i++) 
			{
				for (j = 0; j < N; j++) 
				{
					fprintf(arq, "%lf\t", diag2[i][j]);
				}
				fprintf(arq, "\n");
			}
			fclose(arq);
			}
		/* ----------------------------------------------------------------- */
		/* Numero de threads invalido!  ------------------------------------ */
		else
		{
			printf("Numero de threads invalido!\n\n");
			return 1;
		}

	return 0;
	}
