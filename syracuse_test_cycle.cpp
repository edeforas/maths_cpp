#include <iostream>
#include <string>
#include <cassert>

#include "biginteger.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// convert the cycle to a string
string cycle_to_string(biginteger i)
{

	if (i == 0)
		return "p";

	string s;
	while (i != 0)
	{
		if (i & 1)
			s = "p" + s;
		else
			s = "n" + s;

		i >>= 1;
	}
	return s;
}
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
	assert((i & 1) == 0); // check end by pair
	assert(i & (i << 1) == 0); //check every odd followed by even

	i += 2; //cannot end by odd, so go to next odd finishing cycle

	while(true)
	{
		biginteger u = i & (i >> 1); //bitmask of next invalid odd combination
		if (!u)
			return i;
		i += u; //magic, compute next possible cycle by adding a bit to the xxx1100 sequence
	}

	return -1; //impossible to be here
}
////////////////////////////////////////////////////////////////////////////////
// test if iCycle is a trivial cycle ("100100100...") or a new cycle 
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
			string s = cycle_to_string(iCycle);
			bool bIsTrivial = is_trivial_cycle(iCycle);
			cout << "Cycle found !!!  =" << s << " size=" << s.size() << (bIsTrivial?"  trivial":"  !!! NOT TRIVIAL !!!") << endl ;
		}
	
		iNbTestedCycles++;
		iCycle = next_valid_cycle(iCycle);
	}

	cout << "end of computation, total nb cycles tested=" << iNbTestedCycles  << endl;
}