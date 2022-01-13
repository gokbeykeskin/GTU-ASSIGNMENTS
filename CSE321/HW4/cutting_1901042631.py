import math


def cut(n):
    if n == 1:
        return 0
    return cut(math.ceil(n / 2)) + 1


print(cut(5))
