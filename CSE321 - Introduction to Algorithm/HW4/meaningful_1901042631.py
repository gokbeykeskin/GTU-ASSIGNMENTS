def partition(arr, l, r):
    x = arr[r]
    i = l
    for j in range(l, r):
        if (arr[j] <= x):
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
    arr[i], arr[r] = arr[r], arr[i]
    return i


def findKthSmallest(arr, l, r, k):

    if(k<0 or k > r-l+1):
        return -1

    pos = partition(arr, l, r)
    if (pos - l == k - 1): #if pos is kth smallest
        return arr[pos]
    elif (pos - l > k - 1): #if kth smallest is on left of pos
        return findKthSmallest(arr, l, pos - 1, k)
    else: #if kth smallest is on right of posA
        return findKthSmallest(arr, pos + 1, r, k - pos + l - 1)


arr = [7, 9, 55, 17, 4, 219, 6]
print("K'th smallest element is", findKthSmallest(arr, 0, len(arr)-1, 3))
