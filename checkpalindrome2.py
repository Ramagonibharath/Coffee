def checkpalindrome(word):
  word = word.casefold()
  
  rev = reversed(word)
  
  # check if the string is equal to its reverse
  if list(word) == list(rev):
     print("It is palindrome")
  else:
     print("It is not palindrome")
   
word = input("Enter the word you want to check")
checkpalindrome(word)

file  = open("words.txt", "r")
strings = file.read()
if(word in strings):
    print("word found")
else:
    print("word not found")