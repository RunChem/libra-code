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

###################################################################
# Tutorial: Building the system (Molecular structure and topology)
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
verbose = 1
Load_PT(U, "elements.dat", verbose)

# System creation section
print "Create empty system"
syst = System()  # chemical system

# Create atoms, link them, define groups
print "Creating atoms and adding them to the system"
syst.CREATE_ATOM( Atom(U,  {"Atom_element":"C","Atom_cm_x":0.0,"Atom_cm_y":0.0,"Atom_cm_z":0.0})  )
syst.CREATE_ATOM( Atom(U,  {"Atom_element":"H","Atom_cm_x":-1.0,"Atom_cm_y":-1.0,"Atom_cm_z":0.0}) )

print "First create atom and then add it to the system"
at = Atom(U, {"Atom_element":"H","Atom_cm_x":-1.0,"Atom_cm_y":1.0,"Atom_cm_z":0.0}) 
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"C","Atom_cm_x":2.0,"Atom_cm_y":0.0,"Atom_cm_z":0.0})    
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"H","Atom_cm_x":3.0,"Atom_cm_y":1.0,"Atom_cm_z":0.0})   
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"H","Atom_cm_x":3.0,"Atom_cm_y":-1.0,"Atom_cm_z":0.0}) 
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"Au","Atom_cm_x":0.0,"Atom_cm_y":0.0,"Atom_cm_z":-2.0}) 
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"Au","Atom_cm_x":1.0,"Atom_cm_y":0.0,"Atom_cm_z":-2.0}) 
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"Au","Atom_cm_x":0.0,"Atom_cm_y":1.0,"Atom_cm_z":-2.0}) 
syst.CREATE_ATOM(at)

at = Atom(U, {"Atom_element":"Au","Atom_cm_x":1.0,"Atom_cm_y":1.0,"Atom_cm_z":-2.0}) 
syst.CREATE_ATOM(at)

print "Showing Atoms"
syst.show_atoms()
print "Showing Bonds"
syst.show_bonds()


print "Linking atoms"
syst.LINK_ATOMS(1,2)
syst.LINK_ATOMS(1,3)
syst.LINK_ATOMS(1,4)
syst.LINK_ATOMS(4,5)
syst.LINK_ATOMS(4,6)

print "Showing Bonds"
syst.show_bonds()

print "Showing Molecules"
syst.show_molecules()

print "Showing Fragments"
syst.show_fragments()

print "Grouping atoms"
print "GROUP_ATOMS([1,2,3],1)"
syst.GROUP_ATOMS([1,2,3],1)
syst.show_fragments()

print "GROUP_ATOMS([4,5,6],2)"
syst.GROUP_ATOMS([4,5,6],2)
syst.show_fragments()

print "GROUP_ATOMS([7,8,9,10],3)"
syst.GROUP_ATOMS([7,8,9,10],3)
syst.show_fragments()


syst.determine_functional_groups()

syst.show_atoms()



