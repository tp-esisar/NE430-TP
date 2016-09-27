#include <stdint.h>



typedef struct Node {
	unsigned int gw;
    struct Node* childs[];

}Node;

Node* tree = NULL;
//char strides[32];
char strides[] = {16,8,4,2,2};


void initMyAlgo() {}

Node* newNode(char stride, unsigned int gw) {
	Node* temp = calloc((sizeof(unsigned int)+sizeof(Node*)*(1<<stride)),1);
	if (temp == NULL) {
		fprintf(stderr, "erreur malloc newNode");
		exit(1);
	}
	temp->gw = gw;
	return temp;
}

char getCIDR(unsigned int netmask) {
	char i = 1;
	while(netmask<<i) {
		i++;
	}
	return i;
}

void setChildsRange(Node* node, unsigned char min, unsigned char max, unsigned int gw, unsigned char deep) {
	Node* temp = NULL;
	for(unsigned char i = min;i<max;i++) {
		if(node->childs[i]==NULL) {
			if(temp == NULL) {
				temp = newNode(strides[deep],gw);
			}
			node->childs[i] = temp;
		}
	}
}

void insertMyAlgo(unsigned int addr,unsigned int netmask,unsigned int gw) {
	unsigned char walk = 0;
	unsigned char deep = 0;
	unsigned char cidr = getCIDR(netmask);
	if(tree == NULL) {
		tree = newNode(strides[0],0);
	}
	Node* local = tree;
	while(1) {
		unsigned char index = (addr<<walk)>>(32-strides[deep]);
		if(walk+strides[deep]>cidr) {
			//cas merdique final
			unsigned char shift = walk+strides[deep]-cidr;
			unsigned char min = (index>>shift)<<shift;
			unsigned char max = min + (1<<shift);
			setChildsRange(local,min,max,gw,deep);
			return;
		}
		else if(walk+strides[deep]==cidr){
			//cas simple final
			if(local->childs[index]==NULL) {
				local->childs[index] = newNode(strides[deep+1],gw);
			}
			else {
				local->childs[index]->gw = gw;
			}
			return;
		}
		else {
			//cas intermédiaire
			if(local->childs[index]==NULL) {
				local->childs[index] = newNode(strides[deep+1],0);
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
	Node* local = tree;
	if(tree == NULL) {
		return 0;
	}

	while(1) {
		unsigned char index = (addr<<walk)>>(32-strides[deep]);
		if(local->childs[index]==NULL) {
			return local->gw;
		}
		else {
			local = local->childs[index];
			walk += strides[deep];
			deep++;
		}
	}
}