#pragma once

#include "Clause.h"
#include "Spine.h"
#include "ObStack.h"
#include "IMap.h"
#include "IntMap.h"
#include "IntStack.h"
#include "Toks.h"
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
class Engine
{
private:
	std::vector<std::string> slist;
	IntStack* trail;
	IntStack* ustack;
	ObStack<Spine> * spines;
	/**
	* tags of our heap cells - that can also be seen as
	* instruction codes in a compiled implementation
	*/
	const static int V = 0;
	const static int U = 1;
	const static int R = 2;

	const static int C = 3;
	const static int N = 4;

	const static int A = 5;

	// G - ground?

	const static int BAD = 7;

	/**
	* tags an integer value while fliping it into a negative
	* number to ensure that untagged cells are always negative and the tagged
	* ones are always positive - a simple way to ensure we do not mix them up
	* at runtime
	*/
	const static int tag(const int t, const int w) {
		return -((w << 3) + t);
	}

	/**
	* removes tag after flipping sign
	*/
	const static int detag(const int w) {
		return -w >> 3;
	}

	/**
	* extracts the tag of a cell
	*/
	const static int tagOf(const int w) {
		return -w & 7;
	}

	/**
	* places an identifier in the symbol table
	*/
	const int addSym(std::string sym) {
		int I = syms->at(sym);
		if (0 == I) {
			const int i = syms->size();
			I = i;
			syms->insert_or_assign(sym, I);
			slist.push_back(sym);
		}
		return I;
	}

	/**
	* returns the symbol associated to an integer index
	* in the symbol table
	*/
	const std::string getSym(const int w) {
		if (w < 0 || w >= slist.size())
			return "BADSYMREF=" + w;
		return slist.at(w);
	}

	void makeHeap() {
		makeHeap(MINSIZE);
	}

	void makeHeap(const int size) {
		heap = new int[size];
		clear();
	}

	const int getTop() {
		return top;
	}

	const int setTop(const int top) {
		return this->top = top;
	}

	const void clear() {
		//for (int i = 0; i <= top; i++)
		//heap[i] = 0;
		top = -1;
	}

	
	const void expand() {
		const int l = MINSIZE;
		int* newstack = new int[];
		MINSIZE = l << 1;
		for (int i = 0; i < l;i++)
			heap[i] =  newstack[i];
		heap = newstack;
	}

	void ensureSize(const int more) {
		if (1 + top + more >= MINSIZE) {
			expand();
		}
	}

	static std::vector<int> toNums(const std::vector<Clause> clauses) {
		const int l = clauses.size();
		std::vector<int> cls;
		for (int i = 0; i < l; i++) {
			cls.push_back(i);
		}
		return cls;
	}

	/*
	* encodes string constants into symbols while leaving
	* other data types untouched
	*/
	const int encode(const int t, const std::string s) {
		int w;
		try {
			w = std::stoi(s);
		}
		catch (const int e) {
			if (C == t) {
				w = addSym(s);
			}
			else
				//pp("bad in encode=" + t + ":" + s);
				return tag(BAD, 666);
		}
		return tag(t, w);
	}

	/**
	* true if cell x is a variable
	* assumes that variables are tagged with 0 or 1
	*/
	const static bool isVAR(const int x) {
		//const int t = tagOf(x);
		//return V == t || U == t;
		return tagOf(x) < 2;
	}

	const void setRef(const int w, const int r) {
		heap[detag(w)] = r;
	}

	/**
	* removes binding for variable cells
	* above savedTop
	*/
	void unwindTrail(const int savedTop) {
		while (savedTop < trail->getTop()) {
			const int href = trail->pop();
			// assert href is var

			setRef(href, href);
		}
	}

	/**
	* scans reference chains starting from a variable
	* until it points to an unbound root variable or some
	* non-variable cell
	*/
	const int deref(int x) {
		while (isVAR(x)) {
			const int r = getRef(x);
			if (r == x) {
				break;
			}
			x = r;
		}
		return x;
	}


