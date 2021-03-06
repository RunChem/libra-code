/*********************************************************************************
* Copyright (C) 2015 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/
/**
  \file RigidBody_methods4.cpp
  \brief The file implements RigidBody rotation methods

*/

#include "RigidBody.h"

/// liblibra namespace
namespace liblibra{


/// librigidbody namespace
namespace librigidbody{

void RigidBody::Rotate_I_x(double phi){
/**
  Rotation around center of mass along the x
  axis in external coordinate system (lab frame)

  \param[in] phi The rotation angle, in radians
*/

  MATRIX3x3 R; R.Rx(phi);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate_I_y(double phi){
/**
  Rotation around center of mass along the y
  axis in external coordinate system (lab frame)

  \param[in] phi The rotation angle, in radians
*/

  MATRIX3x3 R; R.Rx(phi);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate_I_z(double phi){
/**
  Rotation around center of mass along the z
  axis in external coordinate system (lab frame)

  \param[in] phi The rotation angle, in radians
*/

  MATRIX3x3 R; R.Rx(phi);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate_e_x(double phi){
/**
  Rotation around center of mass along the x
  axis in body-fixed coordinate system (body frame)

  \param[in] phi The rotation angle, in radians
*/

  VECTOR ux,uy,uz;
  rb_A_I_to_e.get_vectors(ux,uy,uz);
  MATRIX3x3 R; R.Rotation(phi*ux);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate_e_y(double phi){
/**
  Rotation around center of mass along the y
  axis in body-fixed coordinate system (body frame)

  \param[in] phi The rotation angle, in radians
*/

  VECTOR ux,uy,uz;
  rb_A_I_to_e.get_vectors(ux,uy,uz);
  MATRIX3x3 R; R.Rotation(phi*ux);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate_e_z(double phi){
/**
  Rotation around center of mass along the y
  axis in body-fixed coordinate system (body frame)

  \param[in] phi The rotation angle, in radians
*/

  VECTOR ux,uy,uz;
  rb_A_I_to_e.get_vectors(ux,uy,uz);
  MATRIX3x3 R; R.Rotation(phi*ux);
  rb_A_I_to_e = R * rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate(MATRIX3x3& R){
/**
  \brief Arbitrary rotation in body frame, parameterized by a matrix

  Warning: Matrix R should be a valid (unitary, with norm = 1) rotation matrix

  \param[in] R Rotation matrix
*/
  rb_A_I_to_e = R*rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate(QUATERNION& quat){
/**
  \brief Arbitrary rotation in body frame, parameterized by a quaternion

  \param[in] quaternion Rotation quaternion
*/

  MATRIX3x3 R;
  QUATERNION_TO_MATRIX(quat,R);
  rb_A_I_to_e = R*rb_A_I_to_e;
  set_orientation(rb_A_I_to_e);
}

void RigidBody::Rotate(double phi,VECTOR& dir){
/**
  \brief Arbitrary rotation in body frame, parameterized by a rotation axis and angle

  The rotation axis and angle define a quaternion, which is then applied to produce rotation

  \param[in] phi the rotation angle
  \param[in] dir the rotation axis, direction
*/

  double cs = cos(0.5*phi);
  double si = sin(0.5*phi);
  VECTOR u = dir.unit();
  QUATERNION quat(cs,si*u.x,si*u.y,si*u.z);
  Rotate(quat);
}


}// namespace librigidbody
}// liblibra
