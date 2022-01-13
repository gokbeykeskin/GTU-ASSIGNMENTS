from typing import List

    
def partition(arr, low, high, pivot):

	i = low
	j = low
	while j < high: 
		if (arr[j] < pivot): 
			arr[i], arr[j] = arr[j], arr[i]
			i += 1
		elif (arr[j] == pivot):
			arr[j], arr[high] = arr[high], arr[j]
			j -= 1
		j += 1
	arr[i], arr[high] = arr[high], arr[i]
	return i
    
def matchPairs(boxes, gifts, low, high):
    if (low < high):
        pivot = partition(boxes, low, high, gifts[low])#rearrange boxes. pivot of the boxes is gifts last element
        partition(gifts, low, high, boxes[pivot]) #rearrenge gifts according to pivot from boxes partition

        #repeat for both subarrays
        matchPairs(boxes, gifts, low, pivot - 1)
        matchPairs(boxes, gifts, pivot + 1, high)



boxes = [5,3,9,1,2,7,4,6,8] #size of the boxes
gifts = [9,1,7,8,2,5,4,6,3] #size of the gifts
print("Unmatched gifts and boxes are : ")
print(str(gifts) + " \n" + str(boxes))
matchPairs(gifts, boxes, 0, 8)
print("Matched gifts and boxes are : ")
print(str(gifts) + " \n" + str(boxes))

