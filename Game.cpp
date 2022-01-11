#include "Game.h"

int Game::row_from_bottom()
{
	for (int i = n - 1; i >= 0; i--)
	{
		for (int j = 0; j < n; j++)
		{
			if (tiles[i][j] == 0)
				return n - i;
		}
	}
	return 0;
}

int Game::size()
{
	return n;
}

int Game::Manhattan(Node* node)
{
	//The  sum  of  the  distances  of  the  tiles  from  their  goal  positions.
	//Because  tiles cannot move along diagonals,  the distance we will count
	//is the sum of the horizontal and vertical distances. 
	//abs(r-i) + abs(c-j)
	int sum = 0;
	for (int i = 0; i < node->size; i++)
	{
		for (int j = 0; j < node->size; j++)
		{
			std::pair<int,int> x = node->v->at(node->state[i][j]);
			sum += (abs(x.first - i) + abs(x.second - j));
		}
	}
	return sum;
}
std::pair<int, int> Game::BinarySearch(Node* node, int row, int col, int x)
{
	if ((row < 0 || col < 0))
		return std::make_pair(0, 0);
	if (x == node->finish[row][col])
		return std::make_pair(row, col);
	if (x > node->finish[row][col] && x > node->finish[row][node->size - 1])
		return BinarySearch(node, ++row, node->size - 1, x);
	else
		if (x > node->finish[row][col] && x <= node->finish[row][node->size - 1])
			return BinarySearch(node, row, col + 1, x);

	return BinarySearch(node, row, col / 2 - 1, x);
}
double Game::calculate_cost(Node* x)
{
	double cost = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			if (x->state[i][j] != finish[i][j])
				cost++;
		}
	}
	return cost;
}
Game::Game()
{
}

bool Game::solvable(Node* state)
{
	if ((state->size % 2 == 1 && inversions(state) % 2 == 0) || (state->size % 2 == 0 && (row_from_bottom() % 2 == 1 && inversions(state) % 2 == 0)))
		return true;
	return false;
}

int Game::inversions(Node* state)
{
	int n = state->size, inversiuni = 0;
	int* v = new int[n * n];
	for (int i = 0; i < n; i++)
	{
		v[i] = state->state[i / n][i % n];
		std::cout << v[i] << " ";
	}

	int* temp = new int[n];
	return mergeSort(v, temp, 0, (n * n) - 1);
	delete[] v;
}

int Game::inversions()
{
	int  inversiuni = 0;
	int* v = new int[n * n], k = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			v[k++] = tiles[i][j]; //std::cout << v[k - 1];
		}

	int* temp = new int[n * n];
	int x = mergeSort(v, temp, 0, (n * n) - 1);
	delete[] v;
	delete[] temp;
	return x;
}

bool Game::isSolved(Node* state)
{
	if (state->cost == 0)
		return true;
	return false;
}

Node* Game::createNode(Node* parent, std::string action)
{
	Node* node = new Node();

	node->size = parent->size;
	node->parent = parent;
	node->state = new int* [node->size];
	node->blank_coord = parent->blank_coord;
	for (int i = 0; i < node->size; i++)
		node->state[i] = new int[node->size];

	for (size_t i = 0; i < node->size; i++)
		for (size_t j = 0; j < node->size; j++)
			node->state[i][j] = parent->state[i][j];

	node->action.assign(action);
	node->cost = calculate_cost(node);
	node->finish = parent->finish;
	node->v = parent->v;
	if (action == "left")
	{
		if (node->blank_coord.second == 0)
			return nullptr;
		std::swap(node->state[node->blank_coord.first][node->blank_coord.second], node->state[node->blank_coord.first][node->blank_coord.second - 1]);
		node->blank_coord.second--;
	}
	else
		if (action == "right")
		{
			if (node->blank_coord.second == node->size - 1)
				return nullptr;
			std::swap(node->state[node->blank_coord.first][node->blank_coord.second], node->state[node->blank_coord.first][node->blank_coord.second + 1]);
			node->blank_coord.second++;
		}
		else if (action == "up")
		{
			if (node->blank_coord.first == 0)
				return nullptr;
			std::swap(node->state[node->blank_coord.first][node->blank_coord.second], node->state[node->blank_coord.first - 1][node->blank_coord.second]);
			node->blank_coord.first--;
		}
		else
			if (action == "down")
			{
				if (node->blank_coord.first == node->size - 1)
					return nullptr;
				std::swap(node->state[node->blank_coord.first][node->blank_coord.second], node->state[node->blank_coord.first + 1][node->blank_coord.second]);
				node->blank_coord.first++;
			}
	return node;
}

void Game::print(Node* node, int& stari)
{
	if (node != nullptr)
	{
		++stari;
		print(node->parent, stari);
		std::cout << node;
		node->dealloc();
		delete node;
	}
}

std::istream& operator>>(std::istream& in, Game& game)
{
	in >> game.n;
	game.tiles = new int* [game.n];
	for (int i = 0; i < game.n; i++)
		game.tiles[i] = new int[game.n];
	for (int i = 0; i < game.n; i++)
	{
		for (int j = 0; j < game.n; j++)
		{
			in >> game.tiles[i][j];
			if (game.tiles[i][j] == 0)
			{
				game.blank_coord.first = i; game.blank_coord.second = j;
			}
		}
	}

	game.finish = new int* [game.n];
	for (size_t i = 0; i < game.n; i++)
		game.finish[i] = new int[game.n];

	game.coord = new std::vector<std::pair<int, int>>();
	int k = 0;
	for (size_t i = 0; i < game.n; i++)
	{
		for (size_t j = 0; j < game.n; j++)
		{
			game.finish[i][j] = k++;
			game.coord->push_back(std::make_pair(i, j));
		}
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, Game& game)
{
	for (size_t i = 0; i < game.n; i++)
	{
		for (size_t j = 0; j < game.n; j++)
		{
			out << game.tiles[i][j] << " ";
		}
		out << "\n";
	}
	return out;
}
