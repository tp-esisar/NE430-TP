//TODO lancer une série de recherche aléatoire pour tester l'efficacité du merdier avec time
//sera utilié par l'algo génétique pour noter les individus

#include <stdlib.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <string.h> 
#include <time.h>

#define NBADRESSE 1000000000 
#define MAXLEN 100 
#include "multibit.c"

// This function must not be modified !!! 
int loadFile(char *path) 
{
	FILE *fi;
	char s[MAXLEN],*p,*start; 
 	struct in_addr c; 
	unsigned int addr,netmask,gw; 

	// first sanity check 
	if (!path) return 0; 
	if ((fi=fopen(path,"r")) == NULL ) return 0; 

	// read file line by line (max = MAXLEN ) 
	while (fgets(s,MAXLEN,fi) != NULL) {
		s[MAXLEN]=0; 
		p=start=s; 
		// seek for blank char
		while (*p && (*p != ' ')) p++; 
		if (p > start) {
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			addr=htonl(c.s_addr);  			
		}
		// skip remaining blank char 
		while (*p && (*p == ' ')) p++;

		// stat a new search for blank char 
		start=p; 
		while (*p && (*p != ' ')) p++; 
		if (p > start) {
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			netmask=htonl(c.s_addr);
		}		
		// skip remaining blank char 
		while (*p && (*p == ' ')) p++;

		// stat a new search for blank '\n' char 		
		start=p; 
		while (*p && (*p != '\n')) p++; 
		if (p > start) { 
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			gw=htonl(c.s_addr);
		}

		// call your function to insert entry to routing table 
		insertMyAlgo(addr,netmask,gw); 	
	}
	fclose(fi); 
	return 1; 
}

int main (int argc,char *argv[])
{
	int i, nbMiss=0;
	unsigned int addr;

	if (argc <= 1)
		return -1;

	for (i=1; i<argc; i++)
		strides[i-1]=atoi(argv[i]);
	if (loadFile("routes") != 1)
		return (-1);

	initMyAlgo(); 
	srand(time(NULL));
	printf("IP lookup algo\n");

	for (i=0; i<NBADRESSE; i++) {
		addr = rand() % 4294967296;
		if (lookupMyAlgo(addr) == 0)
			nbMiss++;
	}
	printf("%d tentatives, %d echouée", NBADRESSE, nbMiss);

	return 0; 	 
}

