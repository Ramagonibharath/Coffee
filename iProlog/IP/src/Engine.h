#ifndef ENGINE
#define ENGINE

#include <string>
#include <vector>
#include <stdexcept>
#include <list>
#include <algorithm>
#include <map>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace iProlog { class Clause; }
namespace iProlog { class IntStack; }
namespace iProlog { template<typename T>class ObStack; }
namespace iProlog { class Spine; }
namespace iProlog { template<typename K>class IMap; }
namespace iProlog { class IntMap; }
namespace iProlog { class IntList; }
namespace iProlog { class TermObject; }

namespace iProlog
{

	/// <summary>
	/// Implements execution mechanism
	/// </summary>
	class Engine
	{

  public:
	  static int MAXIND; // number of index args
	  static int START_INDEX;
	  // switches off indexing for less then START_INDEX clauses e.g. <20

	  /// <summary>
	  /// Builds a new engine from a natural-language style assembler.nl file
	  /// </summary>
	  virtual ~Engine()
	  {
		  delete syms;
		  delete trail;
		  delete ustack;
		  delete spines;
		  delete query;
	  }

	  Engine(const std::string &fname);

	  /// <summary>
	  /// trimmed down clauses ready to be quickly relocated to the heap
	  /// </summary>
	  std::vector<Clause*> clauses;

	  std::vector<int> cls;
	  /// <summary>
	  /// symbol table made of map + reverse map from ints to syms </summary>

	  std::map<std::string, int> *syms;
  private:
	  std::vector<std::string> slist;

	  /// <summary>
	  /// runtime areas:
	  /// 
	  /// the heap contains code for and clauses their their copies
	  /// created during execution
	  /// 
	  /// the trail is an undo list for variable bindings
	  /// that facilitates retrying failed goals with alternative
	  /// matching clauses
	  /// 
	  /// the unification stack ustack helps handling term unification non-recursively
	  /// 
	  /// the spines stack contains abstractions of clauses and goals and performs the
	  /// functions of  both a choice-point stack and goal stack
	  /// 
	  /// imaps: contains indexes for up toMAXIND>0 arg positions (0 for pred symbol itself)
	  /// 
	  /// vmaps: contains clause numbers for which vars occur in indexed arg positions
	  /// </summary>

	  std::vector<int> heap;
	  int top;
  public:
	  static int MINSIZE; // power of 2

  private:
	  IntStack * trail;
	  IntStack * ustack;
	  ObStack<Spine*> * spines;

  public:
	  Spine *query;

	  std::vector<IMap<int>*> imaps;
	  std::vector<IntMap*> vmaps;

	  /// <summary>
	  /// tags of our heap cells - that can also be seen as
	  /// instruction codes in a compiled implementation
	  /// </summary>
  private:
	  static const int V = 0;
	  static const int U = 1;
	  static const int R = 2;

	  static const int C = 3;
	  static const int N = 4;

	  static const int A = 5;

	  // G - ground?

	  static const int BAD = 7;

	  /// <summary>
	  /// tags an integer value while fliping it into a negative
	  /// number to ensure that untagged cells are always negative and the tagged
	  /// ones are always positive - a simple way to ensure we do not mix them up
	  /// at runtime
	  /// </summary>
	  static int tag(int const t, int const w);

	  /// <summary>
	  /// removes tag after flipping sign
	  /// </summary>
	  static int detag(int const w);

	  /// <summary>
	  /// extracts the tag of a cell
	  /// </summary>
	  static int tagOf(int const w);

	  /// <summary>
	  /// places an identifier in the symbol table
	  /// </summary>
	  int addSym(const std::string &sym);

	  /// <summary>
	  /// returns the symbol associated to an integer index
	  /// in the symbol table
	  /// </summary>
	  std::string getSym(int const w);

	  void makeHeap();

	  void makeHeap(int const size);

	  int getTop();

	  int setTop(int const top);

	  void clear();

	  /// <summary>
	  /// Pushes an element - top is incremented frirst than the
	  /// element is assigned. This means top point to the last assigned
	  /// element - which can be returned with peek().
	  /// </summary>
	  void push(int const i);

