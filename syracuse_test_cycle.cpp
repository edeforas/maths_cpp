#include <iostream>
#include <cassert>

#include "biginteger.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//compute the size of the sequence, i.e. the pos of the msb
int msb_pos(biginteger i)
{
	if (i == 0)
		return 0;

	int iPos = 0;
	while (i != 0)
	{
		i >>= 1;
		iPos++;
	}
	return iPos-1;
}
////////////////////////////////////////////////////////////////////////////////
// test if iSequence is a valid cycle
bool test_cycle(biginteger iCycle)
{
	if (iCycle & 1)
		return 0; //since first msb is 1 by definition, the lsb cannot be 0 or we have 11 in the cycle
	
	if ((iCycle & (iCycle << 1)) != 0)
		return 0; // two odd following: ( i.e. xx11yy bit), not possible

	int msb = msb_pos(iCycle);
	biginteger iMask = (1LL << msb);
	biginteger a = 1, b = 1;

	//test the sequence
	while (iMask != 0)
	{
		if (iCycle & iMask) //odd
			a = 3 * a + b;
		else //even
			b = 2 * b;

		iMask >>= 1;
	}

	return a == b; // test if a/b== 1 i.e. function is identity
}
////////////////////////////////////////////////////////////////////////////////
biginteger next_valid_cycle(biginteger i) //compute the next valid cycle
{
	//assume the actual cycle is valid
	assert((i & 1) == 0);
	assert(i & (i << 1) == 0); //simple & is intentionnal

	i += 2;

	while(true)
	{
		biginteger u = i & (i << 1); //pos of next invalid odd odd combination
		if (!u)
			return i;
		i = (i | (u - 1)) + 1; //magic
	}

	return -1; //impossible to be here
}
////////////////////////////////////////////////////////////////////////////////
// if is iSequence is a trivial cycle ("100100100...") or a new cycle 
bool is_trivial_cycle(biginteger iCycle)
{
	//remove all trivial cycle end as "xxyyxyxy100" to "xxyyxyxy"
	while (iCycle)
	{
		int trivial = iCycle & 7; //7=0b111 get 3 cycles
		if (trivial == 4) //4 means 0x100, so odd,even,even = trivial cycle
			iCycle >>= 3; //discard the trivial cycle
		else
			return false; //no a trivial cycle ! !!! WOW
	}
	return true; //all trivial removed
}
////////////////////////////////////////////////////////////////////////////////
void syracuse_test_cycle()
{
	cout << "test all cycle with size =1 to 40" << endl;
	cout << "compute the global affine function and test if a integer is the solution" << endl;

	//use the bitmask iSequence
	//even is bit=0
	//odd is bit=1
	//every cycle start with odd number so msb=1
	biginteger iCycle = 0;
	
	int iNbTestedCycles = 0;

	while(iCycle< (1LL << 40))
	{
		if (test_cycle(iCycle))
		{
			int iSize = msb_pos(iCycle)+1;

			if (is_trivial_cycle(iCycle))
				cout << "trivial cycle found=" << iCycle << " size=" << iSize << endl;
			else
				cout << "NONtrivial cycle found !!!!!! WOW  =" << iCycle << " size=" << iSize << endl;
		}
	
		iNbTestedCycles++;
		iCycle = next_valid_cycle(iCycle);
	}

	cout << "end of computation, total nb cycles tested=" << iNbTestedCycles  << endl;
}