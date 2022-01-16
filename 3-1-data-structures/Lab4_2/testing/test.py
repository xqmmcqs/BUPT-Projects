import os
import time

with open("./result.txt", "w") as file:
    for i in (10, 30, 50, 100, 500, 1000, 10000, 1000000):
        for j in (3, 6, 10, 20, 30, 50, 95):
            tot = 0
            for k in range(100):
                time.sleep(1)
                os.system("./data {0} {1}".format(i, j))
                os.system("./main 1 raw.in out.out")
                tot += os.path.getsize("./out.out") / \
                    os.path.getsize("./raw.in")
            file.write("[{0}, {1}, {2}],\n".format(i, j, str(tot / 10)[:9]))
