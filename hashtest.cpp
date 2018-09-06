#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <vector>
#include "hash.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "main" << endl;
	string pass1 = "abc123";
	string pass2 = "password";
	string pass3 = "fighton";
	string pass4 = "Cs4";

	unsigned long long hash1 = calculateHash(pass1);
	unsigned long long hash2 = calculateHash(pass2);
	unsigned long long hash3 = calculateHash(pass3);
	unsigned long long hash4 = calculateHash(pass4);

	cout << pass1 << "'s hash is " << hash1 << endl; 
	cout << pass2 << "'s hash is " << hash2 << endl; 
	cout << pass3 << "'s hash is " << hash3 << endl; 
	cout << pass4 << "'s hash is " << hash4 << endl; 

}