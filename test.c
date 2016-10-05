#include <stdlib.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <string.h> 
#include <time.h>

#define MAXLEN 100 

#include "MyAlgo.c"

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
int loadTestFile(char *path) 
{
	FILE *fi;
	char s[MAXLEN],*p,*start,ligne=1; 
 	struct in_addr c,t; 
	unsigned int addr,gw,try,errors=0; 


	// first sanity check 
	if (!path) return -1; 
	if ((fi=fopen(path,"r")) == NULL ) return -1; 

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
		while (*p && (*p != '\n')) p++; 
		if (p > start) {
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			gw=htonl(c.s_addr);
		}
		// call your function to insert entry to routing table 
		try = lookupMyAlgo(addr);
		if(try != gw) {
			t.s_addr = htonl(try);
			printf("correctness test fail line %d get :%s ",ligne, inet_ntoa(t));
			t.s_addr = htonl(gw);
			printf("instead of %s\n", inet_ntoa(t));
			
			errors++;
		}
		ligne++;
	}
	fclose(fi); 
	return errors; 
}

// only for debug purpose might be modified ! 
int main (int argc,char *argv[])
{
 	int err;
	clock_t start, end;
 	double cpu_time_used;
 	printf("--- Init algo ---\n");
	initMyAlgo(); 
	printf("--- Load route file \"%s\" ---\n",argv[1]); 
	start = clock();
	if ((argc > 2 ) && loadFile(argv[1])) { 
		end = clock();
 		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 		fprintf(stderr, "temps chargement des routes: %f\n", cpu_time_used);
		printf("--- Run test file \"%s\" +++\n",argv[2]); 
		start = clock();
		err=loadTestFile(argv[2]);
		end = clock();
 		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 		fprintf(stderr, "temps lecture des routes: %f\n", cpu_time_used);
		if(err!=0) {
			printf("Error correctness\n");
		}	 
		else {
			printf("correctness OK\n");
		}
		
		return err;
	}
	return -1; 	 
}