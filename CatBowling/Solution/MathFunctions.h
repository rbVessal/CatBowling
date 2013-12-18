#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

// Factorial
inline unsigned __int16 Factorial(const unsigned __int16 NUMBER)
{
	unsigned __int16 total = 1;

	for( __int16 i = 1; i < NUMBER + 1; ++i )
			total *= i;

	return total;
}


#endif


