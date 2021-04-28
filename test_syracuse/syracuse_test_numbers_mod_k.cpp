// this is a test for syracuse number mod k
// when k increase, less and less numbers need to be tested in proportion in a k size set

#include <iostream>
using namespace std;

#include "syracuse_test_numbers_mod_k.h"
#include "biginteger.h"

///////////////////////////////////////////////////////////////////////////////////////////
void syracuse_test_numbers_mod_k()
{
	// test at k level
	for (int m = 0; m < 28; m++)
	{
		biginteger k = (1LL << m);
		int iKept = 0;

		for (biginteger b = 0; b < k; b++)
		{
			//every number has the form
			// x=a*k+b 0<=b<k

			biginteger newk = k;
			biginteger newb = b;

			for (;;) // in case of divergence or cycle, this would never stop, but it does (the program finishes)
			{
				if ((newk < k) && (newb <= b))
					break; //not a possible minimum, no need to iterate more

				if (is_odd(newk))
				{
					//	cout << "number with k=" << k << " and b=" << b << " is kept" << endl;
					iKept++; // kept as a number to test
					break; // cannot iterate more, parity of result is not known
				}

				//one iteration if odd
				if (is_odd(newb))
				{
					//odd number
					newk = newk * 3;
					newb = newb * 3 + 1;
					//the number is now even
				}

				//the number is even, one more iteration
				newk = newk >> 1; // /2
				newb = newb >> 1; // /2
			}
		}
		cout << "m=" << m << " k=" << k << " numbers=" << iKept << " proportion=" << (100.f*iKept) / k << " %" << endl;
	}
}
