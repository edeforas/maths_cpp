// Simple test for syracuse number
// test all number under 1e9
// small optimisation: 
//     for every values, early abort if lower than start (already tested previously)
//     (the start number is not a cycle minimum)

#include <iostream>
using namespace std;

#include "syracuse_test_numbers.h"
#include "biginteger.h"

#define MAX_BIGINTEGER_BEFORE_TRIPLE ((MAX_BIGINTEGER)/3-1)

biginteger syracuse_iterate(biginteger n)
{
	if (n % 2)
	{
		if (n >= MAX_BIGINTEGER_BEFORE_TRIPLE)
			return 0; //precision loss

		return n * 3 + 1;
	}
	else
		return n >> 1;
}
///////////////////////////////////////////////////////////////////////////////////////////
void syracuse_test_numbers()
{
	biginteger N = (1'000'000'000LL);
	bool bStrange = false;

	cout << "testing with number up to " << N << endl; 

	// test with i<N
	for (biginteger i = 1; i < N; i++)
	{
		if ((i % 10'000'000)==0)
			cout << i << endl;
		 
		biginteger iMaxFly =N;

		biginteger iS = i, iFly = 0;
		while ( (iS > 1) && (iFly< iMaxFly) )
		{
			iS = syracuse_iterate(iS);
		
			if (iS < i) //already tested
				if(iS!=0)
					iS = 1; // iS is not a sequence minimum nor a computation loss

			iFly++;
		}

		if (iS == 0)
		{ // computation precision problem, lower precision or minimum sequence number ? 
			cout << endl << i << " precision loss!";
			bStrange = true;
		}

		if (iFly == iMaxFly)
		{  // strange number, can be a sequence minimum
			cout << endl << i << "*****************************";
			bStrange = true;
		}
	}
	
	if (bStrange)
		cout << "Strange number found!!!!" << endl << endl ;
	else
	{
		//no strange number found;
		cout << "there is no cycle other than 1,4,2,1 or divergence, for N < " << N << endl << endl;
	}
}