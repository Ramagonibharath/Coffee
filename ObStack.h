#pragma once
#include<vector>
template <class T>
class ObStack
{
private:
	std::vector<T> arraylist;
	static const long serialVersionUID = 1L;
public:
	const T pop() {
		return arraylist.pop_back();
	}

	void push(const T O) {
		arraylist.push_back(O);
	}

	const T peek() {
		return arraylist.at(arraylist.size() - 1);
	}
	ObStack() {
		arraylist = new vector<T>();
	}
};