	const bool unify(const int base) {
		while (!ustack->isEmpty()) {
			const int x1 = deref(ustack->pop());
			const int x2 = deref(ustack->pop());
			if (x1 != x2) {
				const int t1 = tagOf(x1);
				const int t2 = tagOf(x2);
				const int w1 = detag(x1);
				const int w2 = detag(x2);

				if (isVAR(x1)) { /* unb. var. v1 */
					if (isVAR(x2) && w2 > w1) { /* unb. var. v2 */
						heap[w2] = x1;
						if (w2 <= base) {
							trail->push(x2);
						}
					}
					else { // x2 nonvar or older
						heap[w1] = x2;
						if (w1 <= base) {
							trail->push(x1);
						}
					}
				}
				else if (isVAR(x2)) { /* x1 is NONVAR */
					heap[w2] = x1;
					if (w2 <= base) {
						trail->push(x2);
					}
				}
				else if (R == t1 && R == t2) { // both should be R
					if (!unify_args(w1, w2))
						return false;
				}
				else
					return false;
			}
		}
		return true;
	}

	const bool unify_args(const int w1, const int w2) {
		const int v1 = heap[w1];
		const int v2 = heap[w2];
		// both should be A
		const int n1 = detag(v1);
		const int n2 = detag(v2);
		if (n1 != n2)
			return false;
		const int b1 = 1 + w1;
		const int b2 = 1 + w2;
		for (int i = n1 - 1; i >= 0; i--) {
			const int i1 = b1 + i;
			const int i2 = b2 + i;
			const int u1 = heap[i1];
			const int u2 = heap[i2];
			if (u1 == u2) {
				continue;
			}
			ustack->push(u2);
			ustack->push(u1);
		}
		return true;
	}

	

	/**
	* relocates a variable or array reference cell by b
	* assumes var/ref codes V,U,R are 0,1,2
	*/
	const static int relocate(const int b, const int cell) {
		return tagOf(cell) < 3 ? cell + b : cell;
	}

	/**
	* pushes slice[from,to] of array cs of cells to heap
	*/
	const void pushCells(const int b, const int from, const int to, const int base) {
		ensureSize(to - from);
		for (int i = from; i < to; i++) {
			push(relocate(b, heap[base + i]));
		}
	}

	/**
	* pushes slice[from,to] of array cs of cells to heap
	*/
	const void pushCells(const int b, const int from, const int to, const int* cs) {
		ensureSize(to - from);
		for (int i = from; i < to; i++) {
			push(relocate(b, cs[i]));
		}
	}

	/**
	* copies and relocates head of clause at offset from heap to heap
	*/
	const int pushHead(const int b, const Clause C) {
		pushCells(b, 0, C.neck, C.base);
		const int head = C.hgs[0];
		return relocate(b, head);
	}

	/**
	* copies and relocates body of clause at offset from heap to heap
	* while also placing head as the first element of array gs that
	* when returned contains references to the toplevel spine of the clause
	*/
	const int* pushBody(const int b, const int head, const Clause C) {
		pushCells(b, C.neck, C.len, C.base);
		const int l = C.hgs.size();
		int* gs = new int[l];
		gs[0] = head;
		for (int k = 1; k < l; k++) {
			const int cell = C.hgs[k];
			gs[k] = relocate(b, cell);
		}
		return gs;
	}

	/**
	* makes, if needed, registers associated to top goal of a Spine
	* these registers will be reused when matching with candidate clauses
	* note that xs contains dereferenced cells - this is done once for
	* each goal's toplevel subterms
	*/
	const void makeIndexArgs(Spine G, const int goal) {
		if (NULL != G.xs)
			return;

		const int p = 1 + detag(goal);
		const int n = std::min(MAXIND, detag(getRef(goal)));

		int* xs = new int[MAXIND];

		for (int i = 0; i < n; i++) {
			const int cell = deref(heap[p + i]);
			xs[i] = cell2index(cell);
		}

		G.xs = xs;

		if (imaps.empty())
			return;
		int* cs = IMap<int>::get(imaps, vmaps, xs);
		G.cs = cs;
	}

	const int* getIndexables(const int ref) {
		const int p = 1 + detag(ref);
		const int n = detag(getRef(ref));
		int* xs = new int[MAXIND];
		for (int i = 0; i < MAXIND && i < n; i++) {
			const int cell = deref(heap[p + i]);
			xs[i] = cell2index(cell);
		}
		return xs;
	}

	const int cell2index(const int cell) {
		int x = 0;
		const int t = tagOf(cell);
		switch (t) {
		case R:
			x = getRef(cell);
			break;
		case C:
		case N:
			x = cell;
			break;
			// 0 otherwise - assert: tagging with R,C,N <>0
		}
		return x;
	}

