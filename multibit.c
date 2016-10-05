

//+++++++++++COMPILATION++++++++++++
// -std=c99 -O3

//+++++++++AUTO EVALUATION++++++++++
/*
chargement de la table 800
lookup 2000
mémoire 1000
-Notre algorithme est assez performant sur l'insertion, un parcours et entre 0 et 4 allocations sont réalisés au maximum.
-La recherche dans l'arbre se fait dans le pire des cas en 5 indirections certains algorithmes sont plus rapide mais cela reste performant 
-Normalement, la consomation mémoire est plutôt importante sur cet algorithme, et est très dépendante de la répartition des routes,
ainsi que du choix des strides. Dans notre cas, nous avons pu optimiser le jeu des strides pour le fichier de routes utilisé,
cependant, un autre fichier de routes avec une répartition différente peux faire augmenter l'utilisation mémoire,
et il faudrait utiliser un autre jeu de strides pour retrouver des performances mémoire acceptables 
(la performance de recherche ne sera pas affecté)

Algorithme multibit à strides fixes par niveau
chaque noeaud stock une gateway, son cidr, ansi que 2^N pointeurs de noeuds fils avec N la stride du niveau.

Optimisation: pour un noeud sans fils (feuille), la gateway et le cidr sont stockés à la place du pointeur du parent
*/

//type d'un pointeur de noeud
#define VIDE 0
#define NODE 1
#define NODE_OPT 2 //optimisation d'un noeud sans fils

//bitfield d'un noeud optimisé
typedef struct __attribute__((__packed__)) NodeOpti {
	unsigned int padding: 24;
	unsigned int cidr: 8;
	unsigned int gw: 32;
}NodeOpti;

//structure contenant un pointeur de noeud OU un noeud optimisé (feuille) ansi qu'un champ type
typedef struct __attribute__((__packed__)) pNode {
	unsigned char type;
	union {
		struct Node* node;
		NodeOpti nodeOpti;
	};
}pNode;

//structure d'un noeud (taille variable)
typedef struct __attribute__((__packed__)) Node {
	unsigned int gw;
	char cidr;
    	pNode childs[];
}Node;


Node* tree = NULL;
//lschar strides[32];

//répartition des strides optimisé pour -03 avec un algorithme génétique
char strides[] = {9,10,3,4,6};



//constructeur de noeud
Node* newNode(char stride, unsigned int gw, char cidr) {
	//calloc permet d'initialiser les pointeurs à null pour le bon fonctionnement de l'algorithme
	Node* temp = calloc((sizeof(unsigned int)+sizeof(char)+sizeof(pNode)*(1<<stride)),1);
	if (temp == NULL) {
		fprintf(stderr, "erreur malloc newNode");
		exit(1);
	}
	temp->gw = gw;
	temp->cidr = cidr;
	return temp;
}

//constructeur de noeud optimisé 
NodeOpti newNodeOpti(unsigned int gw, char cidr) {
	NodeOpti temp;
	temp.gw = gw;
	temp.cidr = cidr;
	return temp;
}

//calcul du CIDR d'un netmask
char getCIDR(unsigned int netmask) {
	char i = 0;
	while(netmask<<i) {
		i++;
	}
	return i;
}

//permet la création de fils dans une range d'index pour stocker une gateway
void setChildsRange(Node* node, unsigned int min, unsigned int max, unsigned int gw, char cidr,char stride) {
	for(unsigned int i = min;i<max;i++) { //itération dans la range d'index
		if(node->childs[i].type == VIDE) { //cas où il n'y a pas de noeud
			//faire l'opti de stockage
			node->childs[i].type = NODE_OPT;
			node->childs[i].nodeOpti = newNodeOpti(gw,cidr);
		}
		else if (node->childs[i].type == NODE_OPT &&
				 node->childs[i].nodeOpti.cidr <= cidr) { //cas où il y a un noeud optimisé et que le cidr du noeud est inferieur -> mise à jour
			node->childs[i].nodeOpti.cidr = cidr;
			node->childs[i].nodeOpti.gw = gw;
		}
		else if (node->childs[i].type == NODE &&
				 node->childs[i].node->cidr <= cidr) {//cas où il y a un pointeur de noeud et que le cidr du noeud sous-jacent est inferieur -> mise à jour
			node->childs[i].node->cidr = cidr;
			node->childs[i].node->gw = gw;
		}
	}
}

void initMyAlgo() {
	tree = newNode(strides[0],0,0);
}

//insertion de routes
void insertMyAlgo(unsigned int addr,unsigned int netmask,unsigned int gw) {
	unsigned char walk = 0; //bits déja traités
	unsigned char deep = 0; //profondeur courante de l'arbre
	unsigned char cidr = getCIDR(netmask);
	Node* local = tree;
	while(1) {
		//extraction de l'index pour le noeud suivant
		unsigned int index = (addr<<walk)>>(32-strides[deep]);
		
		if(walk+strides[deep]>=cidr) { 
			//cas final, il faut ajouter la gateway dans le ou les bonnes entrées du noeud
			unsigned char shift = walk+strides[deep]-cidr;
			unsigned int min = (index>>shift)<<shift;
			unsigned int max = min + (1<<shift);
			setChildsRange(local,min,max,gw,cidr,strides[deep+1]);
			return;
		}
		else {
			//cas intermédiaire, on parcours l'arbre en créant des noeuds si necessaire
			if(local->childs[index].type==VIDE) {//création noeud
				local->childs[index].type = NODE;
				local->childs[index].node = newNode(strides[deep+1],0,0);
			}
			else if (local->childs[index].type == NODE_OPT) {//transformation noeud opti -> neud
				NodeOpti temp = local->childs[index].nodeOpti;
				local->childs[index].type = NODE;
				local->childs[index].node = newNode(strides[deep+1],temp.gw,temp.cidr);
			}
			local = local->childs[index].node;
			walk += strides[deep];
			deep++;

		}
	}
}

unsigned int lookupMyAlgo(unsigned int addr) {
	unsigned char walk = 0;
	unsigned char deep = 0;
	unsigned int bestMatchGw = 0;
	Node* local = tree;
	while(1) {
		//extraction de l'index pour le noeud suivant
		unsigned int index = (addr<<walk)>>(32-strides[deep]);

		if(local->gw != 0) {//mise à jour de la gateway avec le plus long préfixe (certains noeuds peuvent ne pas avoir de gateway même si c'est TRES rare)
			bestMatchGw = local->gw;
		}
		if(local->childs[index].type==VIDE) {//on arrive à la fin de l'arbre
			return bestMatchGw;
		}
		else if (local->childs[index].type == NODE_OPT) {//on arrive à la fin de l'arbre (sur un noeud opti)
			return local->childs[index].nodeOpti.gw;
		}
		else {
			local = local->childs[index].node;
			walk += strides[deep];
			deep++;
		}
	}
}
