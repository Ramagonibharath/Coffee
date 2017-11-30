#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>

using namespace std;
template <class T>
class ObStack {
   private:
      vector<T> elems;

   public:
      void push(T const&);
            void pop();
                 T top() const;
      bool empty() const {
         return elems.empty();
      }
};
template <class T>
T ObStack<T>::top () const {
   if (elems.empty()) {
      throw out_of_range("Stack<>::top(): empty stack");
   }
      return elems.back();
}

template <class T>
void ObStack<T>::push (T const& elem) {

   elems.push_back(elem);
}

template <class T>
void ObStack<T>::pop () {
   if (elems.empty()) {
      throw out_of_range("Stack<>::pop(): empty stack");
   }

 elems.pop_back();
}
int * randomsdq(int seed){


        static int data[4];int random;

        for (int i=100,j=0;j<4;i=i*seed,j++){
            random=i*seed+seed*(i-seed);
            data[j]=random;
        }
        return data;

}
int main() {

   try {

      ObStack<string> Objectinst;
      string gg=to_string(42);
      int *str1;
      str1=randomsdq(3);
     Objectinst.push(to_string(str1[0]));
     cout<<"Pushing "<<str1[0]<<endl;
	  Objectinst.push(to_string(str1[1]));
	      cout<<"Pushing "<<str1[1]<<endl;
	  Objectinst.push(to_string(str1[2]));
	      cout<<"Pushing "<<str1[2]<<endl;
	  Objectinst.push(to_string(str1[3]));
	      cout<<"Pushing "<<str1[3]<<endl;
      cout << "Top Item: "<<Objectinst.top() << std::endl;
      Objectinst.pop();
       cout <<"Top After pop "<< Objectinst.top() << std::endl;

   } catch (exception const& ex) {
      cerr << "Exception: " << ex.what() <<endl;
      return -1;
   }
}
//}
