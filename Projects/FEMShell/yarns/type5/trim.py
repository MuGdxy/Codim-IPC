
for i in range(0,120):
    s = "yarn_" + str(i) + ".txt";
    f = open("origin/" + s, "r")
    of = open(s, "w")
    for j in range(0, 1388):
        line = f.readline()
        of.write(line)
    f.close()
    of.close()