	/**
	* tests if the head of a clause, not yet copied to the heap
	* for execution could possibly match the current goal, an
	* abstraction of which has been place in xs
	*/
	const bool match(const int* xs, const Clause C0) {
		for (int i = 0; i < MAXIND; i++) {
			const int x = xs[i];
			const int y = C0.xs[i];
			if (0 == x || 0 == y) {
				continue;
			}
			if (x != y)
				return false;
		}
		return true;
	}

	/**
	* transforms a spine containing references to choice point and
	* immutable list of goals into a new spine, by reducing the
	* first goal in the list with a clause that successfully
	* unifies with it - in which case places the goals of the
	* clause at the top of the new list of goals, in reverse order
	*/
	const Spine* unfold(Spine G) {

		const int ttop = trail->getTop();
		const int htop = getTop();
		const int base = htop + 1;

		const int goal = IntList::gethead(G.gs);

		makeIndexArgs(G, goal);

		const int last = G.cs.size();
		for (int k = G.k; k < last; k++) {
			const Clause C0 = clauses[G.cs[k]];

			if (!match(G.xs, C0))
				continue;

			const int base0 = base - C0.base;
			const int b = tag(V, base0);
			const int head = pushHead(b, C0);

			ustack->clear(); // set up unification stack

			ustack->push(head);
			ustack->push(goal);

			if (!unify(base)) {
				unwindTrail(ttop);
				setTop(htop);
				continue;
			}
			const int* gs = pushBody(b, head, C0);
			const IntList *newgs = IntList::gettail(IntList::app(gs, IntList::gettail(G.gs)));
			G.k = k + 1;
			if (!IntList::isEmpty(newgs))
				return new Spine(gs, base, IntList::gettail(G.gs), ttop, 0, cls);
			else
				return answer(ttop);
		} // end for
		return NULL;
	}
public:
	const static int MAXIND = 3; // number of index args
	const static int START_INDEX = 20;
	int * heap;
	int top;
	// switches off indexing for less then START_INDEX clauses e.g. <20

	/**
	* Builds a new engine from a natural-language style assembler.nl file
	*/
	Engine(const std::string fname) {
		syms = new std::unordered_map<std::string, int>();

		makeHeap();

		trail = new IntStack();
		ustack = new IntStack();

		spines = new ObStack<Spine>();

		clauses = dload(fname);

		cls = toNums(clauses);

		query = init();

		vmaps = vcreate(MAXIND);
		imaps = index(clauses, vmaps);
	}

	/**
	* trimmed down clauses ready to be quickly relocated to the heap
	*/
	std::vector<Clause> clauses;

	std::vector<int> cls;
	/** symbol table made of map + reverse map from ints to syms */

	std::unordered_map<std::string, int> * syms;

	/** runtime areas:
	*
	* the heap contains code for and clauses their their copies
	* created during execution
	*
	* the trail is an undo list for variable bindings
	* that facilitates retrying failed goals with alternative
	* matching clauses
	*
	* the unification stack ustack helps handling term unification non-recursively
	*
	* the spines stack contains abstractions of clauses and goals and performs the
	* functions of  both a choice-point stack and goal stack
	*
	* imaps: contains indexes for up toMAXIND>0 arg positions (0 for pred symbol itself)
	*
	* vmaps: contains clause numbers for which vars occur in indexed arg positions
	*/


	static int MINSIZE; // power of 2

	Spine query;

	const std::vector<IMap<int>> imaps;
	const std::vector<IntMap> vmaps;



	const int size() {
		return top + 1;
	}

	/**
	* dynamic array operation: doubles when full
	*/
	

	/**
	* expands a "Xs lists .." statements to "Xs holds" statements
	*/

	static std::vector<std::vector<std::string>> maybeExpand(const std::vector<std::string> Ws) {
		const std::string W = Ws.at(0);
		std::string temp = "l:";
		std::vector<std::vector<std::string>> Rss;
		if (W.length() < 2 || !temp.compare(W.substr(0, 1)))
			return Rss;

		const int l = Ws.size();
		const std::string V = W.substr(2);
		for (int i = 1; i < l; i++) {
			std::vector<std::string> Rs;
			const std::string Vi = 1 == i ? V : V + "__" + std::to_string(i - 1);
			const std::string Vii = V + "__" + std::to_string(i);
			Rs.push_back("h:" + Vi);
			Rs.push_back("c:list");
			Rs.push_back(Ws.at(i));
			Rs.push_back(i == l - 1 ? "c:nil" : "v:" + Vii);
			Rss.push_back(Rs);
		}
		return Rss;

	}

