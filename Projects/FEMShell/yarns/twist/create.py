from math import *

def tofile(name:str, fiber:list):
        f = open(name,'w')
        for pos in fiber:
            f.write('{},{},{}\n'.format(pos[0],pos[1],pos[2]))
    

def tolist(offset, step, count):
    l = []
    for i in range(count):
        theta = (step *(offset+i))/180*pi
        x = cos(theta)
        z = sin(theta)
        l.append((x,z,i))
    return l
    
if __name__ == '__main__':
    count = 6
    step = 360/6 #degree

    for fid in range(count):
        l = tolist(fid, step, int(360/step))
        tofile("yarn_"+str(fid)+".txt", l)
    