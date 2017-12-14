package iProlog;
class IntList {

 final int head;
  final IntList tail;

  private IntList(final int head) {
    this.head = head;
    tail = null;
  }

  private IntList(final int X, final IntList Xs) {
    head = X;
    tail = Xs;
  }

  static final boolean isEmpty(final IntList Xs) {
    return null == Xs;
  }

  static final int head(final IntList Xs) {
    return Xs.head;
  }

  static final IntList empty = null;

  static final IntList tail(final IntList Xs) {
    return Xs.tail;
  }

  static final IntList cons(final int X, final IntList Xs) {
    return new IntList(X, Xs);
  }

  static final IntList app(final int[] xs, final IntList Ys) {
    IntList Zs = Ys;
    for (int i = xs.length - 1; i >= 0; i--) {
      Zs = cons(xs[i], Zs);
    }
    return Zs;
  }

  static final IntStack toInts(IntList Xs) {
    final IntStack is = new IntStack();
    while (!isEmpty(Xs)) {
      is.push(head(Xs));
      Xs = tail(Xs);
    }
    return is;
  }

  static final int len(final IntList Xs) {
    return toInts(Xs).size();
  }

  @Override
  public String toString() {
    return toInts(this).toString();
  }
  
  public static int[] random(int seed){
      int[] data;int random;
       data=new int[3];
        for (int i=100,j=0;j<3;i=i*seed,j++){
           random=i*seed+seed*(i-seed);
           data[j]=random;

       }
       return data;

  }
 public static void main(String args[]){
	  int a[]=random(2);
	  IntList headEle = new IntList(a[0]);
	  IntList curList = new IntList(a[1], headEle);
	  System.out.println("Elements = "+ curList.toString());
	  System.out.println("Head -> "+ IntList.head(curList));
	  System.out.println("Tail -> "+ IntList.tail(curList));
	  System.out.println("Length->  "+ IntList.len(curList));
	  System.out.println("Added List with constructor -> " + IntList.cons(a[2], curList));
	  int b[]=random(2);
	  System.out.println("added array List-> "+ IntList.app(new int[]{b[0],b[1],b[2]}, curList));

 
}
}
