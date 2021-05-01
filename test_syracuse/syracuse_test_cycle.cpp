#include <iostream>
#include <string>
#include <cassert>

#include "biginteger.h"
using namespace std;

// Test for possibles Syracuse cycles
// cycles are encoded into a binary pattern in a long long int:
// bit =1 => odd transformation, bit=0 => even transformation
// cycles minimum start with a odd transformation, so with the first MSB=1
// this implies also that cycles ends with LSB =0 (even)
// also all cycles cannot contain xx11yy ( two odd following)

////////////////////////////////////////////////////////////////////////////////
// convert the cycle to a string
string cycle_to_string(biginteger i)
{
	if (i == 0)
		return "p"; // valid if x==0 but Syracuse start with 1

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
// test if iCycle is a valid cycle
bool test_cycle(biginteger iCycle)
{
	//assume the actual cycle is valid
	assert((iCycle & 1) == 0); // check end by even
	assert((iCycle & (iCycle >> 1)) == 0); //check every odd followed by even 

	biginteger iMask = keep_msb(iCycle);
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

	// test if a/b== 1 i.e. function is identity
	// test if cycle is not one even (valid with x=0, but in Syracuse definition) 
	return (a == b) && (iCycle!=0); 
}
////////////////////////////////////////////////////////////////////////////////
biginteger next_valid_cycle(biginteger i) //compute the next valid cycle
{
	//assume the actual cycle is valid
	assert((i & 1) == 0); // check end by pair
	assert((i & (i >> 1)) == 0); //check every odd followed by even

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
			return false; //not a trivial cycle ! !!! WOW
	}
	return true; //all trivial cycle removed
}
////////////////////////////////////////////////////////////////////////////////
void syracuse_test_cycle()
{
	cout << "test all cycle with size = 1 to 40" << endl;
	// compute the global affine function and test if a integer is the solution

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
			cout << "Cycle found !!!  =" << s << " size=" << s.size() << (bIsTrivial?" but is trivial":"  !!! NOT TRIVIAL !!!") << endl ;
		}
	
		iNbTestedCycles++;
		iCycle = next_valid_cycle(iCycle);
	}

	cout << "end of computation, total nb cycles tested=" << iNbTestedCycles  << endl;
}