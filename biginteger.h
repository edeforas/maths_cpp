#ifndef biginteger_
#define biginteger_

#define biginteger long long //for now

#define MAX_BIGINTEGER (1LL<<62)

bool is_odd(const biginteger& i);
bool is_even(const biginteger& i);
biginteger keep_msb(biginteger i);

#endif