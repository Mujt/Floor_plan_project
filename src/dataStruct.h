#pragma once
#include <string>
#include <vector>

enum blockType
{
	SoftBlock = 0,
	HardBlock,
	Terminals
};



typedef struct Coordinate {
	int x, y;
};

typedef struct Block { 
	int idNum;
	std::string id;//����
	blockType type;//����

	int coordNum; //��״
	Coordinate coords[4];

	
	Block() {
		coordNum = 0;
		for (int i = 0; i < 4; i++) {
			coords[i].x = -1;
			coords[i].y = -1;
		}
	}
}Block;

typedef struct Block_tmp {
	int idNum;
	double n1, n2;//����
	Block_tmp() {
		idNum = -1;
		n1 = 0;
		n2 = 0;
	}
}Block_tmp;

typedef struct Net {
	int id;
	int netDegree;
	std::vector<Block_tmp> blocks;
}Net;