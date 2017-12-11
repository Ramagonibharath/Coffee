#pragma once
/**
* representation of a clause
*/
class Clause {
	const int len;
	const int base;
	const int neck;
	const int * xs;
	const int * hgs;
	Clause(const int len, const int * hgs, const int base, const int neck, const int * xs) :len(len), hgs(hgs), base(base), neck(neck), xs(xs) {
	
	}
};
