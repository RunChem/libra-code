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
  \file Electronic_Dynamics1.cpp
  \brief The file implements the methods for solving TD-SE (electronic dynamics)
    
*/

#include "Electronic.h"
#include <cmath>
#include "../../math_meigen/libmeigen.h"
#include "../../math_operators/liboperators.h"

/// liblibra namespace
namespace liblibra{


using namespace libmeigen;

/// libdyn namespace 
namespace libdyn{

/// libelectronic namespace 
namespace libelectronic{


using liboperators::rotate;


void Electronic::propagate_electronic(double dt,Hamiltonian* ham){
/**
  \brief Propagate electronic DOF using sequential rotations in the MMTS variables

  Methodologically: This version is based on the Hamiltonian formulation of TD-SE
  This propagator is good for general Hamiltonian - diabatic of adiabatic
  iL = iL_qp + iL_qq + iL_pp
  iL_qp = sum_i,j {}

  API: The member function of the Electronic function, with the pointer to Hamiltonian object

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in] ham The pointer to Hamiltonian object, that affects the dynamics

*/ 

  libdyn::libelectronic::propagate_electronic(dt,this,ham);
}

void Electronic::propagate_electronic(double dt,Hamiltonian& ham){
/**
  \brief Propagate electronic DOF using sequential rotations in the MMTS variables

  Methodologically: This version is based on the Hamiltonian formulation of TD-SE
  This propagator is good for general Hamiltonian - diabatic of adiabatic
  iL = iL_qp + iL_qq + iL_pp
  iL_qp = sum_i,j {}

  API: The member function of the Electronic function, with the reference to Hamiltonian object

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in] ham The reference to Hamiltonian object, that affects the dynamics

*/ 

  libdyn::libelectronic::propagate_electronic(dt,this,&ham);
}


void propagate_electronic(double dt,Electronic* el,Hamiltonian* ham){
/**
  \brief Propagate electronic DOF using sequential rotations in the MMTS variables

  Methodologically: This version is based on the Hamiltonian formulation of TD-SE
  This propagator is good for general Hamiltonian - diabatic of adiabatic
  iL = iL_qp + iL_qq + iL_pp
  iL_qp = sum_i,j {}

  API: A free function that takes Electronic object as the input and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] el The pointer to the Electronic object containing the electronic DOF
  \param[in] ham The pointer to Hamiltonian object, that affects the dynamics

*/ 


  int i,j;

  double dt_half = 0.5*dt;

  //------------- Phase evolution (adiabatic) ---------------- 
  // exp(iL_qp * dt/2)
  for(i=0;i<el->nstates;i++){
    for(j=0;j<el->nstates;j++){

      rotate(el->p[j],el->q[i], dt_half*ham->Hvib(i,j).real());

    }// for j
  }// for i

  //------------- Population transfer (adiabatic) ----------------
  // exp((iL_qq + iL_pp) * dt/2)
  for(i=0;i<el->nstates;i++){
    for(j=i+1;j<el->nstates;j++){

      rotate(el->q[j],el->q[i], dt_half*ham->Hvib(i,j).imag());
      rotate(el->p[j],el->p[i], dt_half*ham->Hvib(i,j).imag());

    }// for j
  }// for i

  // exp((iL_qq + iL_pp) * dt/2)
  for(i=el->nstates-1;i>=0;i--){
    for(j=el->nstates-1;j>i;j--){


      rotate(el->q[j],el->q[i], dt_half*ham->Hvib(i,j).imag());
      rotate(el->p[j],el->p[i], dt_half*ham->Hvib(i,j).imag());

    }// for j
  }// for i


  //------------- Phase evolution (adiabatic) ----------------
  // exp(iL_qp * dt/2)
  for(i=el->nstates-1;i>=0;i--){
    for(j=el->nstates-1;j>=0;j--){

      rotate(el->p[j],el->q[i], dt_half*ham->Hvib(i,j).real());

    }// for j
  }// for i


}// propagate_electronic


void propagate_electronic(double dt,Electronic& el, CMATRIX& Hvib){
/**
  \brief Propagate electronic DOF using sequential rotations in the MMTS variables

  Methodologically: This version is based on the Hamiltonian formulation of TD-SE
  This propagator is good for general Hamiltonian - diabatic of adiabatic
  iL = iL_qp + iL_qq + iL_pp
  iL_qp = sum_i,j {}

  API: A free function that takes Electronic object as the input and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] el The reference to the Electronic object containing the electronic DOF
  \param[in] ham The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) 

  This is the Python-friendly function
*/ 


  int i,j;

  double dt_half = 0.5*dt;

  //------------- Phase evolution (adiabatic) ---------------- 
  // exp(iL_qp * dt/2)
  for(i=0;i<el.nstates;i++){
    for(j=0;j<el.nstates;j++){

      rotate(el.p[j],el.q[i], dt_half*Hvib.get(i,j).real());

    }// for j
  }// for i

  //------------- Population transfer (adiabatic) ----------------
  // exp((iL_qq + iL_pp) * dt/2)
  for(i=0;i<el.nstates;i++){
    for(j=i+1;j<el.nstates;j++){

      rotate(el.q[j],el.q[i], dt_half*Hvib.get(i,j).imag());
      rotate(el.p[j],el.p[i], dt_half*Hvib.get(i,j).imag());

    }// for j
  }// for i

  // exp((iL_qq + iL_pp) * dt/2)
  for(i=el.nstates-1;i>=0;i--){
    for(j=el.nstates-1;j>i;j--){


      rotate(el.q[j],el.q[i], dt_half*Hvib.get(i,j).imag());
      rotate(el.p[j],el.p[i], dt_half*Hvib.get(i,j).imag());

    }// for j
  }// for i


  //------------- Phase evolution (adiabatic) ----------------
  // exp(iL_qp * dt/2)
  for(i=el.nstates-1;i>=0;i--){
    for(j=el.nstates-1;j>=0;j--){

      rotate(el.p[j],el.q[i], dt_half*Hvib.get(i,j).real());

    }// for j
  }// for i


}// propagate_electronic


void propagate_electronic(double dt, CMATRIX& Coeff, CMATRIX& Hvib){
/**
  Solves the time-dependent Schrodinger equation:

  i*hbar*dc/dt = Hvib*c 
   
  API: A free function that takes electronic DOF in the form of matrix-colomun and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] Coeff The reference to the CMATRIX object containing the electronic DOF (coefficient)
  \param[in] ham The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) - the complex-valued matrix, CMATRIX
             it is also assumed to Hermitian - pay attention to how it is constructed!
  \param[in] S The reference to the overlap matrix (assumed to be a complex-valued time-dependent matrix, CMATRIX)

  This integrator IS unitary
  This is the Python-friendly function
*/ 


  int i,j;
 
  // Let us first diagonalize the overlap matrix S
  int sz = Hvib.n_cols;  

  // Transform the Hamiltonian accordingly:
  CMATRIX* I; I = new CMATRIX(sz, sz);  I->load_identity();
  CMATRIX* C; C = new CMATRIX(sz, sz);  *C = complex<double>(0.0, 0.0); // eigenvectors
  CMATRIX* Heig; Heig = new CMATRIX(sz, sz);  *Heig = complex<double>(0.0,0.0); // eigenvalues
  CMATRIX* expH;   expH = new CMATRIX(sz, sz);    *expH = complex<double>(0.0,0.0);   


  // Compute the exponential  exp(-i*Hvib*dt)  
  libmeigen::solve_eigen(Hvib, *I, *Heig, *C, 0);  // Hvib_eff * C = I * C * Heig  ==>  Hvib = C * Heig * C.H()


  // Diagonal form expH
  complex<double> one(0.0, 1.0);
  for(i=0;i<sz;i++){
    complex<double> val = std::exp(-one*Heig->get(i,i)*dt );
    expH->set(i,i,val);
  }

  // Transform back to the original basis:
  *expH = (*C) * (*expH) * ((*C).H());

  // Propagation
  Coeff = *expH * Coeff;

  
  // Clean temporary memory
  delete expH;  delete Heig;  delete C;  delete I;



}// propagate_electronic





void propagate_electronic(double dt,Electronic& el, CMATRIX& Hvib, MATRIX& S){
/**
  \brief Propagate electronic DOF in the MMTS variables

  Methodologically: solve i*hbar*S*dc/dt = Hvib*c directly, using Lowdin-type transformation and 
  matrix exponentiation. This solver is good only for time-independent S matrix

  API: A free function that takes Electronic object as the input and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] el The reference to the Electronic object containing the electronic DOF
  \param[in] ham The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) - the complex-valued matrix, CMATRIX
  \param[in] S The reference to the overlap matrix (assumed to be a real-valued matrix, MATRIX)

  This integrator is fully unitary (the norm is conserved exactly)
  This is the Python-friendly function
*/ 

  int i,j;

  // Let us first diagonalize the overlap matrix S
  int sz = S.n_cols;
  
  MATRIX* I; I = new MATRIX(sz, sz);  I->Init_Unit_Matrix(1.0);
  CMATRIX* C; C = new CMATRIX(sz, sz);  *C = complex<double>(0.0, 0.0);
  CMATRIX* Seig; Seig = new CMATRIX(sz, sz);  *Seig = complex<double>(0.0,0.0);

  // Transformation to adiabatic basis
  libmeigen::solve_eigen(S, *I, *Seig, *C, 0);  // S * C = I * C * Seig  ==>  S = C * Seig * C.H()


  CMATRIX* S_i_half; S_i_half = new CMATRIX(sz, sz);  *S_i_half = complex<double>(0.0,0.0);  // S^{-1/2}
  CMATRIX* S_half;   S_half = new CMATRIX(sz, sz);    *S_half = complex<double>(0.0,0.0);    // S^{1/2}


  // Diagonal form of S^{-1/2} and S^{1/2} matrices
  for(i=0;i<sz;i++){
    complex<double> val = std::sqrt(Seig->get(i,i));

    S_i_half->M[i*sz+i] = 1.0/val;
    S_half->M[i*sz+i] = val;
  }

  // Convert to original basis
  *S_i_half = (*C) * (*S_i_half) * ((*C).H());
  *S_half = (*C) * (*S_half) * ((*C).H());




  // Transform the Hamiltonian accordingly:
  CMATRIX* Hvib_eff;  Hvib_eff = new CMATRIX(sz,sz);
  CMATRIX* coeff;     coeff = new CMATRIX(sz,1);
  for(i=0;i<sz;i++){  coeff->M[i] = complex<double>(el.q[i], el.p[i]);  }


  // Transform Hvib and coefficients
  *Hvib_eff = (*S_i_half) * (Hvib) * (*S_i_half);
  *coeff = (*S_half) * (*coeff); // * (*S_half);      // now these are effective coefficients
  
  // Set up the object
  Electronic el_eff; el_eff = el;
  for(i=0; i<sz; i++){  el_eff.q[i] = coeff->get(i,0).real(); el_eff.p[i] = coeff->get(i,0).imag(); }


  // Now do the standard propagation
  propagate_electronic(dt, el_eff, *Hvib_eff);


  for(i=0;i<sz;i++){  coeff->M[i] = complex<double>(el_eff.q[i], el_eff.p[i]);  }


  // Transform the coefficients back to the original representation:
  *coeff = (*S_i_half) * (*coeff); // * (*S_i_half);      // now these are effective coefficients
  

  // Update "normal" electronic variables
  for(i=0; i<sz; i++){  el.q[i] = coeff->get(i,0).real(); el.p[i] = coeff->get(i,0).imag(); }  


  // Clean temporary memory
  delete coeff;
  delete Hvib_eff;
  delete S_i_half;
  delete S_half;
  delete Seig;
  delete C;
  delete I;



}// propagate_electronic


void Electronic::propagate_electronic(double dt,Hamiltonian& ham, CMATRIX& S){
/**
  \brief Propagate electronic DOF 

  Same as 
  void propagate_electronic(double dt,Electronic& el, CMATRIX& Hvib, CMATRIX& S)

*/ 
  CMATRIX* Hvib; Hvib = new CMATRIX(nstates, nstates);

  for(int i=0;i<nstates;i++){
    for(int j=0;j<nstates;j++){
      Hvib->set(i,j, ham.Hvib(i,j) );
    }
  }
  
  libdyn::libelectronic::propagate_electronic(dt, *this, *Hvib, S);

  delete Hvib;

}


void propagate_electronic(double dt,Electronic& el, CMATRIX& Hvib, CMATRIX& S){
/**
  \brief Propagate electronic DOF in the MMTS variables

  Methodologically: solve i*hbar*S*dc/dt = Hvib*c directly, using Lowdin-type transformation and 
  matrix exponentiation. This solver is good only for time-independent S matrix

  API: A free function that takes Electronic object as the input and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] el The reference to the Electronic object containing the electronic DOF
  \param[in] ham The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) - the complex-valued matrix, CMATRIX
  \param[in] S The reference to the overlap matrix (assumed to be a complex-valued matrix, CMATRIX)

  This integrator is fully unitary (the norm is conserved exactly)
  This is the Python-friendly function
*/ 

  int i,j;
 
  // Let us first diagonalize the overlap matrix S
  int sz = S.n_cols;  
  CMATRIX* I; I = new CMATRIX(sz, sz);  I->load_identity(); //->Init_Unit_Matrix(complex<double>(1.0,0.0));
  CMATRIX* C; C = new CMATRIX(sz, sz);  *C = complex<double>(0.0, 0.0);
  CMATRIX* Seig; Seig = new CMATRIX(sz, sz);  *Seig = complex<double>(0.0,0.0);


  // Transformation to adiabatic basis
  libmeigen::solve_eigen(S, *I, *Seig, *C, 0);  // S * C = I * C * Seig  ==>  S = C * Seig * C.H()


  // Diagonal form of S^{-1/2} and S^{1/2} matrices
  CMATRIX* S_i_half; S_i_half = new CMATRIX(sz, sz);  *S_i_half = complex<double>(0.0,0.0);  // S^{-1/2}
  CMATRIX* S_half;   S_half = new CMATRIX(sz, sz);    *S_half = complex<double>(0.0,0.0);    // S^{1/2}
  for(i=0;i<sz;i++){
    complex<double> val = std::sqrt(Seig->get(i,i));

    S_i_half->M[i*sz+i] = 1.0/val;
    S_half->M[i*sz+i] = val;
  }


  // Convert to original basis
  *S_i_half = (*C) * (*S_i_half) * ((*C).H());
  *S_half = (*C) * (*S_half) * ((*C).H());


  // Transform the Hamiltonian accordingly:
  CMATRIX* Hvib_eff;  Hvib_eff = new CMATRIX(sz,sz);
  CMATRIX* coeff;     coeff = new CMATRIX(sz,1);
  for(i=0;i<sz;i++){  coeff->M[i] = complex<double>(el.q[i], el.p[i]);  }


  // Transform Hvib and coefficients
  *Hvib_eff = (*S_i_half) * (Hvib) * (*S_i_half);
  *coeff = (*S_half) * (*coeff);                 // now these are effective coefficients

  
  // Set up the object
  Electronic el_eff; el_eff = el;
  for(i=0; i<sz; i++){  el_eff.q[i] = coeff->get(i,0).real(); el_eff.p[i] = coeff->get(i,0).imag(); }


  // Compute the exponential  exp(-i*Hvib*dt)  
  libmeigen::solve_eigen(*Hvib_eff, *I, *Seig, *C, 0);  // Hvib_eff * C = I * C * Seig  ==>  Hvib = C * Seig * C.H()


  // Diagonal form expH
  CMATRIX* expH;   expH = new CMATRIX(sz, sz);    *expH = complex<double>(0.0,0.0);    // 
  complex<double> one(0.0, 1.0);
  for(i=0;i<sz;i++){
    complex<double> val = std::exp(-one*Seig->get(i,i)*dt );
    expH->set(i,i,val);
  }


  // Transform back to the original basis:
  *expH = (*C) * (*expH) * ((*C).H());


  // Propagation
   *coeff = *expH * *coeff;


  // Transform the coefficients back to the original representation:
  *coeff = (*S_i_half) * (*coeff); // * (*S_i_half);      // now these are effective coefficients

  
  // Update "normal" electronic variables
  for(i=0; i<sz; i++){  el.q[i] = coeff->get(i,0).real(); el.p[i] = coeff->get(i,0).imag(); }  


  // Clean temporary memory
  delete expH;
  delete coeff;
  delete Hvib_eff;
  delete S_i_half;
  delete S_half;
  delete Seig;
  delete C;
  delete I;



}// propagate_electronic




void propagate_electronic(double dt, CMATRIX& Coeff, CMATRIX& Hvib, CMATRIX& S){
/**
  Solves the generalized time-dependent Schrodinger equation:

  i*hbar*S*dc/dt = Hvib*c 

  Although the vibronic Hamiltonian, Hvib_tilda, computed in the non-orthonormal basis is not Hermitian,
  the theory eventually leads to a Hermitian version of the vibronic Hamiltonian, so we take the Hermitian one
  as the input. See my paper on the details.
 
  API: A free function that takes electronic DOF in the form of matrix-colomun and modifies it

  \param[in] dt The integration time step (also the duration of propagation)
  \param[in,out] Coeff The reference to the CMATRIX object containing the electronic DOF (coefficient)
  \param[in] ham The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) - the complex-valued matrix, CMATRIX
             it is also assumed to Hermitian - pay attention to how it is constructed!
  \param[in] S The reference to the overlap matrix (assumed to be a complex-valued time-dependent matrix, CMATRIX)

  This integrator IS unitary
  This is the Python-friendly function
*/ 


  int i,j;
 
  // Let us first diagonalize the overlap matrix S
  int sz = S.n_cols;  
  CMATRIX* I; I = new CMATRIX(sz, sz);  I->load_identity(); //->Init_Unit_Matrix(complex<double>(1.0,0.0));
  CMATRIX* C; C = new CMATRIX(sz, sz);  *C = complex<double>(0.0, 0.0);
  CMATRIX* Seig; Seig = new CMATRIX(sz, sz);  *Seig = complex<double>(0.0,0.0);


  // Transformation to adiabatic basis
  libmeigen::solve_eigen(S, *I, *Seig, *C, 0);  // S * C = I * C * Seig  ==>  S = C * Seig * C.H()


  // Diagonal form of S^{-1/2} and S^{1/2} matrices
  CMATRIX* S_i_half; S_i_half = new CMATRIX(sz, sz);  *S_i_half = complex<double>(0.0,0.0);  // S^{-1/2}
  CMATRIX* S_half;   S_half = new CMATRIX(sz, sz);    *S_half = complex<double>(0.0,0.0);    // S^{1/2}
  for(i=0;i<sz;i++){
    complex<double> val = std::sqrt(Seig->get(i,i));

    S_i_half->M[i*sz+i] = 1.0/val;
    S_half->M[i*sz+i] = val;
  }


  // Convert to original basis
  *S_i_half = (*C) * (*S_i_half) * ((*C).H());
  *S_half = (*C) * (*S_half) * ((*C).H());


  // Transform the Hamiltonian accordingly:
  CMATRIX* Hvib_eff;  Hvib_eff = new CMATRIX(sz,sz);
  CMATRIX* coeff;     coeff = new CMATRIX(sz,1);

  *Hvib_eff = (*S_i_half) * Hvib * (*S_i_half);  // Hermitian part
  *coeff = (*S_half) * Coeff;                    // now these are the effective coefficients

  
  // Compute the exponential  exp(-i*Hvib*dt)  
  libmeigen::solve_eigen(*Hvib_eff, *I, *Seig, *C, 0);  // Hvib_eff * C = I * C * Seig  ==>  Hvib = C * Seig * C.H()


  // Diagonal form expH
  CMATRIX* expH;   expH = new CMATRIX(sz, sz);    *expH = complex<double>(0.0,0.0);    // 
  complex<double> one(0.0, 1.0);
  for(i=0;i<sz;i++){
    complex<double> val = std::exp(-one*Seig->get(i,i)*dt );
    expH->set(i,i,val);
  }

  // Transform back to the original basis:
  *expH = (*C) * (*expH) * ((*C).H());

  // Propagation
   *coeff = *expH * *coeff;

  // Transform the coefficients back to the original representation:
  *coeff = (*S_i_half) * (*coeff); // * (*S_i_half);      // convert the effective coefficients back to original representation

  for(i=0;i<Coeff.n_elts;i++){  Coeff.M[i] = coeff->M[i]; } // so we don't allocate new memory for Coeff!!!

  
  // Clean temporary memory
  delete expH;
  delete coeff;
  delete Hvib_eff;
  delete S_i_half;
  delete S_half;
  delete Seig;
  delete C;
  delete I;



}// propagate_electronic




void grid_propagator(double dt, CMATRIX& Hvib, CMATRIX& S, CMATRIX& U){
/**
  A super-fast version specifically taylored to grid integration (since grid is known,
  the matrices S, Hvib and all the dependent matrices are fixed, so can be computed only once and then
  passed here)

  Solves the generalized time-dependent Schrodinger equation:

  i*hbar*S*dc/dt = Hvib*c 
 
  \param[in] dt The integration time step (also the duration of propagation)
  \param[in] Hvib The reference to the vibronic Hamiltonian matrix (not the Hamiltonian object!) - the complex-valued matrix, CMATRIX
             it is also assumed to be Hermitian - pay attention to how it is constructed!
  \param[in] S The reference to the overlap matrix (assumed to be a complex-valued time-dependent matrix, CMATRIX)

  This is the Python-friendly function
  This function returns a propagator
*/ 

  int sz = S.n_cols;  
  CMATRIX S_half(sz,sz);
  CMATRIX S_i_half(sz,sz);

  // Diagonal form of S^{-1/2} and S^{1/2} matrices
  sqrt_matrix(S, S_half, S_i_half);

  // Transform the Hamiltonian accordingly:
  CMATRIX Hvib_eff(sz,sz);  
  Hvib_eff = S_i_half * Hvib * S_i_half;

  // Compute the exponential  exp(-i*Hvib_eff*dt)  
  CMATRIX expH(sz,sz);  
  exp_matrix(expH, Hvib_eff, complex<double>(0.0, -dt) );

  // Propagator
  U = S_i_half * expH * S_half;

  
  // Clean temporary memory
  //delete expH;
  //delete Hvib_eff;
  //delete S_i_half;
  //delete S_half;



}// propagate_electronic



}// namespace libelectronic
}// namespace libdyn
}// liblibra

