import java.util.Arrays;

class IntStack {

  private int stack[];

  private int top;

  static int SIZE = 16; 

  static int MINSIZE = 1 << 15; 

  IntStack() {
    this(SIZE);
  }

  IntStack(final int size) {
    stack = new int[size];
    clear();
  }

  final int getTop() {
    return top;
  }

  final int setTop(final int top) {
    return this.top = top;
  }

  final void clear() {
 
    top = -1;
  }

  final boolean isEmpty() {
    return top < 0;
  }

  final void push(final int i) {

    if (++top >= stack.length) {
      expand();
    }
    stack[top] = i;
  }

  final int pop() {
    final int r = stack[top--];
    shrink();
    return r;
  }

  final int get(final int i) {
    return stack[i];
  }

  final void set(final int i, final int val) {
    stack[i] = val;
  }

  final int size() {
    return top + 1;
  }


  private final void expand() {
    final int l = stack.length;
    final int[] newstack = new int[l << 1];

    System.arraycopy(stack, 0, newstack, 0, l);
    stack = newstack;
  }


  private final void shrink() {
    int l = stack.length;
    if (l <= MINSIZE || top << 2 >= l)
      return;
    l = 1 + (top << 1); 
    if (top < MINSIZE) {
      l = MINSIZE;
    }

    final int[] newstack = new int[l];
    System.arraycopy(stack, 0, newstack, 0, top + 1);
    stack = newstack;
  }

  int[] toArray() {
    final int[] array = new int[size()];
    if (size() > 0) {
      System.arraycopy(stack, 0, array, 0, size());
    }
    return array;
  }

  public final void reverse() {
    int l = size();
    int h = l >> 1;
    for (int i = 0; i < h; i++) {
      int temp = stack[i];
      stack[i] = stack[l - i - 1];
      stack[l - i - 1] = temp;
    }
  }
 public static int[] randomm()  {
      int seed=88;int[] data;int random;
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
          System.out.println(random);
      }
      
      return data;
      }

  @Override
  public String toString() {
    return Arrays.toString(toArray());
  }

public static void main(final String[] args) {
    IntStack s=new IntStack();
    int[] a=randomm();
s.push(a[0]);
s.push(a[1]);
s.push(a[2]);
s.push(a[3]);
s.push(a[4]);
//s.push(a[5]);
System.out.println("The size of stack is: ");
System.out.println(s.getTop()+1);


  }
}
