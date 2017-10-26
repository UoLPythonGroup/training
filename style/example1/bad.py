import random

# Number of trials
n = 10000

m = 0
while n > 0:
    x = random.random()
    y = random.random()
    m += (x*x + y*y <= 1)
    n -= 1

print("Pi is about", m / 2500)