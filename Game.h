#pragma once
#include <iostream>
#include "Node.h"
#include <queue>
#include <unordered_map>
#include <vector>
class Game
{
public:
	Game();
	std::vector < std::pair<int, int> >* coord;
	int** finish;
	std::pair<int, int> blank_coord;
	int** tiles;
	int size();
	int Manhattan(Node* node);
	friend std::istream& operator >>(std::istream& in, Game& game);
	friend std::ostream& operator <<(std::ostream& out, Game& game);
	bool solvable(Node* state);
	int inversions(Node* state);
	int inversions();
	bool isSolved(Node* state);
	Node* createNode(Node* parent, std::string action);
	void print(Node* node, int& stari);
	bool operator()(Node* x1, Node* x2)
	{
		double s1 = x1->cost + Manhattan(x1);
		double s2 = x2->cost + Manhattan(x2);
		return s1 > s2;
	}

private:

	int merge(int* arr, int* temp, int left, int mid, int right)
	{
		int i, j, k;
		int inv_count = 0;

		i = left; /* i is index for left subarray*/
		j = mid; /* j is index for right subarray*/
		k = left; /* k is index for resultant merged subarray*/

		for (int index1 = left; index1 <= mid - 1; index1++)
			std::cout << arr[index1] << " ";
		std::cout << "\n";

		for (int index1 = mid; index1 <= right; index1++)
			std::cout << arr[index1] << " ";
		std::cout << "\n";


		while ((i <= mid - 1) && (j <= right)) {
			if (arr[i] == 0)
				temp[k++] = arr[i++];
			else if (arr[j] == 0)
				temp[k++] = arr[j++];
			else
			{
				if (arr[i] <= arr[j]) {
					temp[k++] = arr[i++];
				}
				else {
					temp[k++] = arr[j++];

					inv_count = inv_count + (mid - i);
				}
			}
		}

		while (i <= mid - 1)
			temp[k++] = arr[i++];

		while (j <= right)
			temp[k++] = arr[j++];

		for (i = left; i <= right; i++)
			arr[i] = temp[i];

		return inv_count;
	}
	int mergeSort(int* arr, int* temp, int left, int right)
	{
		int inversions = 0, mid;
		if (right > left)
		{
			mid = (right + left) / 2;
			inversions += mergeSort(arr, temp, left, mid);
			inversions += mergeSort(arr, temp, mid + 1, right);

			inversions += merge(arr, temp, left, mid + 1, right);

		}
		return inversions;
	}
	int row_from_bottom();

private:

	std::pair<int, int> BinarySearch(Node* node, int row, int col, int x);
	int n;
	double calculate_cost(Node* x);
};

