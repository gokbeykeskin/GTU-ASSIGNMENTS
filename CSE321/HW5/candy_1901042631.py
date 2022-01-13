def cutCandy(prices,length):
    max_prices = [0 for x in range(len(prices)+1)] #empty list with size length
 
    for i in range(1, len(prices)+1):
        temp_val = -99999999999
        for j in range(i):
             temp_val = max(temp_val, prices[j] + max_prices[i-j-1]) #if there is a better option update temp_val
        max_prices[i] = temp_val
 
    return max_prices[length] #last index of max_prices is the highest profit

    
prices = [1, 5, 8, 9, 10, 17, 17, 20]
length = len(prices)
print(cutCandy(prices, length))
