#pragma once
#include "IntList.h"
/**
* runtime representation of an immutable list of goals
* together with top of heap and trail pointers
* and current clause tried out by head goal
* as well as registers associated to it
*
* note that parts of this immutable lists
* are shared among alternative branches
*/
class Spine {
private:
	const int hd; // head of the clause to which this corresponds
	const int base; // top of the heap when this was created

	const IntList* gs; // goals - with the top one ready to unfold
	const int ttop; // top of the trail when this was created

	int k;

	int* xs; // index elements
	int* cs; // array of  clauses known to be unifiable with top goal in gs
public:
	/**
	* creates a spine - as a snapshot of some runtime elements
	*/
	Spine(const int* gs0, const int base, const IntList* gs, const int ttop, const int k, int* cs):hd(gs0[0]),base(base),gs(IntList::gettail(IntList::app(gs0, gs))),ttop(ttop),k(k),cs(cs) {
	}

	/**
	* creates a specialized spine returning an answer (with no goals left to solve)
	*/
	Spine(const int hd, const int ttop):hd(hd),ttop(ttop),base(0) {
		gs = IntList::empty;
		k = -1;
		cs = nullptr;
	}
};
