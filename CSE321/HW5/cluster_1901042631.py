def maxProfitableCluster(arr):
    maximum = -999999
    temp_max = 0
    for i in arr:
        temp_max = max(i, i+temp_max)
        if(temp_max>maximum):
            maximum = temp_max
    return maximum


print(maxProfitableCluster([3,-5,2,11,-8,9,-5]))
