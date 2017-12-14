#ifndef INTMAP
#define INTMAP

#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "stringhelper.h"
#include "stringbuilder.h"
#include "IntStack.h"

/// <summary>
/// derived from code at https://github.com/mikvor/hashmapTest
/// </summary>
namespace iProlog
{
	class IntMap
	{
  private:
	  static const long long serialVersionUID = 1LL;

	  static const int FREE_KEY = 0;

  public:
	  static const int NO_VALUE = 0;

	  /// <summary>
	  /// Keys and values </summary>
  private:
	  std::vector<int> m_data;

	  /// <summary>
	  /// Do we have 'free' key in the map? </summary>
	  bool m_hasFreeKey;
	  /// <summary>
	  /// Value of 'free' key </summary>
	  int m_freeValue;

	  /// <summary>
	  /// Fill factor, must be between (0 and 1) </summary>
	  const float m_fillFactor;
	  /// <summary>
	  /// We will resize a map once it reaches this size </summary>
	  int m_threshold;
	  /// <summary>
	  /// Current map size </summary>
	  int m_size;

	  /// <summary>
	  /// Mask to calculate the original position </summary>
	  int m_mask;
	  int m_mask2;

  public:

	  IntMap(int const size = 4, float const fillFactor = 0.75);

	  int get(int const key);

	  // for use as IntSet - Paul Tarau

	  bool contains(int const key);

	  bool add(int const key);

	  virtual bool delete_Renamed(int const key);

	  bool isEmpty();

	  static void intersect0(IntMap *const m, std::vector<IntMap*> &maps, std::vector<IntMap*> &vmaps, IntStack *const r);

	  static IntStack *intersect(std::vector<IntMap*> &maps, std::vector<IntMap*> &vmaps);

	  // end changes

	  int put(int const key, int const value);

	  int remove(int const key);

  private:
	  int shiftKeys(int pos);

  public:
	  int size();

  private:
	  void rehash(int const newCapacity);

	  /// <summary>
	  /// Taken from FastUtil implementation </summary>

	  /// <summary>
	  /// Return the least power of two greater than or equal to the specified value.
	  /// 
	  /// <para>Note that this function will return 1 when the argument is 0.
	  /// 
	  /// </para>
	  /// </summary>
	  /// <param name="x"> a long integer smaller than or equal to 2<sup>62</sup>. </param>
	  /// <returns> the least power of two greater than or equal to the specified value. </returns>
	  static long long nextPowerOfTwo(long long x);

	  /// <summary>
	  /// Returns the least power of two smaller than or equal to 2<sup>30</sup>
	  /// and larger than or equal to <code>Math.ceil( expected / f )</code>.
	  /// </summary>
	  /// <param name="expected"> the expected number of elements in a hash table. </param>
	  /// <param name="f"> the load factor. </param>
	  /// <returns> the minimum possible size for a backing array. </returns>
	  /// <exception cref="IllegalArgumentException"> if the necessary size is larger than 2<sup>30</sup>. </exception>
	  static int arraySize(int const expected, float const f);

	  //taken from FastUtil
	  static const int INT_PHI = 0x9E3779B9;

	  static int phiMix(int const x);

  public:
	  virtual std::string toString();


	private:
		void InitializeInstanceFields();
	};

}


#endif	//#ifndef INTMAP
