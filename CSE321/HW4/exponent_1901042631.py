from math import floor

def powerBrute(x, y):
    res = 1
    while (y > 0):
        res *= x
        y -= 1
    return res


def powerDQ(x, y):
    if (y == 0):
        return 1  # base case

    elif (y % 2 == 0):  # x^4 = x^(4/2) * x^(4/2)
        return (powerDQ(x, y / 2) * powerDQ(x, y / 2))
    else:  # x^5 = x * x^2 * x^2
        return (x * powerDQ(x, floor(y / 2)) * powerDQ(x, floor(y / 2)))


print(powerBrute(4, 3))
print(powerDQ(4, 3))