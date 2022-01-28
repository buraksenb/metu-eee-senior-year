#include "AllSortingAlgorithms.h"


void SwapElements(int& swX, int& swY)
{
    int temp = swX;
    swX = swY;
    swY = temp;
}

void PrintArray(int* arrToPrint, int iSize)
{
    for (int i = 0; i < iSize; i++)
    {
        std::cout << std::setw(4) << arrToPrint[i] << " ";
        if (i % 24 == 0 && i != 0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}


void BubbleSort(int* arrToSort, int iSize)
{
    for (int i = 0; i < iSize - 1; i++)
    {
        for (int j = 0; j < iSize - i - 1; j++)
        {
            arrSortingCounts[0].iComparisonCount++;
            if (arrToSort[j] > arrToSort[j + 1])
            {
                arrSortingCounts[0].iDataMoveCount++;
                SwapElements(arrToSort[j], arrToSort[j + 1]);
            }
        }
    }
}


void SelectionSort(int* arrToSort, int iSize)
{
    int iMinIndex;

    // One by one move boundary of unsorted subarray
    for (int i = 0; i < iSize - 1; i++)
    {
        // Find the minimum element in unsorted array
        iMinIndex = i;
        for (int j = i + 1; j < iSize; j++)
        {
            arrSortingCounts[5].iComparisonCount++;
            if (arrToSort[j] < arrToSort[iMinIndex])
            {
                iMinIndex = j;
            }
        }
        arrSortingCounts[5].iDataMoveCount++;
        SwapElements(arrToSort[iMinIndex], arrToSort[i]);
    }
}


void InsertionSort(int* arrToSort, int iSize)
{
    int j;
    int tKey;
    for (int i = 1; i < iSize; i++)
    {
        tKey = arrToSort[i];
        j = i - 1;

        while (j >= 0 && arrToSort[j] > tKey)
        {
            arrSortingCounts[6].iComparisonCount++;
            arrSortingCounts[6].iDataMoveCount++;

            arrToSort[j + 1] = arrToSort[j];
            j = j - 1;
        }
        arrSortingCounts[6].iComparisonCount++;
        arrSortingCounts[6].iDataMoveCount++;
        arrToSort[j + 1] = tKey;
    }
}


void QuickSort1(int* arrToSort, int iLow, int iHigh) {
    int iPivot;
    if (iLow < iHigh)
    {
        int iScanUp = iLow + 1; // Initialize ScanUp
        int iScanDown = iHigh; // Initialize ScanDown
        iPivot = arrToSort[iLow]; // Set pivot
        while (true)
        {
            while (arrToSort[iScanUp] <= iPivot)
            {
                arrSortingCounts[1].iComparisonCount++;
                iScanUp++; // Increment ScanUp until pivot
            }
            // when comparison is not correct program does not go into while so increment outside
            arrSortingCounts[1].iComparisonCount++;
            while (arrToSort[iScanDown] > iPivot)
            {
                arrSortingCounts[1].iComparisonCount++;
                iScanDown--; // Decrement ScanDown until pivot
            }
            // when comparison is not correct program does not go into while so increment outside
            arrSortingCounts[1].iComparisonCount++;
            if (iScanUp < iScanDown)
            {

                arrSortingCounts[1].iDataMoveCount++;
                SwapElements(arrToSort[iScanDown], arrToSort[iScanUp]);
            }
            else
            {
                break;
            }
        }
        arrSortingCounts[1].iDataMoveCount++;
        SwapElements(arrToSort[iScanDown], arrToSort[iLow]);

        QuickSort1(arrToSort, iLow, iScanDown - 1);
        QuickSort1(arrToSort, iScanDown + 1, iHigh);
    }
}



void QuickSort2(int* arrToSort, int iLow, int iHigh) {
    int iPivot;
    if (iLow < iHigh)
    {
        int iScanUp = iLow + 1; // Initialize ScanUp
        int iScanDown = iHigh; // Initialize ScanDown
        int iPivotIndex = (iLow + iHigh) / 2;
        iPivot = arrToSort[iPivotIndex]; // Set pivot

        arrSortingCounts[2].iDataMoveCount++;
        SwapElements(arrToSort[iPivotIndex], arrToSort[iLow]);
        while (true)
        {
            while (arrToSort[iScanUp] <= iPivot)
            {
                arrSortingCounts[2].iComparisonCount++;
                iScanUp++; // Increment ScanUp until pivot
            }
            arrSortingCounts[2].iComparisonCount++;

            while (arrToSort[iScanDown] > iPivot)
            {
                arrSortingCounts[2].iComparisonCount++;
                iScanDown--; // Decrement ScanDown until pivot
            }
            arrSortingCounts[2].iComparisonCount++;

            if (iScanUp < iScanDown)
            {
                arrSortingCounts[2].iDataMoveCount++;
                SwapElements(arrToSort[iScanDown], arrToSort[iScanUp]);
            }
            else
            {
                break;
            }
        }
        arrSortingCounts[2].iDataMoveCount++;
        SwapElements(arrToSort[iScanDown], arrToSort[iLow]);

        QuickSort2(arrToSort, iLow, iScanDown - 1);
        QuickSort2(arrToSort, iScanDown + 1, iHigh);
    }
}


void QuickSort3(int* arrToSort, int iLow, int iHigh) {
    int iPivot;
    if (iLow < iHigh)
    {
        int iScanUp = iLow + 1; // Initialize ScanUp
        int iScanDown = iHigh; // Initialize ScanDown
        //
        // iRandom will be a value between iLow and iHigh including iLow and iHigh in range
        int iRandom = iLow + (rand() % (iHigh - iLow + 1));

        iPivot = arrToSort[iRandom]; // Set pivot
        arrSortingCounts[3].iDataMoveCount++;
        SwapElements(arrToSort[iRandom], arrToSort[iLow]);
        while (true)
        {
            arrSortingCounts[3].iComparisonCount++;
            while (arrToSort[iScanUp] <= iPivot)
            {
                arrSortingCounts[3].iComparisonCount++;
                iScanUp++; // Increment ScanUp until pivot
            }
            while (arrToSort[iScanDown] > iPivot)
            {
                arrSortingCounts[3].iComparisonCount++;
                iScanDown--; // Decrement ScanDown until pivot
            }
            arrSortingCounts[3].iComparisonCount++;
            if (iScanUp < iScanDown)
            {
                arrSortingCounts[3].iDataMoveCount++;
                SwapElements(arrToSort[iScanDown], arrToSort[iScanUp]);
            }
            else
            {
                break;
            }
        }
        arrSortingCounts[3].iDataMoveCount++;
        SwapElements(arrToSort[iScanDown], arrToSort[iLow]);

        QuickSort3(arrToSort, iLow, iScanDown - 1);
        QuickSort3(arrToSort, iScanDown + 1, iHigh);
    }
}



void QuickSort4(int* arrToSort, int iLow, int iHigh) {
    int iPivot;
    if (iLow < iHigh)
    {
        int iScanUp = iLow + 1; // Initialize ScanUp
        int iScanDown = iHigh; // Initialize ScanDown
        int iMedianIndex;         // iRandom will be a value between iLow and iHigh including iLow and iHigh in range
        if (iHigh - iLow > 1)
        {
            int iRandom1 = iLow + (rand() % (iHigh - iLow + 1));
            int iRandom2 = iLow + (rand() % (iHigh - iLow + 1));
            int iRandom3 = iLow + (rand() % (iHigh - iLow + 1));
            while (iRandom1 == iRandom2 || iRandom1 == iRandom3 || iRandom2 == iRandom3)
            {
                iRandom2 = iLow + (rand() % (iHigh - iLow + 1));
                iRandom3 = iLow + (rand() % (iHigh - iLow + 1));
            }
            // After obtaining three different random indexes find the index of the median of them
            arrSortingCounts[4].iComparisonCount++;
            if (arrToSort[iRandom1] > arrToSort[iRandom2])
            {
                if (arrToSort[iRandom2] > arrToSort[iRandom3])
                {
                    arrSortingCounts[4].iComparisonCount++;
                    iMedianIndex = iRandom2;
                }
                else if (arrToSort[iRandom1] > arrToSort[iRandom3])
                {
                    arrSortingCounts[4].iComparisonCount += 2;
                    iMedianIndex = iRandom3;
                }
                else
                {
                    arrSortingCounts[4].iComparisonCount += 2;
                    iMedianIndex = iRandom1;
                }
            }
            else
            {
                if (arrToSort[iRandom1] > arrToSort[iRandom3])
                {
                    arrSortingCounts[4].iComparisonCount++;
                    iMedianIndex = iRandom1;
                }
                else if (arrToSort[iRandom2] > arrToSort[iRandom3])
                {
                    arrSortingCounts[4].iComparisonCount += 2;
                    iMedianIndex = iRandom3;
                }
                else
                {
                    arrSortingCounts[4].iComparisonCount += 2;
                    iMedianIndex = iRandom2;
                }
            }
        }
        else
        {
            // If length of the array is smaller than 3, let pivot be the smaller index.
            iMedianIndex = iLow;
        }

        iPivot = arrToSort[iMedianIndex]; // Set pivot
        arrSortingCounts[4].iDataMoveCount++;
        SwapElements(arrToSort[iMedianIndex], arrToSort[iLow]);
        while (true)
        {
            while (arrToSort[iScanUp] <= iPivot)
            {
                arrSortingCounts[4].iComparisonCount++;
                iScanUp++; // Increment ScanUp until pivot
            }
            arrSortingCounts[4].iComparisonCount++;

            while (arrToSort[iScanDown] > iPivot)
            {
                arrSortingCounts[4].iComparisonCount++;
                iScanDown--; // Decrement ScanDown until pivot
            }
            arrSortingCounts[4].iComparisonCount++;

            if (iScanUp < iScanDown)
            {
                arrSortingCounts[4].iDataMoveCount++;
                SwapElements(arrToSort[iScanDown], arrToSort[iScanUp]);
            }
            else
            {
                break;
            }
        }
        arrSortingCounts[4].iDataMoveCount++;
        SwapElements(arrToSort[iScanDown], arrToSort[iLow]);

        QuickSort4(arrToSort, iLow, iScanDown - 1);
        QuickSort4(arrToSort, iScanDown + 1, iHigh);
    }
}


// Since insertion sort is much more quick for smaller arrays and almost ordered arrays
// I will use Quick Sort for larger sets and when the sets become small Insertion Sort will be used.
void MySortingAlgorithm(int* arrToSort, int iLow, int iHigh)
{
	// If length is smaller than 16, do insertion sort instead of quick sort for speed
	if(iHigh -iLow < 15 && iHigh != iLow)
	{
        InsertionSort(&arrToSort[iLow], iHigh - iLow + 1);
	}
    else
    {
    int iPivot;
    if (iLow < iHigh)
    {
        int iScanUp = iLow + 1; // Initialize ScanUp
        int iScanDown = iHigh; // Initialize ScanDown

        iPivot = arrToSort[iLow]; // Set pivot
        while (true)
        {
            while (arrToSort[iScanUp] <= iPivot)
            {
                arrSortingCounts[6].iComparisonCount++;
                iScanUp++; // Increment ScanUp until pivot
            }
            arrSortingCounts[6].iComparisonCount++;
            while (arrToSort[iScanDown] > iPivot)
            {
                arrSortingCounts[6].iComparisonCount++;
                iScanDown--; // Decrement ScanDown until pivot
            }
            arrSortingCounts[6].iComparisonCount++;
            if (iScanUp < iScanDown)
            {
                arrSortingCounts[6].iDataMoveCount++;
                SwapElements(arrToSort[iScanDown], arrToSort[iScanUp]);
            }
            else
            {
                break;
            }
        }
        arrSortingCounts[6].iDataMoveCount++;
        SwapElements(arrToSort[iScanDown], arrToSort[iLow]);

        MySortingAlgorithm(arrToSort, iLow, iScanDown - 1);
        MySortingAlgorithm(arrToSort, iScanDown + 1, iHigh);

    }
    }

}
