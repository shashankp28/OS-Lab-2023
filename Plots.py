import subprocess
import matplotlib.pyplot as plt

f = open("random.txt", "w")
random = []
for i in range(1, 61):
    command = f"g++ random.cpp -o random && ./random 60 {i} 60 ./Test_files/req4.dat"
    completed_process = subprocess.run(command, stdout=subprocess.PIPE, shell=True)
    output_str = completed_process.stdout.decode()
    f.write(output_str.split("\n")[0])
    f.write(", ")
    random.append(int(output_str.split("\n")[0]))
    print(i, " Done!")
f.close()

f = open("lru.txt", "w")
lru = []
for i in range(1, 61):
    command = f"g++ lru.cpp -o lru && ./lru 60 {i} 60 ./Test_files/req4.dat"
    completed_process = subprocess.run(command, stdout=subprocess.PIPE, shell=True)
    output_str = completed_process.stdout.decode()
    f.write(output_str.split("\n")[0])
    f.write(", ")
    lru.append(int(output_str.split("\n")[0]))
    print(i, " Done!")
f.close()

f = open("fifo.txt", "w")
fifo = []
for i in range(1, 61):
    command = f"g++ fifo.cpp -o fifo && ./fifo 60 {i} 60 ./Test_files/req4.dat"
    completed_process = subprocess.run(command, stdout=subprocess.PIPE, shell=True)
    output_str = completed_process.stdout.decode()
    f.write(output_str.split("\n")[0])
    f.write(", ")
    fifo.append(int(output_str.split("\n")[0]))
    print(i, " Done!")
f.close()

print(fifo)
print(lru)
print(random)
x = [i for i in range(1, 61)]
plt.plot(x, lru, label='LRU')
plt.plot(x, fifo, label='FIFO')
plt.plot(x, random, label='Random')

plt.xlabel('Number of frames')
plt.ylabel('Page faults')
plt.title('Page faults vs No. of frames - req4.dat')
plt.legend()
plt.savefig('req4.png')
plt.show()

