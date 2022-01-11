#include "Game.h"
#include <fstream>
#include <chrono>
using namespace std::chrono;

struct hashFunct
{
	long operator()(const Node* node) const
	{
		int p = node->size * node->size;
		long hsh = 0;
		for (int i = 0; i < node->size; i++)
		{
			for (int j = 0; j < node->size; j++)
			{
				int x = node->state[i][j];
				hsh = p * hsh + x;
			}
		}
		return hsh;
	}
};

struct equal
{
	bool operator ()(Node* x, Node* y) const
	{
		for (size_t i = 0; i < x->size; i++)
		{
			for (size_t j = 0; j < x->size; j++)
			{
				if (y->state[i][j] != x->state[i][j])
					return false;
			}
		}
		return true;
	}
};

void assignSons(Game& game, Node* parent, std::priority_queue<Node*, std::vector<Node*>, Game>& q, std::unordered_map<Node*, double, hashFunct, equal>& hashMap,std::vector<std::string>& v )
{
	Node* newNode;
	hashFunct h;

	for (std::string s : v)
	{
		newNode = game.createNode(parent, s);
		if (newNode != nullptr)
		{
			if (hashMap.find(newNode) != hashMap.end())
			{
				if (newNode->cost + game.Manhattan(newNode) < hashMap[newNode])
				{
					hashMap[newNode] = newNode->cost + game.Manhattan(newNode);
					q.push(newNode);
				}
				else
				{
					newNode->dealloc();
					delete newNode;
				}
			}
			else
			{
				hashMap[newNode] = newNode->cost + game.Manhattan(newNode);
				q.push(newNode);
			}
		}
	}
}

void hashFct(int a[][3], int n)
{
	long hsh = 0, sum = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			hsh = (n * n) * hsh + a[i][j];
		}
	}
	std::cout << hsh << "\n";
}

void A_star(Game& game, Node* node, std::priority_queue<Node*, std::vector<Node*>, Game>& q, std::unordered_map<Node*, double, hashFunct, equal>& hashMap)
{
	std::vector<std::string> v;
	v.resize(4);
	v[0] = "left";
	v[1] = "right";
	v[2] = "up";
	v[3] = "down";

	q.push(node);
	hashMap[node] = node->cost + game.Manhattan(node);
	assignSons(game, node, q, hashMap,v);

	while (!q.empty())
	{
		Node* newNode = q.top();
		q.pop();

		assignSons(game, newNode, q, hashMap,v);

		if (game.isSolved(newNode))
		{
			int states = 0;
			game.print(newNode,states);
			std::cout << "Pasi: " << states;
			std::cout << "\n";
			return;
		}
	}
}
int main()
{
	std::ifstream f("dat.txt");
	Game game;

	std::priority_queue<Node*, std::vector<Node*>, Game> q;

	std::unordered_map<Node*, double, hashFunct, equal> hashMap;
	f >> game;
	int size = game.size();
	Node* node = new Node();
	node->state = new int* [size];
	for (size_t i = 0; i < 3; i++)
		node->state[i] = new int[size];

	node->size = size;
	node->blank_coord = game.blank_coord;
	node->v = game.coord;
	
	node->state = game.tiles;

	auto start = high_resolution_clock::now();

	A_star(game, node, q, hashMap);
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start);
	std::cout << "Duration: " << duration.count() << "\n";

	std::cout << "States: " << hashMap.size();
	return 0;
}

