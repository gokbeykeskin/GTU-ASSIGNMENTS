def countSubstr(string, start, end):
    count = 0
    for i in range(len(string)):
        substr = ''
        for j in range(i, len(string)):
            substr += string[j]
            if substr[0] == start and substr[len(substr) - 1] == end:
                count += 1
    return count


print(countSubstr('TXZXXJZWX','X','Z'))