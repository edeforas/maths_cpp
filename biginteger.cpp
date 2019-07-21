#include "biginteger.h"


bool is_odd(const biginteger& i)
{ return (i & 1) != 0; }

bool is_even(const biginteger& i)
{ return (i & 1) == 0; }