	/**
	* expands, if needed, "lists" statements in sequence of statements
	*/
	const static std::vector<std::vector<std::string>> mapExpand(const std::vector<std::vector<std::string>> Wss) {
		std::vector<std::vector<std::string>> Rss;
		for (const std::vector<std::string> Ws : Wss) {

			const std::vector<std::vector<std::string>> Hss = maybeExpand(Ws);

			if (Hss.empty()) {
				std::vector<std::string> ws;
				for (int i = 0; i < Ws.size(); i++) {
					ws.push_back(Ws.at(i));
				}
				Rss.push_back(ws);
			}
			else {
				for (std::vector<std::string> X : Hss) {
					Rss.push_back(X);
				}
			}
		}
		return Rss;
	}

	/**
	* loads a program from a .nl file of
	* "natural language" equivalents of Prolog/HiLog statements
	*/
	std::vector<Clause> dload(const std::string s) {
		const bool fromFile = true;
		std::vector<std::vector<std::vector<std::string>>> Wsss = Toks::toSentences(s, fromFile);

		std::vector<Clause> Cs;

		for (std::vector<std::vector<std::string>> Wss : Wsss) {
			// clause starts here

			std::unordered_map<std::string, IntStack*> *refs = new std::unordered_map<std::string, IntStack*>();
			IntStack * cs = new IntStack();
			IntStack * gs = new IntStack();

			const std::vector<std::vector<std::string>> Rss = mapExpand(Wss);
			int k = 0;
			for (std::vector<std::string> ws : Rss) {

				// head or body element starts here

				const int l = ws.size();
				gs->push(tag(R, k++));
				cs->push(tag(A, l));

				for (std::string w : ws) {

					// head or body subterm starts here

					if (1 == w.length()) {
						w = "c:" + w;
					}

					const std::string L = w.substr(2);

					switch (w.at(0)) {
					case 'c':
						cs->push(encode(C, L));
						k++;
						break;
					case 'n':
						cs->push(encode(N, L));
						k++;
						break;
					case 'v': {
						IntStack * Is = refs->at(L);
						if (NULL == Is) {
							Is = new IntStack();
							refs->insert_or_assign(L, Is);
						}
						Is->push(k);
						cs->push(tag(BAD, k)); // just in case we miss this
						k++;
					}
							  break;
					case 'h': {
						IntStack * Is = refs->at(L);
						if (NULL == Is) {
							Is = new IntStack();
							refs->insert_or_assign(L, Is);
						}
						Is->push(k - 1);
						cs->set(k - 1, tag(A, l - 1));
						gs->pop();
					}
							  break;
					default:
						printf("FORGOTTEN=%s" + w.c_str);
					} // end subterm
				} // end element
			} // end clause

			  // linker
			for (std::unordered_map<std::string, IntStack*>::iterator K= refs->begin(); K != refs->end(); ++K){
				IntStack *Is = K->second;

				// finding the A among refs
				int leader = -1;
				int* array = Is->toArray();
				int size = Is->size();
				for (int j = 0; j < size;j++) {
					if (A == tagOf(cs->get(array[j]))) {
						leader = array[j];

						break;
					}
				}
				if (-1 == leader) {
					// for vars, first V others U
					leader = Is->get(0);
					int * array = Is->toArray();
					int size = Is->size();
					for (int j = 0; j < size; j++) {
						if (array[j] == leader) {
							cs->set(array[j], tag(V, array[j]));
						}
						else {
							cs->set(array[j], tag(U, leader));
						}

					}
				}
				else {
					int * array = Is->toArray();
					int size = Is->size();
					for (int j = 0; j < size;j++) {
						if (array[j] == leader) {
							continue;
						}
						cs->set(array[j], tag(R, leader));
					}
				}
			}

			const int neck = 1 == gs->size() ? cs->size() : detag(gs->get(1));
			const int* tgs = gs->toArray();

			const Clause C = putClause(cs->toArray(), tgs, neck);

			Cs.push_back(C);

		} // end clause set

		const int ccount = Cs.size();
		std::vector<Clause> cls;
		for (int i = 0; i < ccount; i++) {
			cls.push_back(Cs.at(i));
		}
		return cls;
	}

	

