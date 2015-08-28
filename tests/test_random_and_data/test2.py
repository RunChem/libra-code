#*********************************************************************************
#* Copyright (C) 2015 Alexey V. Akimov
#*
#* This file is distributed under the terms of the GNU General Public License
#* as published by the Free Software Foundation, either version 2 of
#* the License, or (at your option) any later version.
#* See the file LICENSE in the root directory of this distribution
#* or <http://www.gnu.org/licenses/>.
#*
#*********************************************************************************/

import os
import sys
import math

# Fisrt, we add the location of the library to test to the PYTHON path
cwd = os.getcwd()
print "Current working directory", cwd
sys.path.insert(1,cwd+"/../../_build/src/mmath")


print "\nTest 1: Importing the library and its content"
print "from cygmmath import *"
from cygmmath import *



def boltz():
# This function generates x and p taken from the Boltzmann distribution
    #rnd = Random()

    Er = 2.39e-2
    omega = 3.5e-4
    kT = 9.5e-4
    mo2 = 0.5*omega*omega # mass = 1
    M = math.sqrt(mo2*Er)


    X_ = -0.5*M/mo2             # minimum
    p_ = math.sqrt(kT)          # momentum that corresponds to temperatures
    x_ = X_ + 50.0*rnd.normal() # + 0.5*(M/mo2)*rnd.normal()  #(M/mo2)*

    
    Eold = mo2*x_*x_ + M*x_ + 0.5*p_*p_    # energy
    Enew = 0.0


    for i in xrange(1000):
        #print i, x_
        x = x_ + 10.0*rnd.uniform(-1.0, 1.0) #50.0*rnd.normal()  #0.1*(M/mo2)*rnd.normal()  # proposed state x
        p = p_ + 1.1*math.sqrt(kT)*rnd.uniform(-1.0, 1.0) #rnd.normal() # proposed state p

        Enew = mo2*x*x + M*x + 0.5*p*p
        dE = Enew - Eold

        ksi = rnd.uniform(0.0,1.0)
        prob = 1.0
        argg = dE/kT
        if argg >40:
            prob = 0.0
        elif argg < -40:
            prob = 1.0
        else: 
            prob = math.exp(-dE/kT)  #min(1.0, math.exp(-dE/kT))

        if(ksi<prob):  # accept new state with Metropolis scheme
            Eold = Enew
            x_ = x
            p_ = p

    return [x_, p_]



# Define it only once
rnd = Random()

# Here, we test the accuracy of random distribution generated by the Metropolis scheme

Er = 2.39e-2
omega = 3.5e-4
kT = 9.5e-4
mo2 = 0.5*omega*omega # mass = 1
M = math.sqrt(mo2*Er)
x_ = -0.5*M/mo2           # minimum
p_ = math.sqrt(kT)        # momentum that corresponds to temperatures


x = []
p = []
for i in range(0,5000):    
    [a, b] = boltz()
    x.append(a)
    p.append(b)
    
X = DATA(x)
P = DATA(p)


ax = []
spread = 500.0
dx = 2.0*spread / 100.0
for i in range(0,100):   
    ax.append(x_ - spread + i*dx)


ap = []
spread = 1.0*math.sqrt(kT)
dp = 4.0*spread / 100.0
for i in range(0,100):   
    ap.append(-2.0*math.sqrt(kT) + i*dp)


Xdens = X.Calculate_Distribution(ax)[0]
Pdens = P.Calculate_Distribution(ap)[0]




f = open("metropol_x.txt","w")
i = 0
sz = len(ax)
Z = 0.0
Z1 = 0.0
for i in range(0, sz):
    prob = math.exp(-( mo2*ax[i]*ax[i] + M*ax[i] + 0.5*p_*p_ )/kT)
    Z = Z + prob * dx
    Z1 = Z1 + Xdens[i] * dx

for i in range(0, sz):
    prob = math.exp(-( mo2*ax[i]*ax[i] + M*ax[i] + 0.5*p_*p_ )/kT) 
    f.write("%8.5f  %8.5f  %8.5f  \n" % (ax[i], Xdens[i]/Z1,  prob/Z ) )

f.close()


f = open("metropol_p.txt","w")
i = 0
sz = len(ap)
Z = 0.0
Z1 = 0.0
for i in range(0, sz):
    prob = math.exp(-( mo2*x_*x_ + M*x_ + 0.5*ap[i]*ap[i] )/kT)
    Z = Z + prob * dp
    Z1 = Z1 + Pdens[i] * dp

for i in range(0, sz):
    prob = math.exp(-( mo2*x_*x_ + M*x_ + 0.5*ap[i]*ap[i] )/kT) 
    f.write("%8.5f  %8.5f  %8.5f  \n" % (ap[i], Pdens[i]/Z1,  prob/Z ) )

f.close()


