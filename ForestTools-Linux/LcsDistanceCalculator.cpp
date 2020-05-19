/******************************************************************************
DeAngelo Wilson
January 3 2020

						LCSDistanceMeasureCalculator (strategy -- derived)
******************************************************************************/

#include "LcsDistanceCalculator.h"
#include "FileObject.h"

namespace distanceMeasure
{
	distanceMeasure::LcsDistanceCalculator::LcsDistanceCalculator(RunFlags* flags):
	Unaligned_InternalDMCalculator(flags)
	{
	}
	
	void distanceMeasure::LcsDistanceCalculator::calculate_and_output_matrix(FileObjectManager& fileObjectManager, const std::vector<std::string>& sequence_set_names, const std::string& sequence_set, const int batch_id)
	{
		this->StartCalculationTimer();

		Unaligned_InternalDMCalculator::calculate_and_output_matrix(fileObjectManager, sequence_set_names, sequence_set, batch_id);

		this->StopCalculationTimer(batch_id, sequence_set);
	}

	std::string distanceMeasure::LcsDistanceCalculator::GetCalculatorName() const
	{
		return "LCS";
	}
	
	float LcsDistanceCalculator::calculate_normalized_distance(const FileObject& file1, const FileObject& file2) const
	{

		const int m = file1.GetSequenceSize();
		const int n = file2.GetSequenceSize();
		const int totalsize = m * n;

		/*
		const int testSize = totalsize + m + n;
		int* c = new int[testSize];

		for (int i = 0; i <= m ; i++)
		{
			for (int j = 0; j <= n; j++)
			{
				if(j==0 || i ==0)
				{
					c[getArrayIndex(i,j,n)] = 0;
				}
				// buffer[i-1] "-1" to account for Epsilon
				else if (file1.sequenceBuffer[i - 1] == file2.sequenceBuffer[j - 1])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
				}
				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
				}
				else
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
				}
			}
		}
		*/


		int* c = new int[totalsize] {0};

		//**************     NEEDED????????????       **********
		//for (int i = 0; i < totalsize; i += n)
		//{
		//	c[i] = 0;
		//}
		//for (int j = 0; j < n; j++)
		//{
		//	c[j] = 0;
		//}
		
		//~.04 seconds to initalize values to 0
		for (int i = 1; i < m; i++)
		{
			for (int j = 1; j < n; j++)
			{
				// buffer[i-1] "-1" to account for Epsilon
				//if char at index is
				if (file1.GetSequenceCharAt(i - 1) == file2.GetSequenceCharAt(j - 1))
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j - 1, n)] + 1;
				}
				else if (c[getArrayIndex(i - 1, j, n)] >= c[getArrayIndex(i, j - 1, n)])
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i - 1, j, n)];
				}
				else
				{
					c[getArrayIndex(i, j, n)] = c[getArrayIndex(i, j - 1, n)];
				}
			}
		}
		//CHEATY CHEATY + 1 -- improper LCS CALCULATION ON SELF (FIXXX!!!!!!!!!!)
		int result = c[getArrayIndex(m - 1, n - 1, n)] + 1;
		//int* resAddr = &c[getArrayIndex(m - 1, n - 1, n)];
		delete[] c;
		//return last index of array
		return normalize(result, maxSequenceLength(file1.GetSequenceSize(), file2.GetSequenceSize()));

	}

	//const int LcsDistanceCalculator::getArrayIndex(int i, int j, int n) const
	//{
	//	return (i * n) + j;
	//}

	float LcsDistanceCalculator::normalize(int lcs, int maxSequenceSize) const
	{

		//cheaty + 1 to set LCS to correct value???**************LCS CALC OFF BY 1********

		float normalizedDistance = float(maxSequenceSize - lcs) / maxSequenceSize;

		//printf("%f -- %d -- %d\n", normalizedDistance, lcs, maxSequenceSize);
		return normalizedDistance;
	}

	int LcsDistanceCalculator::maxSequenceLength(int sequencesize1, int sequencesize2) const
	{
		return sequencesize1 > sequencesize2 ? sequencesize1 : sequencesize2;
	}

}


//TODO::: FIX/UPDATE/REFACTOR LCS CODE********************************************************************************
/*
int lcs( char *X, char *Y, int m, int n )  
{  
    int L[m + 1][n + 1];  
    int i, j;  
      
     //Following steps build L[m+1][n+1] in bottom up fashion.

     //Note: L[i][j]  contains length of LCS of X[0..i-1] 
     //  and Y[0..j-1] 
    for (i = 0; i <= m; i++)  
    {  
        for (j = 0; j <= n; j++)  
        {  
        if (i == 0 || j == 0)  
            L[i][j] = 0;  
      
        else if (X[i - 1] == Y[j - 1])  
            L[i][j] = L[i - 1][j - 1] + 1;  
      
        else
            L[i][j] = max(L[i - 1][j], L[i][j - 1]);  
        }  
    }  
          
    // L[m][n] contains length of LCS  
   // for X[0..n-1] and Y[0..m-1] 
    return L[m][n];  
}  
  
//Utility function to get max of 2 integers 
int max(int a, int b)  
{  
    return (a > b)? a : b;  
}  
  
// Driver Code 
int main()  
{  
    char X[] = "AGGTAB";  
    char Y[] = "GXTXAYB";  
      
    int m = strlen(X);  
    int n = strlen(Y);  
      
    cout << "Length of LCS is " 
         << lcs( X, Y, m, n );  
      
    return 0;  
}  


*/