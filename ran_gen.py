import random

num  = input('How many numbers do you want to generate?: ')
maximum = input('Enter max number: ')
minimum = input('Enter minimum: ')

f = open("ranNums.txt","w+")
f.truncate()

for x in range(num):
  f.write("%d\n" % random.randint(minimum, maximum))
  
f.write("\n")
  
f.close()