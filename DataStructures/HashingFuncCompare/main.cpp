#include <iostream>
#include "GlobalFunctions.h"
#include <fstream>
#include <string>

// To calculate load factor and collision count, this value can be modified to 1
// then  after printing the table,the program will print collision count and load factor if input is "6"
#define CHECKLFANDCOLLISION 1
int main()
{

	HashType enHashType;
	int N = 0;
	std::list<long long int>* hashTable = nullptr;

	std::ifstream stFile;
	stFile.open("hash_table_init.txt");
	std::string sLine;
	int iInput = 0;
	long long int liTCNumber = 0 ;
	std::list<long long int>::iterator lIterator;

	std::cout << "Please enter (1-6) to perform desired operation\n" << "1. Initialize Hash Table\n"
			  << "2. Load T.C. ID Numbers from file\n"
			  << "3. Add new T.C. ID Number\n"
			  << "4. Delete a T.C. ID Number\n"
			  << "5. Search for a T.C. ID Number\n"
			  << "6. Print out Hash Table\n";

	while(true)
	{
		std::cin >> iInput;
		switch(iInput)
		{
			case 1 :
				std::cout << "Enter the size of the hash table\n";
				std::cin >> N;
				hashTable = new std::list<long long int>[N]();
				std::cout << "Please select the hashing function:\n1)Folding\n2)Middle Squaring\n3)Truncating\n";
				std::cin >> iInput;
				// Cast input to enum type
				enHashType = static_cast<HashType>(iInput);
				break;

			case 2:
				if(stFile.is_open())
				{

					while(!stFile.eof())
					{
						// Get T.C IDs one by one in string format then convert them to long long int and add to hash table
						std::getline(stFile,sLine);
					 	liTCNumber = std::stoll(sLine) ;
						addToHashTable(hashTable,enHashType,liTCNumber,N);


					}
				}
				stFile.close();

				break;

			case 3:
				std::cout << "Please enter new T.C ID Number\n";
				std::cin >> liTCNumber;
				addToHashTable(hashTable,enHashType,liTCNumber,N);

				break;

			case 4:
				std::cout << "Please enter T.C ID Number which will be deleted\n";
				std::cin >> liTCNumber;
				RemoveFromHashTable(hashTable,enHashType,liTCNumber,N);

				break;
			case 5:
				std::cout << "Please enter the T.C ID Number which will be searched \n";
				std::cin >> liTCNumber;
				SearchKeyHashTable(hashTable,enHashType,liTCNumber,N);
				break;
			case 6:
				printHashTable(hashTable,N);

			default:
			    std::cout << "Wrong input is entered\n";
			break;
		}
			#if CHECKLFANDCOLLISION
			if(iInput == 6)
			{
				int iCollisionCount = 0;
				float fLoadingFactor = 0.0f;
				int iZeroSizeEntryCount = 0;
				int iOneSizeEntryCount = 0;
				int iSizeofList = 0;
				for (int i = 0; i < N; i++)
				{
					if (hashTable[i].empty())
					{
						iZeroSizeEntryCount++;
					}
					else
					{
						iSizeofList = hashTable[i].size();
						if (iSizeofList == 1)
						{
							iOneSizeEntryCount++;
						}
						else
						{
							iCollisionCount += (iSizeofList - 1);
						}

					}
				}
				// normally loading factor is Entry/Size; however if we calculate it this way
				// loading factors of the 3 hash functions will be equal. Thus, i will calculate lf for Entry/
				std::string sHashF = "";
				switch (enHashType)
				{
				case FOLDING:
					sHashF = "Folding";
					break;

				case MIDDLESQUARING:
					sHashF = "Middle Squaring";
					break;

				case TRUNCATION:
					sHashF = "Truncation";
					break;
				default:
					break;
				}
				std::cout << "Hash funcion is " << sHashF << std::endl;
				fLoadingFactor = 200.0f / (N - iZeroSizeEntryCount);
				std::cout << "Loading Factor: " << fLoadingFactor << std::endl;
				std::cout << "Collision count: " << iCollisionCount << std::endl;
				std::cout << "Zero size entry count: " << iZeroSizeEntryCount << std::endl;
				std::cout << "One size entry count: " << iOneSizeEntryCount << std::endl;

			}
			#endif

		std::cout << "Another input may be entered:\n";
	}



}
