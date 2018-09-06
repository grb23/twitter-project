#include <iostream>
#include <vector>
using namespace std;

template <class T, class Comparator>
void merge(vector<T>& input, vector<T>& output, int s1, int e1, int s2, int e2)
{
	// you have two starts and two ends, they shouldn't cross each other
	// start from the beginning of both those parts are compare

	Comparator comp;
	unsigned int i = s1;

	// fails when you reach the end of one of the lists
	while((s1 < e1 && s2 < e2) && i < output.size())
	{
		if(comp(input.at(s1),input.at(s2)))
		{
			// overwriting the output?
			output.at(i) = (input.at(s1++));
			i++;
		}
		else
		{
			output.at(i) = (input.at(s2++));
			i++;
		}
	}

	// then if either is finished you just put whatever's left
	while(s1 < e1 && i < output.size())
	{
		output.at(i) = (input.at(s1++));
		i++;
	}

	while(s2 < e2 && i < output.size())
	{
		output.at(i) = (input.at(s2++));
		i++;
	}

	return;
}

template <class T, class Comparator>
void msort(vector<T>& input, vector<T>& output, int start, int end)
{

	// base case
	if(end-start <= 1)
	{
		return;
	}
	// recursive calls
	int mid = (start+end)/2;

	msort<T, Comparator>(input, output, start, mid);
	msort<T, Comparator>(input, output, mid, end);

	merge<T, Comparator>(input, output, start, mid, mid, end);

	// save progress
	for(int i = start; i < end; i++)
  	{
  		input.at(i) = output.at(i);
  	}

}

template <class T, class Comparator>
void mergeSort (vector<T>& input, Comparator comp)
{
	vector<T> copyInput(input); // copy of vector
	// use copyInput as the source vector, input as the output vector

	msort<T, Comparator>(copyInput, input, 0, (int)input.size());
}