  public:
	  int size();

	  /// <summary>
	  /// dynamic array operation: doubles when full
	  /// </summary>
  private:
	  void expand();

	  void ensureSize(int const more);

	  /// <summary>
	  /// expands a "Xs lists .." statements to "Xs holds" statements
	  /// </summary>

	  static std::vector<std::vector<std::string> > maybeExpand(const std::vector<std::string> &Ws);

	  /// <summary>
	  /// expands, if needed, "lists" statements in sequence of statements
	  /// </summary>
	  static std::vector<std::vector<std::string> > mapExpand(const std::vector<std::vector<std::string> > &Wss);

	  /// <summary>
	  /// loads a program from a .nl file of
	  /// "natural language" equivalents of Prolog/HiLog statements
	  /// </summary>
  public:
	  virtual std::vector<Clause*> dload(const std::string &s);

  private:
	  static std::vector<int> toNums(std::vector<Clause*> &clauses);

	  /*
	   * encodes string constants into symbols while leaving
	   * other data types untouched
	   */
	  int encode(int const t, const std::string &s);

	  /// <summary>
	  /// true if cell x is a variable
	  /// assumes that variables are tagged with 0 or 1
	  /// </summary>
	  static bool isVAR(int const x);

	  /// <summary>
	  /// returns the heap cell another cell points to
	  /// </summary>
  public:
	  int getRef(int const x);

	  /*
	   * sets a heap cell to point to another one
	   */
  private:
	  void setRef(int const w, int const r);

	  /// <summary>
	  /// removes binding for variable cells
	  /// above savedTop
	  /// </summary>
	  void unwindTrail(int const savedTop);

	  /// <summary>
	  /// scans reference chains starting from a variable
	  /// until it points to an unbound root variable or some
	  /// non-variable cell
	  /// </summary>
	  int deref(int x);

	  /// <summary>
	  /// raw display of a term - to be overridden
	  /// </summary>
  public:
	  virtual std::string showTerm(int const x);

	  /// <summary>
	  /// raw display of a externalized term
	  /// </summary>
	  virtual std::string showTerm(TermObject const &O);

	  /// <summary>
	  /// prints out content of the trail
	  /// </summary>
	  virtual void ppTrail();

	  /// <summary>
	  /// builds an array of embedded arrays from a heap cell
	  /// representing a term for interaction with an external function
	  /// including a displayer
	  /// </summary>
	  virtual TermObject exportTerm(int x);

	  /// <summary>
	  /// extracts an integer array pointing to
	  /// the skeleton of a clause: a cell
	  /// pointing to its head followed by cells pointing to its body's
	  /// goals
	  /// </summary>
	  static std::vector<int> getSpine(std::vector<int> &cs);

	  /// <summary>
	  /// raw display of a cell as tag : value
	  /// </summary>
	  std::string showCell(int const w);

	  /// <summary>
	  /// a displayer for cells
	  /// </summary>

	  virtual std::string showCells(int const base, int const len);

	  virtual std::string showCells(std::vector<int> &cs);

	  /// <summary>
	  /// to be overridden as a printer of a spine
	  /// </summary>
	  virtual void ppc(Spine *const C);

	  /// <summary>
	  /// to be overridden as a printer for current goals
	  /// in a spine
	  /// </summary>
	  virtual void ppGoals(IntList *const gs);

	  /// <summary>
	  /// to be overriden as a printer for spines
	  /// </summary>
	  virtual void ppSpines();

	  /// <summary>
	  /// unification algorithm for cells X1 and X2 on ustack that also takes care
	  /// to trail bindigs below a given heap address "base"
	  /// </summary>
  private:
	  bool unify(int const base);

	  bool unify_args(int const w1, int const w2);

	  /// <summary>
	  /// places a clause built by the Toks reader on the heap
	  /// </summary>
  public:
	  virtual Clause *putClause(std::vector<int> cs, std::vector<int> &gs, int const neck);

	  /// <summary>
	  /// relocates a variable or array reference cell by b
	  /// assumes var/ref codes V,U,R are 0,1,2
	  /// </summary>
  private:
	  static int relocate(int const b, int const cell);

