//
//
// COMPILE:  gcc -Wall -std=c99 -o generate-sets generate-sets.c
//
// USAGE:    ./generate-sets 14 4 8 2 c

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main ( int argc, char *argv[] )
{
    int answers;
    int questions;
    int sets;
    unsigned int seed;
    bool c_source;
    int r;

    if ( argc != 6 )
    {
        fprintf(stderr, "Usage: %s <seed> <answers> <questions> <sets> {c|t}\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ( argv[5][0] != 'c' && argv[5][0] != 't' )
    {
        fprintf(stderr, "Usage: %s <seed> <answers> <questions> <sets> {c|t}\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    seed = atoi(argv[1]);
    answers = atoi(argv[2]);
    questions = atoi(argv[3]);
    sets = atoi(argv[4]);
    c_source = ( argv[5][0] == 'c' ) ? true:false;
    if ( seed==0 || answers==0 || questions==0 || sets==0 )
    {
        fprintf(stderr, "error: invalid numerical parameter\n");
        exit(EXIT_FAILURE);
    }

    if ( c_source )
	printf("    {\n");
    srand(seed);
    for ( int s=0; s<sets; s++)
    {
	if ( c_source )
	    printf("        // set #%d\n        { ",s);
	for ( int q=0; q<questions; q++)
	{
	    r =  (int)rand() % answers + 1;
	    // printf("%d ==> mod4: %d   div: %d\n", r, r%4+1, (int)(((double)r/(double)RAND_MAX)*4.0+0.5) );
	    if ( c_source )
		printf("%d%s",r,(q==(questions-1))?"":",");
	    else
		printf("%d%s",r,(q==(questions-1))?"":";");
	}
	if ( c_source )
	    printf("}%s\n",(s==(sets-1))?"":",");
	else
	    printf("\n");
    }
    if ( c_source )
	printf("    }\n");
    exit(EXIT_SUCCESS);
}
