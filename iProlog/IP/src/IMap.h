#ifndef IMAP
#define IMAP

#include "IntMap.h"
#include "IntStack.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

namespace iProlog
{

    template<typename K>
    class IMap
    {
    private:
        std::map<K, IntMap*> _map;

    public:
        IMap() : _map(std::map<K, IntMap*>())
        {
        }

        void clear()
        {
            _map.clear();
        }

        bool put(K const key, int const val)
        {
            IntMap *vals;
            if (_map.find(key) == _map.end())
            {
                vals = new IntMap();
                _map[key] = vals;
            }
            else
            {
                vals = _map.at(key);
            }
            return vals->add(val);
        }

        IntMap *get(K const key)
        {
            IntMap *s;
            if (_map.find(key) == _map.end())
            {
                s = new IntMap();
            }
            else
            {
                s = _map.at(key);
            }
            return s;
        }

        bool remove(K const key, int const val)
        {
            IntMap * const vals = get(key);
            const bool ok = vals->delete_Renamed(val);
            if (vals->isEmpty())
            {
                _map.erase(key);
            }
            return ok;
        }

        bool remove(K const key)
        {
            return 0 != _map.erase(key);
        }
        /*
        int size()
        {
            const Iterator<K> I = _map.keySet().begin();
            int s = 0;
            while (I->hasNext())
            {
                const K key = I->next();
                IntMap * const vals = get(key);
                s += vals->size();
                I++;
            }
            return s;
        }
        */
        virtual std::string toString()
        {
            StringBuilder * const b = new StringBuilder("{");
            bool first = true;
            typedef typename std::map<K, IntMap*>::iterator intmap_iter;
            for (intmap_iter it = _map.begin(); it != _map.end(); ++it)
            {
                if (!first)
                {
                    b->append(',');
                }
                first = false;
                b->append(it->first);
                b->append("=");
                b->append(it->second->toString());
            }
            b->append("}");
            return b->toString();
        }

        // specialization for array of int maps

        static std::vector<IMap<int>*> create(int const l)
        {
            IMap<int>* const first = new IMap<int>();
            std::vector<IMap<int>*> imaps(l);
            //new IMap[l];
            imaps[0] = first;
            for (int i = 1; i < l; i++)
            {
                imaps[i] = new IMap<int>();
            }
            return imaps;
        }

        static bool put(std::vector<IMap<int>*> &imaps, int const pos, int const key, int const val)
        {
            return imaps[pos]->put(int(key), val);
        }

        static std::vector<int> get(std::vector<IMap<int>*> &iMaps, std::vector<IntMap*> &vmaps, std::vector<int> &keys)
        {
            const int l = iMaps.size();
            std::vector<IntMap*> ms(l);
            std::vector<IntMap*> vms(l);
            int tmpsize = 0;
            for (int i = 0; i < l; i++)
            {
                const int key = keys[i];
                if (0 == key)
                {
                    continue;
                }
                //Main.pp("i=" + i + " ,key=" + key);
                IntMap * const m = iMaps[i]->get(int(key));
                //Main.pp("m=" + m);
                ms[tmpsize] = m;// .push_back(m);
                vms[tmpsize] = vmaps[i]; // .push_back(vmaps[i]);
                tmpsize++;
            }
            std::vector<IntMap*> ims(tmpsize);
            std::vector<IntMap*> vims(tmpsize);

            for (int i = 0; i < tmpsize; i++)
            {
                IntMap * const im = ms[i];
                ims[i] = im;
                IntMap * const vim = vms[i];
                vims[i] = vim;
            }

            //Main.pp("-------ims=" + Arrays.toString(ims));
            //Main.pp("-------vims=" + Arrays.toString(vims));

            IntStack * const cs = IntMap::intersect(ims, vims); // $$$ add vmaps here
            std::vector<int> is = cs->toArray();
            for (int i = 0; i < is.size(); i++)
            {
                is[i] = is[i] - 1;
            }
            std::sort(is.begin(), is.end(), std::less<int>());
            return is;
        }

        static std::string show(std::vector<IMap<int>*> &imaps)
        {
            if (imaps.size() == 0) return "[]";

            std::string result = "[";
            for (std::size_t i = 0; i < imaps.size(); ++i)
            {
                std::ostringstream stm;
                stm << imaps[i]->toString();
                result += stm.str() + ", ";
            }

            // remove the last ", " and add a ']'
            return result.substr(0, result.size() - 2) + ']';

        }

        static std::string show(std::vector<int> &is)
        {
            return StringHelper::toArrayString(is);
        }
    };

    // end

}


#endif	//#ifndef IMAP
