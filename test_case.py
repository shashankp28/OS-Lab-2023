import random

N = 1000

unused = [i for i in range(26)]
used = []

print(1)
print()
print(11, 1)
print()

for _ in range(N):
    weight = len(used) / (len(used) + len(unused))
    if random.uniform(0, 1) < weight:
        i = random.choice(used)
        print(chr(i+65), 0)
        used.remove(i)
        unused.append(i)
    else:
        i = random.choice(unused)
        print(chr(i+65), random.randint(2, 100))
        unused.remove(i)
        used.append(i)
print()