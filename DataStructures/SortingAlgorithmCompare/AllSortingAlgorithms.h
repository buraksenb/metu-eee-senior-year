#pragma once
#include  <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

struct CompAndDataMove
{
    int iComparisonCount, iDataMoveCount;

	CompAndDataMove()
	{
        iComparisonCount = 0;
        iDataMoveCount = 0;
	}
};
extern CompAndDataMove arrSortingCounts[7];
// I have combined all of the data for the sorting algorithms in an array
// Indexes of this arrays correspond to an algorithms comparison count and data move count
/* Indexes correspond to
 0- Bubble Sort
 1- QuickSort1
 2- QuickSort2
 3- QuickSort3
 4- QuickSort4
 5- Selection Sort
 6 -MySortingAlgorithm
*/


void SwapElements(int& swX, int& swY);

void PrintArray(int* arrToPrint, int iSize);

void BubbleSort(int* arrToSort, int iSize);

void SelectionSort(int* arrToSort, int iSize);

void InsertionSort(int* arrToSort, int iSize);

void QuickSort1(int* arrToSort, int iLow, int iHigh);

void QuickSort2(int* arrToSort, int iLow, int iHigh);

void QuickSort3(int* arrToSort, int iLow, int iHigh);

void QuickSort4(int* arrToSort, int iLow, int iHigh);

void MySortingAlgorithm(int* arrToSort, int iLow, int iHigh);
