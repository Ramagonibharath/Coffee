#pragma once
#include <stdio.h>
#include <math.h>
#include <string>
#include "IntStack.h"
class IntMap
{
	static const long serialVersionUID = 1L;

	static const int FREE_KEY = 0;

	static const int NO_VALUE = 0;

	/** Keys and values */
	int* m_data;
	int m_data_length;

	/** Do we have 'free' key in the map? */
	bool m_hasFreeKey;
	/** Value of 'free' key */
	int m_freeValue;

	/** Fill factor, must be between (0 and 1) */
	const float m_fillFactor;
	/** We will resize a map once it reaches this size */
	int m_threshold;
	/** Current map size */
	int m_size;

	/** Mask to calculate the original position */
	int m_mask;
	int m_mask2;

	//taken from FastUtil
	static const int INT_PHI = 0x9E3779B9;

	const static int phiMix(const int x) {
		const int h = x * INT_PHI;
		return h ^ h >> 16;
	}

	const int shiftKeys(int pos) {
		// Shift entries with the same hash.
		int last, slot;
		int k;
		int* data = m_data;
		while (true) {
			pos = (last = pos) + 2 & m_mask2;
			while (true) {
				if ((k = data[pos]) == FREE_KEY) {
					data[last] = FREE_KEY;
					return last;
				}
				slot = (phiMix(k) & m_mask) << 1; //calculate the starting slot for the current key
				if (last <= pos ? last >= slot || slot > pos : last >= slot && slot > pos) {
					break;
				}
				pos = pos + 2 & m_mask2; //go to the next entry
			}
			data[last] = k;
			data[last + 1] = data[pos + 1];
		}
	}

	const void rehash(const int newCapacity) {
		m_threshold = (int)(newCapacity / 2 * m_fillFactor);
		m_mask = newCapacity / 2 - 1;
		m_mask2 = newCapacity - 1;

		const int oldCapacity = m_data_length;
		const int* oldData = m_data;

		m_data = new int[newCapacity];
		m_size = m_hasFreeKey ? 1 : 0;

		for (int i = 0; i < oldCapacity; i += 2) {
			const int oldKey = oldData[i];
			if (oldKey != FREE_KEY) {
				put(oldKey, oldData[i + 1]);
			}
		}
	}

	/** Taken from FastUtil implementation */

	/** Return the least power of two greater than or equal to the specified value.
	*
	* <p>Note that this function will return 1 when the argument is 0.
	*
	* @param x a long integer smaller than or equal to 2<sup>62</sup>.
	* @return the least power of two greater than or equal to the specified value.
	*/
	const static long nextPowerOfTwo(long x) {
		if (x == 0)
			return 1;
		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return (x | x >> 32) + 1;
	}

	/** Returns the least power of two smaller than or equal to 2<sup>30</sup>
	* and larger than or equal to <code>Math.ceil( expected / f )</code>.
	*
	* @param expected the expected number of elements in a hash table.
	* @param f the load factor.
	* @return the minimum possible size for a backing array.
	* @throws IllegalArgumentException if the necessary size is larger than 2<sup>30</sup>.
	*/
	const static int arraySize(const int expected, const float f) {
		const long s = fmax(2, nextPowerOfTwo((long)ceil(expected / f)));
		if (s > 1 << 30)
			printf("Too large (%d expected elements with load factor %f)", expected, f);
		return (int)s;
	}

	

public:
	IntMap():IntMap(1 << 2){
	}

	IntMap(const int size):IntMap(size, 0.75f){
	}

	IntMap(const int size, const float fillFactor):m_fillFactor(fillFactor){
		if (fillFactor <= 0 || fillFactor >= 1) {
			printf("FillFactor must be in (0, 1)\n");
			return;
		}
		if (size <= 0) {
			printf("Size must be positive!\n");
		}
		const int capacity = arraySize(size, fillFactor);
		m_mask = capacity - 1;
		m_mask2 = capacity * 2 - 1;
		

		m_data = new int[capacity * 2];
		m_data_length = capacity * 2;
		m_threshold = (int)(capacity * fillFactor);
	}

	const int get(const int key) {
		int ptr = (phiMix(key) & m_mask) << 1;

		if (key == FREE_KEY)
			return m_hasFreeKey ? m_freeValue : NO_VALUE;

		int k = m_data[ptr];

		if (k == FREE_KEY)
			return NO_VALUE; //end of chain already
		if (k == key) //we check FREE prior to this call
			return m_data[ptr + 1];

		while (true) {
			ptr = ptr + 2 & m_mask2; //that's next index
			k = m_data[ptr];
			if (k == FREE_KEY)
				return NO_VALUE;
			if (k == key)
				return m_data[ptr + 1];
		}
	}

