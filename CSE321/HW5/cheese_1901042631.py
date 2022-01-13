class Cheese:  # weight, price pairs
    def __init__(self, weight, price):
        self.weight = weight
        self.price = price
        self.cost = price // weight

    def __lt__(self, other):  # for sorting by cost
        return self.cost < other.cost


def getMaxPrice(cheeses, capacity):
    cheeses.sort(reverse=True)  # by cost (price/weight)
    totalprice = 0
    for i in cheeses:
        cur_weight = int(i.weight)
        cur_price = int(i.price)
        if capacity - cur_weight >= 0:
            capacity -= cur_weight
            totalprice += cur_price
        else:
            fraction = capacity / cur_weight
            totalprice += cur_price * fraction
            break
    return totalprice


cheeses = []
cheeses.append(Cheese(10,60))
cheeses.append(Cheese(40,40))
cheeses.append(Cheese(20,100))
cheeses.append(Cheese(30,120))
capacity = 50

# Function call

print(getMaxPrice(cheeses, capacity))
