import random

def stocastically_approximate_pi(trials):
    in_circle_count = 0
    for _ in range(trials):
        x = random.random()
        y = random.random()
        if x * x + y * y <= 1.0:
            in_circle_count += 1
    return in_circle_count * 4 / trials

for n in [1000, 10000, 100000]:
    print("From {} trials we estimate Pi as {}".format(
        n, stocastically_approximate_pi(n)))
