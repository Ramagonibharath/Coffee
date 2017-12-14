#ifndef SPINE
#define SPINE

#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace iProlog { class IntList; }

namespace iProlog
{
	/// <summary>
	/// runtime representation of an immutable list of goals
	/// together with top of heap and trail pointers
	/// and current clause tried out by head goal
	/// as well as registers associated to it
	/// 
	/// note that parts of this immutable lists
	/// are shared among alternative branches
	/// </summary>
	class Spine
	{

	  /// <summary>
	  /// creates a spine - as a snapshot of some runtime elements
	  /// </summary>
  public:
	  virtual ~Spine()
	  {
		  delete gs;
	  }

	  Spine(std::vector<int> const &gs0, int const base, IntList *const gs, int const ttop, int const k, std::vector<int> const &cs);

	  /// <summary>
	  /// creates a specialized spine returning an answer (with no goals left to solve)
	  /// </summary>
	  Spine(int const hd, int const ttop);

	  const int hd; // head of the clause to which this corresponds
	  const int base; // top of the heap when this was created

	  IntList* gs; // goals - with the top one ready to unfold
	  const int ttop; // top of the trail when this was created

	  int k;

	  std::vector<int> xs; // index elements
	  std::vector<int> cs; // array of  clauses known to be unifiable with top goal in gs

	private:
		void InitializeInstanceFields();
	};

}


#endif	//#ifndef SPINE
