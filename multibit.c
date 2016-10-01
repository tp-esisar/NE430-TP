#include <stdint.h>



typedef struct Node {
	unsigned int gw;
	char cidr;
    struct Node* childs[];

}Node;

Node* tree = NULL;
//char strides[32];
char strides[] = {16,8,4,2,2};


void initMyAlgo() {}

Node* newNode(char stride, unsigned int gw, char cidr) {
	Node* temp = calloc((sizeof(unsigned int)+sizeof(char)+sizeof(Node*)*(1<<stride)),1);
	if (temp == NULL) {
		fprintf(stderr, "erreur malloc newNode");
		exit(1);
	}
	temp->gw = gw;
	temp->cidr = cidr;
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
		if(node->childs[i] == NULL) {
			node->childs[i] = newNode(stride,gw,cidr);
		}
		else if (node->childs[i]->cidr <= cidr) {
			node->childs[i]->cidr = cidr;
			node->childs[i]->gw = gw;
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
			if(local->childs[index]==NULL) {
				local->childs[index] = newNode(strides[deep+1],0,0);
			}
			local = local->childs[index];
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
		if(local->childs[index]==NULL) {
			return bestMatchGw;
		}
		else {
			local = local->childs[index];
			walk += strides[deep];
			deep++;
		}
	}
}
