#include <ctime>
#include  <iostream>
#include "AllSortingAlgorithms.h"
#include <fstream>
#include <string>
#include <string.h>
CompAndDataMove arrSortingCounts[7];

int main()
{
	srand(time(NULL));
	int N;
	std::ifstream stFile;
	stFile.open("input_array.txt");

	int* arrToSort = nullptr;
	int* arrToCheck = nullptr;
	int iInput = 0 ;
	int iInputPivot = 0;
	std::string sLine;
	double arrTimeForAllAlg[7]  = {0.0} ;
	double dStart = 0.0;
	double dFinish = 0.0;
	// You may change this value for different sizes
	int iLoopForTimeMeasurement = 20;
	std::string arrSortingNames[7] = {"Bubble","Quick1","Quick2","Quick3","Quick4","Selection","MyAlgorithm"} ;

	std::cout << "You may select the desired operation explained below with their numbers 1-7:\n";
	std::cout << "1. Initialize input array randomly\n" << "2. Load input array from a file\n"
	<< "3. Perform Bubble Sort\n" << "4. Perform Quick Sort\n" << "5. Perform Selection Sort\n"
	<< "6. Perform Your Own Sort\n" << "7. Compare sorting algorithms\n";
	while(true)
	{
		std::cin >> iInput;

		switch(iInput)
		{
			case 1:
				std::cout << "Please enter size of the array: \n";
				std::cin >> N;
				arrToSort = new int[N]() ;
				for(int i = 0 ; i < N ; i++)
				{
					// rand() function returns integers range from 0 to 32,767. Thus I add 4 rand() returns to obtain 0 to 131.068 which I think enough for randomness.
					arrToSort[i] = (rand() + rand() + rand() + rand() )% (10*N);
				}
				std::cout << "Array is loaded with random values:\n";
				PrintArray(arrToSort, N);
			break;

			case 2 :
				arrToCheck = new int[N]();
				if(stFile.is_open())
				{
					int j = 0 ;
					// get numbers line by line and convert them to integer and assign to array
					while(!stFile.eof() && j<N)
					{
						std::getline(stFile,sLine);
						arrToCheck[j] = std::stoi(sLine) ;
						j++;
					}
				}
				stFile.close();
			break;

			case 3:
				BubbleSort(arrToSort,N) ;
				PrintArray(arrToSort,N) ;
			break ;

			case 4:
				iInputPivot = 0;
				std::cout << "Please select how pivot will be determined:\n1) First element\n2) Middle element\n";
				std::cout << "3) Randomly choosen element\n4)Median of three randomly choosen element\n" ;
				std::cin >> iInputPivot ;
				switch(iInputPivot)
				{
					case 1 :
						QuickSort1(arrToSort,0,N-1);
					break;
					case 2:
						 QuickSort2(arrToSort,0,N-1);
					break;
					case 3:
						 QuickSort3(arrToSort, 0, N - 1);
					break;
					case 4:
						 QuickSort4(arrToSort, 0, N - 1);
					break;
					default:
					std::cout << "Input was wrong, array will not be sorted\n";
					break;
				}
				PrintArray(arrToSort,N);
			break;

			case 5:
				SelectionSort(arrToSort,N) ;
				PrintArray(arrToSort,N) ;
			break ;

			case 6:
				MySortingAlgorithm(arrToSort,0,N-1) ;
				PrintArray(arrToSort,N) ;
			break;

			case 7:
			    // we have 7 algorithms and these algorithms will be tested with different array sizes.
			    // If smaller arrays are used please increase the iLoopForTimeMeasurement to observe passed time.
			    // For example, for array size 100, iLoopForTimeMeasurement = 1000 can be used for precision. However, for array size 5000
			    // iLoopForTimeMeasurement = 5 is enough.

			    // This code below, uses 7 algorithms to sort the array(with array being reverted to unsorted every loop)
			    // and time passed for each sorting algorithm is added to their respective struct.
			for(int k = 0 ; k<7 ; k++)
			{
				for(int l = 0 ; l < iLoopForTimeMeasurement ; l++)
				{
					switch(k)
					{
					case 0 :
						dStart = static_cast<double>(clock());
						BubbleSort(arrToSort,N) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 1:
						dStart = static_cast<double>(clock());
						QuickSort1(arrToSort,0,N-1) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 2:
						dStart = static_cast<double>(clock());
						QuickSort2(arrToSort,0,N-1) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 3:
						dStart = static_cast<double>(clock());
						QuickSort3(arrToSort,0,N-1) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 4:
						dStart = static_cast<double>(clock());
						QuickSort4(arrToSort,0,N-1) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 5:
						dStart = static_cast<double>(clock());
						SelectionSort(arrToSort,N) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					case 6:
						dStart = static_cast<double>(clock());
						MySortingAlgorithm(arrToSort,0,N-1) ;
						dFinish = static_cast<double>(clock());
						arrTimeForAllAlg[k] += dFinish - dStart ;
					break;
					}
					// for obtaining unsorted random array after sorting
					for (int i = 0; i < N; i++)
					{
						
						arrToSort[i] = (rand() + rand() + rand() + rand()) % (10 * N);
					}
				}
			}
			// Obtain the  comparison, data move counts and passed time by dividing the result with iLoopForTimeMeasurement
			// for obtaining the ONE run value.
			for(int i=0 ; i<7 ; i++)
			{
				arrTimeForAllAlg[i] = arrTimeForAllAlg[i] / iLoopForTimeMeasurement;
				arrSortingCounts[i].iComparisonCount = arrSortingCounts[i].iComparisonCount / iLoopForTimeMeasurement;
				arrSortingCounts[i].iDataMoveCount = arrSortingCounts[i].iDataMoveCount/ iLoopForTimeMeasurement;
			}

			std::cout << "Array size: " << N << " numbers\n" ;
			std::cout << std::setw(15) <<"Algorithm" << std::setw(15) << "#comparisons" << std::setw(15) << "#moves" << std::setw(15) << "time (msec)\n" ;
			for(int i=0 ; i<7; i++)
			{
				std::cout << std::setw(15) << arrSortingNames[i] << std::setw(15) << arrSortingCounts[i].iComparisonCount <<  std::setw(15) <<  arrSortingCounts[i].iDataMoveCount
				<<  std::setw(15) << arrTimeForAllAlg[i] << std::endl;

			}
			break;

			default:
			    std::cout << "Wrong input is entered";
			break ;

		}
		std::cout << "\nAnother operation input can be entered:\n";
	}



}
