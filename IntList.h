#pragma once
#include "IntStack.h"
class IntList {

private:
	const int head;
	const IntList * tail;

	IntList(const int head) : head(head), tail (nullptr) {

	}

	IntList(const int X, const IntList * Xs): head(X),tail(Xs) {
		
	}
public:
	static bool isEmpty(const IntList* Xs)  {
		return nullptr == Xs;
	}

	const int gethead(const IntList* Xs) {
		return Xs->head;
	}

	static IntList* empty;

	static const IntList* gettail(const IntList* Xs) {
		return Xs->tail;
	}
	
	static IntList* cons(const int X, const IntList* Xs) {
		return new IntList(X, Xs);
	}

	static const IntList* app(const int xs[], const IntList* Ys) {
		const IntList* Zs = Ys;
		for (int i = (sizeof(xs) / sizeof(*xs)); i >= 0; i--) {
			Zs = cons(xs[i], Zs);
		}
		return Zs;
	}

	IntStack* toInts(const IntList* Xs) {
		IntStack* is = new IntStack();
		while (!isEmpty(Xs)) {
			is->push(gethead(Xs));
			Xs = gettail(Xs);
		}
		return is;
	}

	int len(const IntList* Xs) {
		return toInts(Xs)->size();
	}

};
