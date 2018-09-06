#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "hash.h"

using namespace std;

unsigned long long calculateHash(string mystring)
{

	int sizePass = mystring.size();

	vector<int> holdInt;

	unsigned long long hashNum = 0;

	for(int i = 0; i < sizePass; i++)
	{
		holdInt.push_back(int(mystring.at(i)));
	}

	for(int i = sizePass-1; i >= 0; i--)
	{
		hashNum = hashNum + ((unsigned long long)(pow(128, sizePass-1-i)) * (unsigned long long)holdInt.at(i));
	}

	unsigned int valueHold[4];

	valueHold[0] = hashNum % 65521;
	hashNum = hashNum / 65521;

	valueHold[1] = hashNum % 65521;
	hashNum = hashNum / 65521;

	valueHold[2] = hashNum % 65521;
	hashNum = hashNum / 65521;

	valueHold[3] = hashNum % 65521;
	hashNum = hashNum / 65521;

	unsigned long long result = 0;
	result = (45912 * valueHold[3]
	 + 35511 * valueHold[2]
	 + 65169 * valueHold[1]
	 + 4625 * valueHold[0])
	 % 65521;

	return result;

}
