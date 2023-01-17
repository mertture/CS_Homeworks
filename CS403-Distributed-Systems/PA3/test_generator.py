from random import randint    
    
    
numLines = 5
lineNums = []
for i in range(numLines):
    newNum = randint(5, 421582)
    while newNum in lineNums:
        newNum = randint(5, 421582)
    lineNums.append(newNum)
maxElt = max(lineNums)

f = open("test_file" + str(numLines) + ".txt", "w")

myData = []
count = 0
with open("Cit-HepPh.txt") as fp:
    for i, line in enumerate(fp):
        if i > maxElt:
            break
        for j in range(numLines):
            if i == lineNums[j]:
                count = count + 1
                #print(count)
                f.write(line)
                elts = line.rstrip().split("\t")
                myData.append((int(elts[0]), int(elts[1])))
f.close()
