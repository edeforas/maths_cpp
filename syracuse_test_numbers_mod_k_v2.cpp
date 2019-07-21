// this is a test for syracuse number mod k
// when k increase, less and less numbers need to be tested in proportion in a k size set

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#include "syracuse_test_numbers_mod_k_v2.h"
#include "biginteger.h"

bool test_iterate(biginteger b, biginteger k)
{
	biginteger newk = k;
	biginteger newb = b;

	while (true)
	{
		if ((newk < k) && (newb <= b))
		{
			//not a minimum
			return false;
		}

		if (is_odd(newk))
			return true; //irresolvable, and can be a minimum
 
		if(is_odd(newb))
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
///////////////////////////////////////////////////////////////////////////////////////////
void syracuse_test_numbers_mod_k_v2()
{
	vector<biginteger> vCandidate,vNewCandidate;
	vCandidate.push_back(0); //every numbers
	biginteger k = 2;

	// test at k level
	for (int m = 1; m < 40; m++)
	{
		vNewCandidate.clear();
		for (int i = 0; i < vCandidate.size(); i++)
		{
			biginteger candidate = vCandidate[i];
			//now we must test candidate and candidate+k/2  vs. (k)
			biginteger candidate1 = candidate;
			biginteger candidate2 = candidate+(k>>1);

			if (test_iterate(candidate1, k))
				vNewCandidate.push_back(candidate1);

			if (test_iterate(candidate2, k))
				vNewCandidate.push_back(candidate2);
		}

		vCandidate = vNewCandidate;
		std::sort(vCandidate.begin(), vCandidate.end());
		cout << "m=" << m << " k=" << k << " numbers=" << vCandidate.size() << " proportion=" << (100.f*vCandidate.size()) / k << " %" << endl;
	
		int iSizeToDisp = std::min<size_t>(vCandidate.size(), 50);

		for (int i = 0; i < iSizeToDisp; i++)
			cout << vCandidate[i] << " ";
		cout << endl << endl;

		k *= 2;
	}
}

