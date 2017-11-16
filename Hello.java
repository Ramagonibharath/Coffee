import java.io.*;
import java.util.Scanner;

public class Hello {
        public static void main(String args[]) throws IOException
        {
            // The name of the file to open.
            String fileName = "C:\\Users\\srite\\IdeaProjects\\first\\src\\palindrome.txt";

            // This will reference one line at a time
            String line = null;

            try {
                // FileReader reads text files in the default encoding.
                FileReader fileReader =
                        new FileReader(fileName);

                // Always wrap FileReader in BufferedReader.
                BufferedReader bufferedReader =
                        new BufferedReader(fileReader);

                while((line = bufferedReader.readLine()) != null) {
                    String original, reverse = "";

                    original = line;
                    System.out.print(line);
                    System.out.print("-------");
                    int length = original.length();

                    for ( int i = length - 1; i >= 0; i-- )
                        reverse = reverse + original.charAt(i);

                    if (original.equals(reverse))
                        System.out.println(" YES");
                    else
                        System.out.println("NO");

                }
                bufferedReader.close();
                }
            catch(FileNotFoundException ex) {
                System.out.println(
                        "Unable to open file '" +
                                fileName + "'");
            }

                }

                // Always close files.

            }