	/**
	* returns the heap cell another cell points to
	*/
	const int getRef(const int x) {
		return heap[detag(x)];
	}

	/*
	* sets a heap cell to point to another one
	*/
	

	/**
	* raw display of a term - to be overridden
	*/
	std::string showTerm(const int x) {
		return showTerm(exportTerm(x));
	}

	/**
	* raw display of a externalized term
	*/
	//std::string showTerm(const Object O) {
//		if (O instanceof Object[])
//			return Arrays.deepToString((Object[]) O);
//		return O.toString();
//	}

	/**
	* prints out content of the trail
	*/
	void ppTrail() {
		for (int i = 0; i <= trail->getTop(); i++) {
			const int t = trail->get(i);
			printf("trail[%d]=%s:%s",i,showCell(t).c_str,showTerm(t).c_str);
		}
	}

	/**
	* builds an array of embedded arrays from a heap cell
	* representing a term for interaction with an external function
	* including a displayer
	*/
	Object exportTerm(int x) {
		x = deref(x);

		const int t = tagOf(x);
		const int w = detag(x);

		Object res = null;
		switch (t) {
		case C:
			res = getSym(w);
			break;
		case N:
			res = new Integer(w);
			break;
		case V:
			//case U:
			res = "V" + w;
			break;
		case R: {

			const int a = heap[w];
			if (A != tagOf(a))
				return "*** should be A, found=" + showCell(a);
			const int n = detag(a);
			const Object[] arr = new Object[n];
			const int k = w + 1;
			for (int i = 0; i < n; i++) {
				const int j = k + i;
				arr[i] = exportTerm(heap[j]);
			}
			res = arr;
		}
				break;
		default:
			res = "*BAD TERM*" + showCell(x);
		}
		return res;
	}

	/**
	* extracts an integer array pointing to
	* the skeleton of a clause: a cell
	* pointing to its head followed by cells pointing to its body's
	* goals
	*/
	static int* getSpine(const int* cs) {
		const int a = cs[1];
		const int w = detag(a);
		int* rs = new int[w - 1];
		for (int i = 0; i < w - 1; i++) {
			const int x = cs[3 + i];
			const int t = tagOf(x);
			if (R != t) {
				printf("*** getSpine: unexpected tag=%d",t);
				return NULL;
			}
			rs[i] = detag(x);
		}
		return rs;
	}
	

	/**
	* places a clause built by the Toks reader on the heap
	*/
	Clause* putClause(const int* cs, const int* gs, const int neck) {
		const int base = size();
		const int b = tag(V, base);
		const int len = cs.length;
		pushCells(b, 0, len, cs);
		for (int i = 0; i < gs.length; i++) {
			gs[i] = relocate(b, gs[i]);
		}
		const int* xs = getIndexables(gs[0]);
		return new Clause(len, gs, base, neck, xs);
	}
	/**
	* raw display of a cell as tag : value
	*/
	const std::string showCell(const int w) {
		const int t = tagOf(w);
		const int val = detag(w);
		std::string s = NULL;
		switch (t) {
		case V:
			s = "v:" + val;
			break;
		case U:
			s = "u:" + val;
			break;
		case N:
			s = "n:" + val;
			break;
		case C:
			s = "c:" + getSym(val);
			break;
		case R:
			s = "r:" + val;
			break;
		case A:
			s = "a:" + val;
			break;
		default:
			s = "*BAD*=" + w;
		}
		return s;
	}

	/**
	* a displayer for cells
	*/

	std::string showCells(const int base, const int len) {
		std::string buf = "", temp;
		for (int k = 0; k < len; k++) {
			const int instr = heap[base + k];
			temp = "[" + (base + k);
			temp = temp + "]";
			buf.append(temp);
			buf.append(showCell(instr));
			buf.append(" ");
		}
		return buf;
	}

	std::string showCells(const std::vector<int> cs) {
		std::string buf = "", temp;
		for (int k = 0; k < cs.size(); k++) {
			temp = "[" + k;
			temp = temp + "]";
			buf.append(temp);
			buf.append(showCell(cs[k]));
			buf.append(" ");
		}
		return buf;
	}

	/**
	* to be overridden as a printer of a spine
	*/
	void ppc(const Spine C) {
		// override
	}

