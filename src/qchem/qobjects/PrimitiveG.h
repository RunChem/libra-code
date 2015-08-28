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

#ifndef PrimitiveG_H
#define PrimitiveG_H

#include "../../mmath/libmmath.h"
using namespace libmmath;


namespace libqchem{
namespace libqobjects{


//============ Primitive Gaussian functions class ====================

class PrimitiveG{
  // f = (x-X)^{x_exp} * (y-Y)^{y_exp} * (z-Z)^(z_exp) * exp(-alpha * (r - R)^2)
  // R = (X, Y, Z)^T

public:
  // Members data
  int x_exp;          int is_x_exp;
  int y_exp;          int is_y_exp;
  int z_exp;          int is_z_exp;
  double alpha;       int is_alpha;
  VECTOR R;           int is_R; 
  // Function value
  double value;       int is_value;


  // Getters-setters
  int get_x_exp();
  int get_y_exp();
  int get_z_exp();
  double get_alpha();
  VECTOR get_R();
  double get_value();
                                   
  void set_x_exp(int _x);
  void set_y_exp(int _y);
  void set_z_exp(int _z);
  void set_alpha(double _alp);
  void set_R(VECTOR& _R);



  //----------------------------------------------
  // Member functions
  void init(int l,int m,int n,double alp,VECTOR& center);  /// general initialization/setup

  PrimitiveG::PrimitiveG();   /// Default c-tor
  PrimitiveG::PrimitiveG(int l,int m,int n,double alp,VECTOR& center); /// general c-tor
  PrimitiveG(const PrimitiveG&);  /// Copy c-tor
  PrimitiveG& operator=(const PrimitiveG&);  /// assignment operator


  // Computations and print
  double compute(VECTOR&);
  double norm2(); 
  double norm1(); 
  double normalization_factor(); 
  void show_info();

  // Transformations
  void shift_position(const VECTOR&);


  friend int operator == (const PrimitiveG& g1, const PrimitiveG& g2){
    return ((g1.x_exp==g2.x_exp) && (g1.y_exp==g2.y_exp) && (g1.z_exp==g2.z_exp)
          &&(fabs(g1.alpha-g2.alpha)<1e-10) && ((g1.R-g2.R).length2()<1e-10));
  }

     
};

// Molints functions exported for use with Gaussians objects:
// Overlaps
double gaussian_overlap
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdB, vector<double*>& auxd,int n_aux
);

double gaussian_overlap
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdB
);

boost::python::list gaussian_overlap
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs
);
double gaussian_overlap( PrimitiveG& GA, PrimitiveG& GB,int is_normalize);
double gaussian_overlap( PrimitiveG& GA, PrimitiveG& GB);


// Moments
double gaussian_moment
( PrimitiveG& GA, PrimitiveG& G, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdR, VECTOR& dIdB, vector<double*>& auxd,int n_aux
);

double gaussian_moment
( PrimitiveG& GA, PrimitiveG& G, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdR, VECTOR& dIdB
);

boost::python::list gaussian_moment
( PrimitiveG& GA, PrimitiveG& G, PrimitiveG& GB,int is_normalize, int is_derivs
);
double gaussian_moment( PrimitiveG& GA, PrimitiveG& G, PrimitiveG& GB,int is_normalize);
double gaussian_moment( PrimitiveG& GA, PrimitiveG& G, PrimitiveG& GB);


// Pseudopotentials
double pseudopot02(double C0, double C2, double alp, const VECTOR& R,
                   PrimitiveG&  GA, PrimitiveG& GB,
                   int is_normalize, 
                   int is_derivs, VECTOR& dIdR, VECTOR& dIdA, VECTOR& dIdB,
                   vector<double*>& auxd,int n_aux
                  );
double pseudopot02(double C0, double C2, double alp, const VECTOR& R,
                   PrimitiveG&  GA, PrimitiveG& GB,
                   int is_normalize, 
                   int is_derivs, VECTOR& dIdR, VECTOR& dIdA, VECTOR& dIdB
                  );

boost::python::list pseudopot02(double C0, double C2, double alp, const VECTOR& R,
                                PrimitiveG&  GA, PrimitiveG& GB,
                                int is_normalize, int is_derivs
                               );

double pseudopot02(double C0, double C2, double alp, const VECTOR& R,
                   PrimitiveG&  GA, PrimitiveG& GB,
                   int is_normalize
                  );

double pseudopot02(double C0, double C2, double alp, const VECTOR& R,
                   PrimitiveG&  GA, PrimitiveG& GB
                  );