	  /// <summary>
	  /// pushes slice[from,to] of array cs of cells to heap
	  /// </summary>
	  void pushCells(int const b, int const from, int const to, int const base);

	  /// <summary>
	  /// pushes slice[from,to] of array cs of cells to heap
	  /// </summary>
	  void pushCells(int const b, int const from, int const to, std::vector<int> &cs);

	  /// <summary>
	  /// copies and relocates head of clause at offset from heap to heap
	  /// </summary>
	  int pushHead(int const b, Clause *const C);

	  /// <summary>
	  /// copies and relocates body of clause at offset from heap to heap
	  /// while also placing head as the first element of array gs that
	  /// when returned contains references to the toplevel spine of the clause
	  /// </summary>
	  std::vector<int> pushBody(int const b, int const head, Clause *const C);

	  /// <summary>
	  /// makes, if needed, registers associated to top goal of a Spine
	  /// these registers will be reused when matching with candidate clauses
	  /// note that xs contains dereferenced cells - this is done once for
	  /// each goal's toplevel subterms
	  /// </summary>
	  void makeIndexArgs(Spine *const G, int const goal);

	  std::vector<int> getIndexables(int const ref);

	  int cell2index(int const cell);

	  /// <summary>
	  /// tests if the head of a clause, not yet copied to the heap
	  /// for execution could possibly match the current goal, an
	  /// abstraction of which has been place in xs
	  /// </summary>
	  bool match(std::vector<int> &xs, Clause *const C0);

	  /// <summary>
	  /// transforms a spine containing references to choice point and
	  /// immutable list of goals into a new spine, by reducing the
	  /// first goal in the list with a clause that successfully
	  /// unifies with it - in which case places the goals of the
	  /// clause at the top of the new list of goals, in reverse order
	  /// </summary>
	  Spine *unfold(Spine *const G);

	  /// <summary>
	  /// extracts a query - by convention of the form
	  /// goal(Vars):-body to be executed by the engine
	  /// </summary>
  public:
	  virtual Clause *getQuery();

	  /// <summary>
	  /// returns the initial spine built from the
	  /// query from which execution starts
	  /// </summary>
	  virtual Spine *init();

	  /// <summary>
	  /// returns an answer as a Spine while recording in it
	  /// the top of the trail to allow the caller to retrieve
	  /// more answers by forcing backtracking
	  /// </summary>
  private:
	  Spine *answer(int const ttop);

	  /// <summary>
	  /// detects availability of alternative clauses for the
	  /// top goal of this spine
	  /// </summary>
	  bool hasClauses(Spine *const S);

	  /// <summary>
	  /// true when there are no more goals left to solve
	  /// </summary>
	  bool hasGoals(Spine *const S);

	  /// <summary>
	  /// removes this spines for the spine stack and
	  /// resets trail and heap to where they where at its
	  /// creating time - while undoing variable binding
	  /// up to that point
	  /// </summary>
	  void popSpine();

	  /// <summary>
	  /// main interpreter loop: starts from a spine and works
	  /// though a stream of answers, returned to the caller one
	  /// at a time, until the spines stack is empty - when it
	  /// returns null
	  /// </summary>
  public:
	  Spine *yield();

	  /// <summary>
	  /// retrieves an answers and ensure the engine can be resumed
	  /// by unwinding the trail of the query Spine
	  /// returns an external "human readable" representation of the answer
	  /// </summary>
	  virtual TermObject ask();

	  /// <summary>
	  /// initiator and consumer of the stream of answers
	  /// generated by this engine
	  /// </summary>
	  virtual void run();

	  // indexing extensions - ony active if START_INDEX clauses or more

	  static std::vector<IntMap*> vcreate(int const l);

	  static void put(std::vector<IMap<int>*> &imaps, std::vector<IntMap*> &vss, std::vector<int> const &keys, int const val);

	  std::vector<IMap<int>*> index(std::vector<Clause*> &clauses, std::vector<IntMap*> &vmaps);

	private:
		void InitializeInstanceFields();
	};

}


#endif	//#ifndef ENGINE