	/**
	* to be overridden as a printer for current goals
	* in a spine
	*/
	void ppGoals(const IntList gs) {
		// override
	}

	/**
	* to be overriden as a printer for spines
	*/
	void ppSpines() {
		// override
	}

	/**
	* unification algorithm for cells X1 and X2 on ustack that also takes care
	* to trail bindigs below a given heap address "base"
	*/
	

	/**
	* extracts a query - by convention of the form
	* goal(Vars):-body to be executed by the engine
	*/
	Clause getQuery() {
		return clauses[clauses.size() - 1];
	}

	/**
	* returns the initial spine built from the
	* query from which execution starts
	*/
	Spine* init() {
		const int base = size();

		const Clause G = getQuery();
		Spine *Q = new Spine(G.hgs, base, IntList::empty, trail->getTop(), 0, cls);
		spines->push(*Q);
		return Q;
	}

	/**
	* returns an answer as a Spine while recording in it
	* the top of the trail to allow the caller to retrieve
	* more answers by forcing backtracking
	*/
	const Spine* answer(const int ttop) {
		return new Spine(spines->get(0).hd, ttop);
	}

	/**
	* detects availability of alternative clauses for the
	* top goal of this spine
	*/
	const private bool hasClauses(const Spine S) {
		return S.k < S.cs.length;
	}

	/**
	* true when there are no more goals left to solve
	*/
	const private bool hasGoals(const Spine S) {
		return !IntList.isEmpty(S.gs);
	}

	/**
	* removes this spines for the spine stack and
	* resets trail and heap to where they where at its
	* creating time - while undoing variable binding
	* up to that point
	*/
	const private void popSpine() {
		const Spine G = spines.pop();
		unwindTrail(G.ttop);
		setTop(G.base - 1);
	}

	/**
	* main interpreter loop: starts from a spine and works
	* though a stream of answers, returned to the caller one
	* at a time, until the spines stack is empty - when it
	* returns null
	*/
	const Spine yield() {
		while (!spines.isEmpty()) {
			const Spine G = spines.peek();
			if (!hasClauses(G)) {
				popSpine(); // no clauses left
				continue;
			}
			const Spine C = unfold(G);
			if (null == C) {
				popSpine(); // no matches
				continue;
			}
			if (hasGoals(C)) {
				spines.push(C);
				continue;
			}
			return C; // answer
		}
		return null;
	}

	/**
	* retrieves an answers and ensure the engine can be resumed
	* by unwinding the trail of the query Spine
	* returns an external "human readable" representation of the answer
	*/
	Object ask() {
		query = yield();
		if (null == query)
			return null;
		const int res = answer(query.ttop).hd;
		const Object R = exportTerm(res);
		unwindTrail(query.ttop);
		return R;
	}

	/**
	* initiator and consumer of the stream of answers
	* generated by this engine
	*/
	void run() {
		long ctr = 0L;
		for (;; ctr++) {
			const Object A = ask();
			if (null == A) {
				break;
			}
			if (ctr<5) Prog.println("[" + ctr + "] " + "*** ANSWER=" + showTerm(A));
		}
		if (ctr>5) Prog.println("...");
		Prog.println("TOTAL ANSWERS=" + ctr);
	}

	// indexing extensions - ony active if START_INDEX clauses or more

	public static IntMap[] vcreate(const int l) {
		const IntMap[] vss = new IntMap[l];
		for (int i = 0; i < l; i++) {
			vss[i] = new IntMap();
		}
		return vss;
	}

	const static void put(const IMap<Integer>[] imaps, const IntMap[] vss, const int[] keys, const int val) {
		for (int i = 0; i < imaps.length; i++) {
			const int key = keys[i];
			if (key != 0) {
				IMap.put(imaps, i, key, val);
			}
			else {
				vss[i].add(val);
			}
		}
	}

	const IMap<Integer>* index(const Clause** clauses, const IntMap** vmaps) {
		if (clauses.length < START_INDEX)
			return null;

		const IMap<Integer>[] imaps = IMap.create(vmaps.length);
		for (int i = 0; i < clauses.length; i++) {
			const Clause c = clauses[i];

			put(imaps, vmaps, c.xs, i + 1); // $$$ UGLY INC

		}
		Main.pp("INDEX");
		Main.pp(IMap.show(imaps));
		Main.pp(Arrays.toString(vmaps));
		Main.pp("");
		return imaps;
	}
};