// Multipoles
VECTOR transition_dipole_moment
( PrimitiveG& GA, PrimitiveG& GB,
  int is_normalize,int is_derivs, MATRIX3x3& dMdA, MATRIX3x3& dMdB,
  vector<double*>& auxd,int n_aux
);
VECTOR transition_dipole_moment
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize,int is_derivs, MATRIX3x3& dMdA, MATRIX3x3& dMdB
);
boost::python::list transition_dipole_moment( PrimitiveG& GA, PrimitiveG& GB, int is_normalize,int is_derivs);
VECTOR transition_dipole_moment( PrimitiveG& GA, PrimitiveG& GB, int is_normalize);
VECTOR transition_dipole_moment( PrimitiveG& GA, PrimitiveG& GB);


// Derivative coupling integrals
VECTOR derivative_coupling_integral
( PrimitiveG& GA, PrimitiveG& GB,
  int is_normalize,int is_derivs, MATRIX3x3& dMdA, MATRIX3x3& dMdB,
  vector<double*>& auxd,int n_aux
);
VECTOR derivative_coupling_integral
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize,int is_derivs, MATRIX3x3& dMdA, MATRIX3x3& dMdB
);
boost::python::list derivative_coupling_integral( PrimitiveG& GA, PrimitiveG& GB, int is_normalize,int is_derivs);
VECTOR derivative_coupling_integral( PrimitiveG& GA, PrimitiveG& GB, int is_normalize);
VECTOR derivative_coupling_integral( PrimitiveG& GA, PrimitiveG& GB);




// Kinetic integrals
double kinetic_integral
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdB, vector<double*>& auxd,int n_aux
);

double kinetic_integral
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs,
  VECTOR& dIdA, VECTOR& dIdB
);

boost::python::list kinetic_integral
( PrimitiveG& GA, PrimitiveG& GB,int is_normalize, int is_derivs
);
double kinetic_integral( PrimitiveG& GA, PrimitiveG& GB,int is_normalize);
double kinetic_integral( PrimitiveG& GA, PrimitiveG& GB);


// Nuclear Attraction Integral
double nuclear_attraction_integral
( PrimitiveG& GA, PrimitiveG& GB, VECTOR& Rc,
  int is_normalize, 
  int is_derivs,  VECTOR& DA,VECTOR& DB,VECTOR& DC,
  vector<double*>& aux,int n_aux,vector<VECTOR*>& auxv,int n_auxv
);
double nuclear_attraction_integral
( PrimitiveG& GA, PrimitiveG& GB, VECTOR& Rc,
  int is_normalize, 
  int is_derivs,  VECTOR& DA,VECTOR& DB,VECTOR& DC
);
boost::python::list nuclear_attraction_integral
( PrimitiveG& GA, PrimitiveG& GB, VECTOR& Rc,
  int is_normalize, int is_derivs
);
double nuclear_attraction_integral
( PrimitiveG& GA, PrimitiveG& GB, VECTOR& Rc,
  int is_normalize
);
double nuclear_attraction_integral
( PrimitiveG& GA, PrimitiveG& GB, VECTOR& Rc
);



// Electron Repulsion Integral
double electron_repulsion_integral
( PrimitiveG& GA, PrimitiveG& GB, PrimitiveG& GC, PrimitiveG& GD,
  int is_normalize, 
  int is_derivs,  VECTOR& DA,VECTOR& DB,VECTOR& DC,VECTOR& DD,
  vector<double*>& aux,int n_aux,vector<VECTOR*>& auxv,int n_auxv
);
double electron_repulsion_integral
( PrimitiveG& GA, PrimitiveG& GB, PrimitiveG& GC, PrimitiveG& GD,
  int is_normalize, 
  int is_derivs,  VECTOR& DA,VECTOR& DB,VECTOR& DC,VECTOR& DD
);
boost::python::list electron_repulsion_integral
( PrimitiveG& GA, PrimitiveG& GB, PrimitiveG& GC, PrimitiveG& GD,
  int is_normalize, int is_derivs
);
double electron_repulsion_integral
( PrimitiveG& GA, PrimitiveG& GB, PrimitiveG& GC, PrimitiveG& GD,
  int is_normalize
);
double electron_repulsion_integral
( PrimitiveG& GA, PrimitiveG& GB, PrimitiveG& GC, PrimitiveG& GD
);








typedef std::vector<PrimitiveG> PrimitiveGList;


}// namespace libqobjects
}// namespace libqchem



#endif // PrimitiveG_H
