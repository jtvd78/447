import math

def x(n):

	if n > 3:
		return 0
	if n < 0:
		return 0
	return n

def sum(func, lower):
	acc = 0

	for i in range(lower, 100):
		acc += func(i)

	return acc


def T(func):

	return lambda n : sum(func, n - 1)

#	return lambda n : func(n ** 2)
#    return lambda n : func(n) * math.cos (3.14 * n)

def D(func):
	return lambda n : func(n - 1)



A = T(D(x))
B = D(T(x))


for i in range(-10, 10):
    print("%d %d %d" % (i, A(i), B(i)))

