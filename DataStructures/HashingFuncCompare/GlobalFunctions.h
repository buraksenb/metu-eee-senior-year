#pragma once
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
// For determining hash function
enum HashType
	{FOLDING = 1,MIDDLESQUARING, TRUNCATION} ;

void printHashTable(std::list <long long int>* pHashTable,int iSize);

// Since T.C numbers are 11 digit numbers, they overflow normal "int" type which is 4 bytes
// Thus, long long int which is 8 bytes is used.
int Folding(long long int iKey, int iSize);

int MiddleSquaring(long long int iKey, int iSize);

int Truncation(long long int iKey, int iSize);

void addToHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize);
void RemoveFromHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize);
void SearchKeyHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize);
