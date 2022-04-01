import math
def bruteMostProfitableCluster(branches):
    mostProfitable = []
    maxProfit = -math.inf
    for i in range(len(branches)):
        tempProfit = 0
        tempList = []
        for j in range(i, len(branches)):
            tempList.append(branches[j][0])
            tempProfit += branches[j][1]
            if tempProfit > maxProfit:
                maxProfit = tempProfit
                mostProfitable = tempList.copy()

    return mostProfitable


def findClusterProfit(branches, l, m, h):
    temp = 0
    left_profit = -math.inf
    #calculate left
    for i in range(m, l - 1, -1):
        temp = temp + branches[i][1]
        if temp > left_profit:
            left_profit = temp

    temp = 0
    right_profit = -math.inf
    #calculate right
    for i in range(m + 1, h + 1):
        temp = temp + branches[i][1]
        if temp > right_profit:
            right_profit = temp

    return max(left_profit + right_profit, left_profit, right_profit)


def smartMostProfitableCluster(branches, l, h):
    if l == h:
        return branches[l][1]

    m = (l + h) // 2

    return max(smartMostProfitableCluster(branches, l, m),
               smartMostProfitableCluster(branches, m + 1, h),
               findClusterProfit(branches, l, m, h))


branches = [('A', 3), ('B', -5), ('C', 2), ('D', 11), ('E', -8), ('F', 9), ('G', -5)]
print(bruteMostProfitableCluster(branches))
print(smartMostProfitableCluster(branches, 0, len(branches) - 1))
