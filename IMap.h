#pragma once
#include<algorithm>
#include<unordered_map>
#include "IntMap.h"
template <class K>
class IMap
{
public:
	static const long serialVersionUID = 1;
	const std::unordered_map<K, IntMap*> *map;
public:
	
	IMap() {
		map = new unordered_map<K, IntMap*>();
	}

	void clear() {
		map->clear();
	}

	bool put(const K key, const int val) {
		IntMap* vals = map->at(key);
		if (null == vals) {
			vals = new IntMap();
			map->insert_or_assign(key, vals);
		}
		return vals->add(val);
	}

	const IntMap* get(const K key) {
		IntMap* s = map->at(key);
		if (null == s) {
			s = new IntMap();
		}
		return s;
	}

	const bool remove(const K key, const int val) {
		const IntMap* vals = get(key);
		const bool ok = vals->Delete(val);
		if (vals->isEmpty()) {
			map->erase(key);
		}
		return ok;
	}

	const bool remove(const K key) {
		return null != map->erase(key);
	}

	const int size() {
		const iterator<K> I = map->begin();
		int s = 0;
		while (I!= map->end()) {
			const K key = *I;
			const IntMap*  vals = get(key);
			s += vals.size();
		}
		return s;
	}



	// specialization for array of int maps

	const static IMap<int>* create(const int l) {
		const IMap<int>* first = new IMap<int>();
		//@SuppressWarnings("unchecked")
		const IMap<int>* imaps = new IMap<int>[l];
		imaps[0] = first;
		for (int i = 1; i < l; i++) {
			imaps[i] = new IMap<int>();
		}
		return imaps;
	}

	const static bool put(const IMap<int>* imaps, const int pos, const int key, const int val) {
		return imaps[pos].put(key, val);
	}

	const static int* get(const IMap<int>* iMaps, const IntMap* vmaps, const int* keys, const int iMaps_length) {
		const int l = iMaps_length;
		const Vector<IntMap*> ms;
		const ArrayList<IntMap*> vms; 

		for (int i = 0; i < l; i++) {
			const int key = keys[i];
			if (0 == key) {
				continue;
			}
			//Main.pp("i=" + i + " ,key=" + key);
			const IntMap* m = iMaps[i].get(key);
			//Main.pp("m=" + m);
			ms.push_back(m);
			vms.push_back(vmaps[i]);
		}
		const IntMap** ims = new IntMap*[ms.size()];
		const IntMap** vims = new IntMap*[vms.size()];

		for (int i = 0; i < ms.size(); i++) {
			const IntMap* im = ms.get(i);
			ims[i] = im;
			const IntMap *vim = vms.get(i);
			vims[i] = vim;
		}

		

		const IntStack* cs = IntMap.intersect(ims, vims, ms.size()); // $$$ add vmaps here
		const int* is = cs->toArray();
		for (int i = 0; i < ms.size(); i++) {
			is[i] = is[i] - 1;
		}
		sort(is,is+ ms.size());
		return is;
	}




};

