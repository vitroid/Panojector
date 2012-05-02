from math import *
#this works very well.
width = 4000 #3000.0
height = width / 2
#x = [443.0, 1143.0, 1855.0, 2654.0] #//y axis of the four corners, in picture coord
#y = [485., 986.] #// x axis of the first column, in pucture coord
x = [494,1489,2487,3485] #//y axis of the four corners, in picture coord
y = [649,1183] #// x axis of the first column, in pucture coord
#//four corners must be vertically aligned.
#296,1303,1702,2709
#346,776
#525,1520,2489,3491
#624,1199
#494,1489,2487,3485
#493,1183
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
print "./panojector -s %s imagefile.jpg box" % int(width),
print "-b %s,%s,%s" % (1,q+r,h1+h2),
print "-e %s,%s,%s" % (p,q/(q+r),h2/(h1+h2)),
print "-x %s" % (x[0]*2.0/width)

            
        
        



    
    


