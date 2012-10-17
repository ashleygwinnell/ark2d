/*
 * Sort.cpp
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */
#ifdef _WIN32
#include <windows.h>
#endif

#include <ctime>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

void selectionSortAscending(int anArray[]);
void selectionSortAscending(int anArray[])
{
	int nSize = sizeof(&anArray) / sizeof(anArray[0]);
	// Step through each element of the array
	for (int nStartIndex = 0; nStartIndex < nSize; nStartIndex++)
	{
		// nSmallestIndex is the index of the smallest element
		// we've encountered so far.
		int nSmallestIndex = nStartIndex;

		// Search through every element starting at nStartIndex+1
		for (int nCurrentIndex = nStartIndex + 1; nCurrentIndex < nSize; nCurrentIndex++)
		{
			// If the current element is smaller than our previously found smallest
			if (anArray[nCurrentIndex] < anArray[nSmallestIndex])
				// Store the index in nSmallestIndex
				nSmallestIndex = nCurrentIndex;
		}

		// Swap our start element with our smallest element
		std::swap(anArray[nStartIndex], anArray[nSmallestIndex]);
	}

}
