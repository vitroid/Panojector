#!/usr/bin/env python
from math import *
#this works very well.
import sys
args = map(float,tuple(sys.argv[1:8]))
width = args[0]     # 1st argument: width of the image
x     = args[1:5]   # 2nd-5th args: x positions of the corners.
y     = args[5:7]   # 6th-7th args: y positions of the ceiling and floor at the 1st corner.
for i in range(0,3):
    if x[i] > x[i+1]:
        x[i+1] += width


height = width / 2

def angles(param):
    p,q,r=param
    a1 = atan(p/q)+atan((1-p)/q)
    a2 = atan(q/(1-p))+atan(r/(1-p))
    a3 = atan(p/r)+atan((1-p)/r)
    a4 = atan(r/p)+atan(q/p)
    return (a1,a2,a3,a4)


def sum(a):
    s = 0
    for i in range(4):
        s += a[i]
    return s*180/pi
    
def devi(a,b):
    s = 0.0
    for i in range(4):
        s += (a[i] - b[i])**2
    return s

def fit(phi):
    param=[0.5,0.5,0.5]
    delta=0.1

    bestp = list(param)
    a = angles(bestp)
    bests = devi(phi,a)
    #print bestp,bests,sum(phi)

    while delta > 0.0001:
        newbestp = None
        for d in ( (delta,0,0), (-delta,0,0), (0,delta,0), (0,-delta,0), (0,0,delta), (0,0,-delta)):
            p = bestp[0]+d[0], bestp[1]+d[1], bestp[2]+d[2]
            s = devi(phi,angles(p))
            if s < bests:
                bests = s
                newbestp = p
                #print p,s,sum(angles(p))
        if newbestp == None:
            delta /= 2
        else:
            bestp = newbestp
    return p

phi = [(x[1]-x[0])*2*pi/width,
       (x[2]-x[1])*2*pi/width,
       (x[3]-x[2])*2*pi/width,
       (x[0]+width-x[3])*2*pi/width]
p,q,r = fit(phi)
l = sqrt(p**2+q**2)

theta = [(height/2-y[0])*pi/height,
         (y[1]-height/2)*pi/height]

h1 = l*tan(theta[0])
h2 = l*tan(theta[1])
#print p,q,r,h1,h2
print "./panojector -s %s" % int(width),
print "box -b %s,%s,%s" % (1,q+r,h1+h2),
print "-e %s,%s,%s" % (p,q/(q+r),h2/(h1+h2)),
print "slide -x %s" % (x[0]*2.0/width),
print "interpolate equi.jpg"

            
        
        



    
    


