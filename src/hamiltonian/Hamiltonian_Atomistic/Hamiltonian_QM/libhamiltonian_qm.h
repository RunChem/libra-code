/*********************************************************************************
* Copyright (C) 2015-2017 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/
/**
  \file libhamiltonian_qm.h
  \brief The file describes Python export function
    
*/

#ifndef LIB_HAMILTONIAN_QM_H
#define LIB_HAMILTONIAN_QM_H


#include "Hamiltonian_QM.h"
#include "SCF.h"

/// liblibra namespace
namespace liblibra{


/// libhamiltonian namespace
namespace libhamiltonian{

/// libhamiltonian_atomistic namespace
namespace libhamiltonian_atomistic{

/// libhamiltonian_qm namespace
namespace libhamiltonian_qm{


void export_Hamiltonian_QM_objects();



}// namespace libhamiltonian_qm
}// namespace libhamiltonian_atomistic
}// namespace libhamiltonian
}// liblibra

#endif// LIB_HAMILTONIAN_QM_H
