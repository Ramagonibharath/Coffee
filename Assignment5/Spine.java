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
    //this.gs = IntList.tail(IntList.app(gs0, gs)); // prepends the goals of clause with head hs
    this.ttop = ttop;
    this.k = k;
    this.cs = cs;

    System.out.println("The head  :"+hd);
    System.out.println("The ttop  :"+ttop);
    System.out.println("The base  :"+base);
    System.out.println("The value of k :"+k);
    //for(int i=0;i<cs.length;i++)
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

  final int hd; // head of the clause to which this corresponds
  final int base; // top of the heap when this was created

  //final IntList gs; // goals - with the top one ready to unfold
  final int ttop; // top of the trail when this was created

  int k;

  int[] xs; // index elements
  int[] cs; // array of  clauses known to be unifiable with top goal in gs
  public static void main(String args[])
  {
	  int[] ac= {1,2};
	  int[] aab= {3,4};
	  System.out.println("Calling the head and tail constructor");
	 Spine b=new Spine(10,12);
	 System.out.println("End of the HT Constructor");
	 System.out.println("Calling the main Five constructor");
	 
	 Spine a=new Spine(ac,10,10,10,ac);
	 System.out.println("End of the Constructor");
  }
}