	// for use as IntSet - Paul Tarau

	const bool contains(const int key) {
		return NO_VALUE != get(key);
	}

	const bool add(const int key) {
		return NO_VALUE != put(key, 666);
	}

	bool Delete(const int key) {
		return NO_VALUE != remove(key);
	}

	const bool isEmpty() {
		return 0 == m_size;
	}

	const static void intersect0(const IntMap* m, IntMap** maps, IntMap** vmaps, IntStack * r, const int maps_length) {
		const int* data = m->m_data;
		for (int k = 0; k < m->m_data_length; k += 2) {
			bool found = true;
			const int key = data[k];
			if (FREE_KEY == key) {
				continue;
			}
			for (int i = 1; i < maps_length; i++) {
				IntMap * map = maps[i];
				const int val = map->get(key);

				if (NO_VALUE == val) {
					IntMap* vmap = vmaps[i];
					const int vval = vmap->get(key);
					if (NO_VALUE == vval) {
						found = false;
						break;
					}
				}
			}
			if (found) {
				r->push(key);
			}
		}
	}

	const static IntStack* intersect(IntMap** maps, IntMap** vmaps, const int maps_length) {
		IntStack* r = new IntStack();

		intersect0(maps[0], maps, vmaps, r, maps_length);
		intersect0(vmaps[0], maps, vmaps, r, maps_length);
		return r;
	}

	// end changes

	const int put(const int key, const int value) {
		if (key == FREE_KEY) {
			const int ret = m_freeValue;
			if (!m_hasFreeKey) {
				++m_size;
			}
			m_hasFreeKey = true;
			m_freeValue = value;
			return ret;
		}

		int ptr = (phiMix(key) & m_mask) << 1;
		int k = m_data[ptr];
		if (k == FREE_KEY) //end of chain already
		{
			m_data[ptr] = key;
			m_data[ptr + 1] = value;
			if (m_size >= m_threshold) {
				rehash(m_data_length * 2); //size is set inside
			}
			else {
				++m_size;
			}
			return NO_VALUE;
		}
		else if (k == key) //we check FREE prior to this call
		{
			const int ret = m_data[ptr + 1];
			m_data[ptr + 1] = value;
			return ret;
		}

		while (true) {
			ptr = ptr + 2 & m_mask2; //that's next index calculation
			k = m_data[ptr];
			if (k == FREE_KEY) {
				m_data[ptr] = key;
				m_data[ptr + 1] = value;
				if (m_size >= m_threshold) {
					rehash(m_data_length * 2); //size is set inside
				}
				else {
					++m_size;
				}
				return NO_VALUE;
			}
			else if (k == key) {
				const int ret = m_data[ptr + 1];
				m_data[ptr + 1] = value;
				return ret;
			}
		}
	}

	const int remove(const int key) {
		if (key == FREE_KEY) {
			if (!m_hasFreeKey)
				return NO_VALUE;
			m_hasFreeKey = false;
			--m_size;
			return m_freeValue; //value is not cleaned
		}

		int ptr = (phiMix(key) & m_mask) << 1;
		int k = m_data[ptr];
		if (k == key) //we check FREE prior to this call
		{
			const int res = m_data[ptr + 1];
			shiftKeys(ptr);
			--m_size;
			return res;
		}
		else if (k == FREE_KEY)
			return NO_VALUE; //end of chain already
		while (true) {
			ptr = ptr + 2 & m_mask2; //that's next index calculation
			k = m_data[ptr];
			if (k == key) {
				const int res = m_data[ptr + 1];
				shiftKeys(ptr);
				--m_size;
				return res;
			}
			else if (k == FREE_KEY)
				return NO_VALUE;
		}
	}

	const int size() {
		return m_size;
	}

	std::string toString() {
		//return java.util.Arrays.toString(m_data);
		std::string b = "{";
		const int l = m_data_length;
		bool first = true;
		for (int i = 0; i < l; i += 2) {

			const int v = m_data[i];
			if (v != FREE_KEY) {
				if (!first) {
					b = b+ ',';
				}
				first = false;
				b = b + std::to_string(v - 1);
			}
		}
		b = b+ "}";
		return b;
	}

};

