#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// assumes little endian
// usage  printBits(sizeof(i), &i);
void printBits(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

bool isEven(int i)
{
	int j = i & 1;
	return j != 1;
}

bool isOdd(int i)
{
	int j = i & 1;
	return j == 1;
}

int main() {
	int N;
	cin >> N;
	cout <<isEven(N);
	getchar();
	return 0;
}