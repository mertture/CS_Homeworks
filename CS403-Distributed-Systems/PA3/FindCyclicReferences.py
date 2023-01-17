from MapReduce import MapReduce

class FindCyclicReferences(MapReduce):
    
    def Map(self, map_input):
        bucketDict = {}
       
        for i in range(len(map_input)):
            convertedKey = self.tupleToString(map_input[i][0], map_input[i][1])
            bucketDict[convertedKey] = 1
        return bucketDict

    def Reduce(self, reduce_input):
        bucketDict = {}
        cyclicDict = {}
        

        for i in range(len(reduce_input)):
            for key in reduce_input[i].keys():
                listKey = self.stringToTuple(key)
                if (listKey[0] > listKey[1]):
                    convertedKey = self.tupleToString(listKey[1], listKey[0])
                    if (convertedKey not in bucketDict):
                        bucketDict[convertedKey] = 1
                    elif (convertedKey in bucketDict):
                        cyclicDict[convertedKey] = 1
                else:
                    convertedKey = self.tupleToString(listKey[0], listKey[1])
                    if (convertedKey not in bucketDict):
                        bucketDict[convertedKey] = 1
                    elif (convertedKey in bucketDict):
                        cyclicDict[convertedKey] = 1
    
        return cyclicDict
    
    def tupleToString(self, number_1, number_2):
        return f"({number_1}, {number_2})"
    
    def stringToTuple(self, string):
        stringList = string[1:-1].split(", ")
        stringList[0] = int(stringList[0])
        stringList[1] = int(stringList[1])
        return stringList