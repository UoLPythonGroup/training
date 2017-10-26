import numpy as np

def stocastically_approximate_pi(trials):
    """Sample `trials` number of points uniformly at random in the unit square
    and return four times the fraction that fall within a circle of radius 1
    centred at the origin, thus approximating :math:`\pi`."""
    return np.sum(np.sum(np.random.random(size=(trials,2))**2, axis=1) <= 1) * 4 / trials

for n in [1000, 10000, 100000]:
    print("From {} trials we estimate Pi as {}".format(
        n, stocastically_approximate_pi(n)))
