import java.io.*;
import java.util.Scanner;

public class Test {
        public static void main(String args[]) throws IOException
        {
           //Give the required path of the 1000 words
            String fileName = "C:\\Users\\Bomule\\eclipse-workspace\\Chegg\\src\\temp.txt";

            String line = null;

            try {

                FileReader fileReader =
                        new FileReader(fileName);

                BufferedReader bufferedReader =
                        new BufferedReader(fileReader);

                while((line = bufferedReader.readLine()) != null) {
                    String original, reverse = "";

                    original = line;
              
                    System.out.print(line);
                    System.out.print("   ");
                    int length = original.length();

                    for ( int i = length - 1; i >= 0; i-- )
                        reverse = reverse + original.charAt(i);

                    if (original.equals(reverse))
                        System.out.println(" It is a palindrome");
                    else
                        System.out.println("It is not palindrome");

                }
                bufferedReader.close();
                }
            catch(FileNotFoundException ex) {
                System.out.println(
                        "Not able to open the file '" +
                                fileName + "'");
            }

                }

            }




