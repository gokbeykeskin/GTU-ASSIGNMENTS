from math import floor


def minMax(start, end, a):
    if start == end:
        return a[start], a[start]
    elif end == start + 1:
        if a[start] > a[end]:
            return a[end], a[start]
        else:
            return a[start], a[end]
    else:
        min1, max1 = minMax(start, floor((start + end) / 2), a)
        min2, max2 = minMax(floor((start + end) / 2) + 1, end, a)
        return min(min1, min2), max(max1, max2)


a = [10, 70, 30, 90, 98, 1, 3, 5, 0, 7]
print(minMax(0, len(a) - 1, a))
