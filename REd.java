import java.io.*;
import java.util.Scanner;

public class REd {
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
                    System.out.println(line);

                }
                bufferedReader.close();
                }
            catch(FileNotFoundException ex) {
                System.out.println(
                        "Unable to open file '" +
                                fileName + "'");
                System.out.println("Please set the correct path");
            }

                }

                // Always close files.

            }




