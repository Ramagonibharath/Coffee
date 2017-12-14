#ifndef INTSTACK
#define INTSTACK

#include <string>
#include <vector>

/// <summary>
/// Dynamic Stack for int data.
/// </summary>
namespace iProlog
{

	class IntStack
	{

  private:
	  std::vector<int> stack;

	  int top;

  public:
	  static int SIZE; // power of 2

	  static int MINSIZE; // power of 2

	  IntStack();

	  IntStack(int const size);

	  int getTop();

	  int setTop(int const top);

	  void clear();

	  bool isEmpty();

	  /// <summary>
	  /// Pushes an element - top is incremented first than the
	  /// element is assigned. This means top point to the last assigned
	  /// element - which can be returned with peek().
	  /// </summary>
	  void push(int const i);

	  int pop();

	  int get(int const i);

	  void set(int const i, int const val);

	  int size();

	  /// <summary>
	  /// dynamic array operation: doubles when full
	  /// </summary>
  private:
	  void expand();

	  /// <summary>
	  /// dynamic array operation: shrinks to 1/2 if more than than 3/4 empty
	  /// </summary>
	  void shrink();

  public:
	  virtual std::vector<int> toArray();

	  void reverse();

	  virtual std::string toString();


	private:
		void InitializeInstanceFields();
	};

}


#endif	//#ifndef INTSTACK
