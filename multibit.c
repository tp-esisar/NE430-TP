#include <stdint.h>

#define VIDE 0
#define NODE 1
#define NODE_OPT 2
typedef struct __attribute__((__packed__)) NodeOpti {
	unsigned int padding: 24;
	unsigned int cidr: 8;
	unsigned int gw: 32;
}NodeOpti;

typedef struct __attribute__((__packed__)) pNode {
	unsigned char type;
	union {
		struct Node* node;
		NodeOpti nodeOpti;
	};
}pNode;

typedef struct __attribute__((__packed__)) Node {
	unsigned int gw;
	char cidr;
    pNode childs[];
}Node;



Node* tree = NULL;
//char strides[32];
char strides[] = {16,8,4,2,2};


void initMyAlgo() {}

Node* newNode(char stride, unsigned int gw, char cidr) {
	Node* temp = calloc((sizeof(unsigned int)+sizeof(char)+sizeof(pNode)*(1<<stride)),1);
	if (temp == NULL) {
		fprintf(stderr, "erreur malloc newNode");
		exit(1);
	}
	temp->gw = gw;
	temp->cidr = cidr;
	return temp;
}
NodeOpti newNodeOpti(unsigned int gw, char cidr) {
	NodeOpti temp;
	temp.gw = gw;
	temp.cidr = cidr;
	return temp;
}

char getCIDR(unsigned int netmask) {
	char i = 0;
	while(netmask<<i) {
		i++;
	}
	return i;
}

void setChildsRange(Node* node, unsigned int min, unsigned int max, unsigned int gw, char cidr,char stride) {
	for(unsigned int i = min;i<max;i++) {
		if(node->childs[i].type == VIDE) {
			//faire l'opti de stockage
			node->childs[i].type = NODE_OPT;
			node->childs[i].nodeOpti = newNodeOpti(gw,cidr);
		}
		else if (node->childs[i].type == NODE_OPT &&
				 node->childs[i].nodeOpti.cidr <= cidr) {
			node->childs[i].nodeOpti.cidr = cidr;
			node->childs[i].nodeOpti.gw = gw;
		}
		else if (node->childs[i].type == NODE &&
				 node->childs[i].node->cidr <= cidr) {
			node->childs[i].node->cidr = cidr;
			node->childs[i].node->gw = gw;
		}
	}
}

void insertMyAlgo(unsigned int addr,unsigned int netmask,unsigned int gw) {
	unsigned char walk = 0;
	unsigned char deep = 0;
	unsigned char cidr = getCIDR(netmask);
	if(tree == NULL) {
		tree = newNode(strides[0],0,0);
	}
	Node* local = tree;
	while(1) {
		unsigned int index = (addr<<walk)>>(32-strides[deep]);
		if(walk+strides[deep]>=cidr) {
			//cas merdique final
			unsigned char shift = walk+strides[deep]-cidr;
			unsigned int min = (index>>shift)<<shift;
			unsigned int max = min + (1<<shift);
			setChildsRange(local,min,max,gw,cidr,strides[deep+1]);
			return;
		}
		else {
			//cas intermédiaire
			if(local->childs[index].type==VIDE) {
				local->childs[index].type = NODE;
				local->childs[index].node = newNode(strides[deep+1],0,0);
			}
			else if (local->childs[index].type == NODE_OPT) {
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
	if(tree == NULL) {
		return 0;
	}
	while(1) {
		unsigned int index = (addr<<walk)>>(32-strides[deep]);

		if(local->gw != 0) {
			bestMatchGw = local->gw;
		}
		if(local->childs[index].type==VIDE) {
			return bestMatchGw;
		}
		else if (local->childs[index].type == NODE_OPT) {
			return local->childs[index].nodeOpti.gw;
		}
		else {
			local = local->childs[index].node;
			walk += strides[deep];
			deep++;
		}
	}
}
