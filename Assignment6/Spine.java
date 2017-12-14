import java.util.*;

/**
 * runtime representation of an immutable list of goals
 * together with top of heap and trail pointers
 * and current clause tried out by head goal
 * as well as registers associated to it
 *
 * note that parts of this immutable lists
 * are shared among alternative branches
 */
class Spine {

  /**
   * creates a spine - as a snapshot of some runtime elements
   */

  Spine(final int[] gs0, final int base,  final int ttop, final int k, final int[] cs) {
    hd = gs0[0];
    this.base = base;
    this.ttop = ttop;
    this.k = k;
    this.cs = cs;

    System.out.println("The head  :"+hd);
    System.out.println("The ttop  :"+ttop);
    System.out.println("The base  :"+base);
    System.out.println("The value of k :"+k);
    System.out.println("The value of cs :"+Arrays.toString(cs));
  }

  /**
   * creates a specialized spine returning an answer (with no goals left to solve)
   */
  Spine(final int hd, final int ttop) {
    this.hd = hd;
    base = 0;
   // gs = IntList.empty;
    this.ttop = ttop;
    System.out.println("The head of the :"+hd);
    System.out.println("The ttop of the :"+ttop);
    k = -1;
    cs = null;
  }

  final int hd;
  final int base;
  final int ttop;
  int k;
  int[] xs;
  int[] cs;
  
  public static int[] random(int seed){
      int[] data;int random;
       data=new int[3];
        for (int i=100,j=0;j<3;i=i*seed,j++){
           random=i*seed+seed*(i-seed);
           data[j]=random;

       }
       return data;

  }
  
  public static void main(String args[])
  {	 
	 int r[]=random(2);
	 int[] ac= {1,2};
	 int[] aab= {3,4};
	 System.out.println("Calling constructors");
	 Spine b=new Spine(r[0],r[1]);
	 System.out.println("HT Constructor finish");
	 System.out.println("Calling main Five constructor");
	 Spine a=new Spine(ac,r[0],r[0],r[0],ac);
	 System.out.println("Constructor terminated");
  }
}
