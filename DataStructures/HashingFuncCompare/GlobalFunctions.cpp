#include "GlobalFunctions.h"



void printHashTable(std::list <long long int>* pHashTable,int iSize)
{	
	// Define an iterator to traverse through the list and print the contents of the list
	std::list <long long int> ::iterator lIterator;
	for(int i=0 ; i< iSize; i++)
	{
		std::cout << i << ")" ;
		for (lIterator = pHashTable[i].begin(); lIterator != pHashTable[i].end(); ++lIterator)
		{
			std::cout << *lIterator << "      ";
		}
		// if entry is empty print 0 
		if(pHashTable[i].empty())
		{
			std::cout << " 0"; 
		}
		std::cout << '\n';
	}	
}

int Folding(long long int iKey, int iSize)
{
	int iSum = 0;
	// Since partition will be 3-3-3-2 digits, first division of the key
	// will be done to 100 for 2 digit partition and after that key will be divided to 1000
	iSum += iKey % iSize;
	iKey /= 100;
	while(iKey > 0)
	{
		iSum += iKey % iSize;
		iKey /= 1000; 
	}
	return iSum % iSize; 
}
int MiddleSquaring(long long int iKey, int iSize)
{
	// Convert key to string, get its length and find the middle index
	// after that get the substring from MiddleIndex-1 to MiddleIndex+1
	const std::string sKey = std::to_string(iKey);
	int iLen = sKey.length();
	int iMiddleIndex = iLen / 2; 
	std::string sMiddleDigits = sKey.substr(iMiddleIndex - 1, 3); 
	int iMiddleDigits = std::stoi(sMiddleDigits);
	return (iMiddleDigits * iMiddleDigits) % iSize; 
}
int Truncation(long long int iKey, int iSize)
{
	return iKey % iSize; 
}


void addToHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize)
{
	int iKey = -1;
	std::list<long long int>::iterator lIterator;
	// determine hash function 
	switch(pHashType)
	{
		case HashType::FOLDING :
			iKey = Folding(pTCnumb,iSize) ; 
			break;

		case HashType::MIDDLESQUARING :
			iKey = MiddleSquaring(pTCnumb,iSize); 
			break;

		case HashType::TRUNCATION:
			iKey = Truncation(pTCnumb,iSize); 
			break; 
		default:
			break;
	}
	// use iterator to check if KEY exists in the table, if it exists lIterator is not equal to pHasbTable[iKey].end()
	// then dont add the KEY
	lIterator = std::find(pHashTable[iKey].begin(), pHashTable[iKey].end(), pTCnumb);
	if(lIterator == pHashTable[iKey].end())
	{
		pHashTable[iKey].push_front(pTCnumb); 
	}
	else
	{
		std::cout << "T.C ID number: " << pTCnumb << " already exists, thus it can not be added\n"; 
	}
}
void RemoveFromHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize)
{
	int iKey = -1; 
	std::list<long long int>::iterator lIterator;
	switch(pHashType)
	{
		case HashType::FOLDING :
			iKey = Folding(pTCnumb,iSize) ; 
			break;	

		case HashType::MIDDLESQUARING :
			iKey = MiddleSquaring(pTCnumb,iSize); 
			break;	

		case HashType::TRUNCATION:
			iKey = Truncation(pTCnumb,iSize); 
			break; 

		default:
			break;
	}
	lIterator = std::find(pHashTable[iKey].begin(), pHashTable[iKey].end(), pTCnumb);
	
	// if lIterator is equal to .end(), KEY does not exist 
	if(lIterator != pHashTable[iKey].end())
	{
	pHashTable[iKey].remove(pTCnumb); 
	}
	else
	{
		std::cout << "T.C ID number: " << pTCnumb << " does not exist, thus it can not be removed\n";
	}
}

void SearchKeyHashTable(std::list <long long int>* pHashTable,HashType pHashType,long long int pTCnumb,int iSize)
{
	int iKey = -1; 
	std::list<long long int>::iterator lIterator;
	switch(pHashType)
	{
	case HashType::FOLDING :
		iKey = Folding(pTCnumb,iSize) ; 
	break;	
	case HashType::MIDDLESQUARING :
		iKey = MiddleSquaring(pTCnumb,iSize); 
	break;	
	case HashType::TRUNCATION:
		iKey = Truncation(pTCnumb,iSize); 
	break; 
	default:
	break;
	}
	lIterator = std::find(pHashTable[iKey].begin(), pHashTable[iKey].end(), pTCnumb);
	if (lIterator != pHashTable[iKey].end())
	{
		std::cout << "T.C ID Number: " << pTCnumb << " exists in the hash table\n"; 
	}
	else
	{
		std::cout << "T.C ID Number: " << pTCnumb << " does not exist in the hash table\n";
	}
}