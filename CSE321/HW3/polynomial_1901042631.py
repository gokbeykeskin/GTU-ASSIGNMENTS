def bruteForcePolynomial(coefficients, x):
    res = 0
    order = len(coefficients) - 1
    for coefficient in coefficients:
        power = 1
        for i in range(0, order):
            power = power * x
        res += coefficient * power
        order -= 1
    return res


def smartPolynomial(coefficients, x):
    res = coefficients[len(coefficients) - 1]
    power = 1
    for i in reversed(range(0, len(coefficients) - 1)):
        power = power * x

        res += coefficients[i] * power

    return res


print(bruteForcePolynomial([2,8,10],9))
print(smartPolynomial([2,8,10],9))