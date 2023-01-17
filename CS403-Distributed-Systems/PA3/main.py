import sys
from FindCitations import FindCitations
from FindCyclicReferences import FindCyclicReferences

if __name__ == '__main__':
    
    numOfWorkers = sys.argv[2]
    filename = sys.argv[3]
    if (sys.argv[1] == "COUNT"):
        print("Find Citations called")
        findCitations = FindCitations(int(numOfWorkers))
        findCitations.start(filename)

    elif (sys.argv[1] == "CYCLE"):
        print("Find Cyclic References called")
        findCyclicReferences = FindCyclicReferences(int(numOfWorkers))
        findCyclicReferences.start(filename)
