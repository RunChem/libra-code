###################################################################
# Tutorial: Grouping atoms and functional group determination
###################################################################

import os
import sys
import math

# Fisrt, we add the location of the library to test to the PYTHON path
cwd = os.getcwd()
print "Current working directory", cwd
sys.path.insert(1,cwd+"/../../_build/src/mmath")
sys.path.insert(1,cwd+"/../../_build/src/chemobjects")


print "\nTest 1: Importing the library and its content"
from cygmmath import *
from cygchemobjects import *
from LoadPT import * #import LoadPT

###################################################################

# Create Universe and populate it
U = Universe()
verbose = 0
Load_PT(U, "elements.dat", verbose)

# System creation section
print "Create empty system"
syst = System()  # chemical system

# Create atoms, link them, define groups
print "Creating atoms and adding them to the system"
syst.CREATE_ATOM( Atom(U,  {"Atom_element":"C","Atom_cm_x":0.0,"Atom_cm_y":0.0,"Atom_cm_z":0.0})  )
syst.CREATE_ATOM( Atom(U,  {"Atom_element":"H","Atom_cm_x":-1.0,"Atom_cm_y":-1.0,"Atom_cm_z":0.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"H","Atom_cm_x":-1.0,"Atom_cm_y":1.0,"Atom_cm_z":0.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"C","Atom_cm_x":2.0,"Atom_cm_y":0.0,"Atom_cm_z":0.0})    )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"H","Atom_cm_x":3.0,"Atom_cm_y":1.0,"Atom_cm_z":0.0})   )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"H","Atom_cm_x":3.0,"Atom_cm_y":-1.0,"Atom_cm_z":0.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"Au","Atom_cm_x":0.0,"Atom_cm_y":0.0,"Atom_cm_z":-2.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"Au","Atom_cm_x":1.0,"Atom_cm_y":0.0,"Atom_cm_z":-2.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"Au","Atom_cm_x":0.0,"Atom_cm_y":1.0,"Atom_cm_z":-2.0}) )
syst.CREATE_ATOM( Atom(U, {"Atom_element":"Au","Atom_cm_x":1.0,"Atom_cm_y":1.0,"Atom_cm_z":-2.0}) )

print "Linking atoms"
syst.LINK_ATOMS(1,2)
syst.LINK_ATOMS(1,3)
syst.LINK_ATOMS(1,4)
syst.LINK_ATOMS(4,5)
syst.LINK_ATOMS(4,6)

print "Grouping atoms"
syst.GROUP_ATOMS([1,2,3],1)
syst.GROUP_ATOMS([4,5,6],2)
syst.GROUP_ATOMS([7,8,9,10],3)
syst.show_fragments()


print "Atoms before functional group determination"
syst.show_atoms()

syst.determine_functional_groups(0)  # 0 - no need for ring determination

print "Atoms after functional group determination"
syst.show_atoms()

