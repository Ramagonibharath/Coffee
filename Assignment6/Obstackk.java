import java.util.*;
import java.util.ArrayList;
class ObStack<T> extends ArrayList<T> {

  private static final long serialVersionUID = 1L;

  final T pop() {
    final int last = this.size() - 1;
    return this.remove(last);
  }

  final void push(final T O) {
    add(O);
  }

  final T peek() {
    return get(this.size() - 1);
  }
 
}


public class Obstackk {
	 public static int[] randomm()  {
	      int seed=3;int[] data;int random;
	      data=new int[10];
	    /*  for (int i=100,j=1;i<105;i++,j++){
	          random=i*seed+seed*(i-seed);
	          data[j]=random;
	          System.out.println(random);
	      }
	      */
	      
	      for (int i=100,j=0;j<5;i=i*seed,j++){
	          random=i*seed+seed*(i-seed);
	          data[j]=random;
	         // System.out.println(random);
	      }
	      
	      return data;
	      }

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ObStack<String> ob=new ObStack<String>();
		int[] a=randomm();
		String b=Integer.toString(a[0]);
		ob.push(b);
		System.out.println("Pushing "+ b);
		String ba=Integer.toString(a[1]);
		ob.push(ba);
		System.out.println("Pushing "+ ba);
		String bc=Integer.toString(a[2]);
		ob.push(bc);
		System.out.println("Pushing "+ bc);
		String bcd=Integer.toString(a[3]);
		ob.push(bcd);
		System.out.println("Pushing "+ bcd);
		System.out.println("Top "+ob.peek());
		ob.pop();
        System.out.println("Top After Pop "+ob.peek());
	}

}

