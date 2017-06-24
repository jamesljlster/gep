#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#include <galib.h>
#include <gep.h>

#ifdef GA_TYPE
#undef GA_TYPE
#endif
#define GA_TYPE union GEP_NODE

//#define DEBUG
#include <debug.h>

#define CHRO_LEN	500
#define NODE_MAX	30
#define NODE_MIN	-30

#define MUT_RATE	0.4

#define GA_RESERVE	50
#define ITER_COUNT	10000

#define TARGET		1333

void RandChro(GA_TYPE* chro, int chroLen);
double fitness(GA_TYPE* chro, int chroLen, void* arg);

int gep_print_chro(struct GA_POOL* gaPoolPtr, int chroIndex)
{
	int i;
	
	// Checking
	if(chroIndex >= gaPoolPtr->poolSize)
		return -1;
	
	for(i = 0; i < gaPoolPtr->chroLen; i++)
	{
		printf("%lf ", gaPoolPtr->pool[chroIndex][i]);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int i, j;
	int iResult;
	int iterCount;
	int crossIndex;

	double fitLog[ITER_COUNT] = {0};

	struct GA_POOL gaPool;
	
	GA_TYPE tmpValue;
	GA_TYPE tmpChro[2][CHRO_LEN];

	FILE* fileWrite = NULL;

	srand(time(NULL));

	// Create ga pool
	iResult = gep_create(&gaPool, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed\n");
		return -1;
	}

	// Generate random chromosome
	for(i = 0; i < 2; i++)
	{
		RandChro(tmpChro[i], CHRO_LEN);
	}

	// Insert chromosomes to ga pool
	for(i = 0; i < 2; i++)
	{
		iResult = gep_insert(&gaPool, tmpChro[i], CHRO_LEN);
		if(iResult < 0)
		{
			printf("ga_insert() failed\n");
			return -1;
		}
	}
	
	iterCount = 0;
	while(1 /*iterCount < ITER_COUNT*/)
	{
		if(kbhit())
		{
			if(getch() == 27)
				break;
		}

		// Crossover
		crossIndex = gep_crossover(&gaPool, 0, 1, rand() % (CHRO_LEN - 1));
		if(crossIndex < 0)
		{
			printf("ga_crossover() failed!\n");
			return -1;
		}

		// Reproduction
		for(i = 0; i < GA_CROSS_OFFSPRING; i++)
		{
			iResult = gep_reproduction(&gaPool, i + crossIndex);
			if(iResult < 0)
			{
				printf("ga_reproduction failed\n");
				return -1;
			}
		}

		// Mutatuon
		for(i = 0; i < GA_CROSS_OFFSPRING; i++)
		{
			for(j = 0; j < CHRO_LEN; j++)
			{
				if(rand() % 100 < MUT_RATE * 100)
				{
					LOG("Mutation");
					tmpValue.t.value = rand() % (NODE_MAX - NODE_MIN + 1) + NODE_MIN;
					gep_edit_chro(&gaPool, i + crossIndex, j, tmpValue);
				}
			}
		}
			
		// Order
		gep_order(&gaPool, fitness, 0, NULL);

		// Print 1st fitness
		//fitLog[iterCount] = fitness(gaPool.pool[0], CHRO_LEN);
		printf("1st fitness: %lf\n", fitness(gaPool.pool[0], CHRO_LEN, NULL));

		// Kill
		gep_kill_after(&gaPool, GA_RESERVE);

		iterCount++;
	}

	if(argc >= 2)
	{
		fileWrite = fopen(argv[1], "w");
		if(fileWrite != NULL)
		{
			fprintf(fileWrite, "# Fitness log\n");
			for(i = 0; i < ITER_COUNT; i++)
			{
				fprintf(fileWrite, "%lf\n", fitLog[i]);
			}

			fclose(fileWrite);
		}
	}

	return 0;
}

void RandChro(GA_TYPE* chro, int chroLen)
{
	int i;
	for(i = 0; i < chroLen; i++)
	{
		chro[i].t.value = rand() % (NODE_MAX - NODE_MIN + 1) + NODE_MIN;
	}
}

double fitness(GA_TYPE* chro, int chroLen, void* arg)
{
	int i;
	double calcTmp;
	
	calcTmp = chro[0].t.value;
	for(i = 0; i < CHRO_LEN - 1; i++)
	{
		calcTmp += pow(-1, i % 2) * chro[i + 1].t.value;
	}

	return TARGET - fabs(TARGET - calcTmp);
}
