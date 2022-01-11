#pragma once
#include <iostream>
#include "Game.h"
#include <vector>
struct Node
{
	int** state;
	int** finish;
	int size;
	std::vector<std::pair<int, int>>* v;
	Node* parent;
	std::string action;
	double cost;
	std::pair<int, int> blank_coord;
	Node()
	{
		parent = nullptr;
		action = "";
		cost = INT_MAX;
	}

	void dealloc()
	{
		for (size_t i = 0; i < size; i++)
			delete[] state[i];
		delete[] state;
	}

	friend std::ostream& operator<<(std::ostream& os,Node* node)
	{
		for (size_t i = 0; i < node->size; i++)
		{
			for (size_t j = 0; j < node->size; j++)
			{
				os << node->state[i][j] << " ";
			}
			os << "\n";
		}
		os << "\n";
		return os;
	}
};