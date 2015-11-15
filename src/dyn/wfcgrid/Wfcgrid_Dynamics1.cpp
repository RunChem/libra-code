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

#include "Wfcgrid.h"

namespace libdyn{
namespace libwfcgrid{

using namespace libmmath;
using namespace libmmath::libmeigen;


void Wfcgrid::update_potential_1D(Hamiltonian& ham){
// This function recomputes the Hamiltonian for all points
// working in atomic units: hbar = 1

  vector<double> q(1, 0.0);

  // Precompute H, d_ij, ... along grid
  for(int nx=0;nx<Nx;nx++){
    // For all r points of the grid compute local(potential energy) part
    q[0] = real(X->M[nx]);

    ham.set_q(q);
    ham.compute();

    for(int nst=0;nst<nstates;nst++){        
      for(int nst1=0;nst1<nstates;nst1++){               

        H[nst][nst1].M[nx] = ham.Hvib(nst, nst1);  
                                                      
      }// for nst1
    }// for nst

  }// for nx

}// update_potential_1D

void Wfcgrid::update_potential_2D(Hamiltonian& ham){
// This function recomputes the Hamiltonian for all points
// working in atomic units: hbar = 1

  vector<double> q(2,0.0);

  // Precompute H, d_ij, ... along grid
  for(int nx=0;nx<Nx;nx++){
    for(int ny=0;ny<Ny;ny++){

      // For all r points of the grid compute local(potential energy) part
      q[0] = real(X->M[nx]);
      q[1] = real(Y->M[ny]);

      ham.set_q(q);
      ham.compute();

      for(int nst=0;nst<nstates;nst++){        
        for(int nst1=0;nst1<nstates;nst1++){               

          H[nst][nst1].M[nx*Ny+ny] = ham.Hvib(nst, nst1);  
                                                        
        }// for nst1
      }// for nst

    }// for ny
  }// for nx

}// update_potential_2D



void Wfcgrid::update_propagator_1D(double dt,double m0){
// This function updates real- and recoprocal-space propagators (diagonal matrices)
// working in atomic units: hbar = 1

  int nst, nst1;

  MATRIX* diaH; diaH = new MATRIX(nstates,nstates);
  MATRIX* adiH; adiH = new MATRIX(nstates,nstates);
  MATRIX* S; S = new MATRIX(nstates, nstates);  S->Init_Unit_Matrix(1.0);
  MATRIX* C; C = new MATRIX(nstates, nstates);  *C = 0.0;
  MATRIX* si; si = new MATRIX(nstates, nstates);  *si = 0.0; // cos(-dt*E), where E is adiabatic Ham.
  MATRIX* cs; cs = new MATRIX(nstates, nstates);  *cs = 0.0; // sin(-dt*E), where E is adiabatic Ham.



  // For each 1D grid point
  for(int nx=0;nx<Nx;nx++){

    // Get diabatic Hamiltonian (in real form)
    for(nst=0;nst<nstates;nst++){  
      for(nst1=0;nst1<nstates;nst1++){  
        diaH->M[nst*nstates+nst1] = H[nst][nst1].M[nx].real();
      }
    }

    // Transformation to adiabatic basis
    solve_eigen(nstates, diaH, S, adiH, C);  // diaH * C = S * C * adiH



    // Now compute sin and cos matrixes: diagonal
    for(int nst=0;nst<nstates;nst++){  
      cs->M[nst*nstates+nst] = std::cos(dt*adiH->M[nst*nstates+nst]);
      si->M[nst*nstates+nst] = std::sin(dt*adiH->M[nst*nstates+nst]);
    }
 
    // Transform cs and si according to matrix C:
    *cs = (*C) * (*cs) * ((*C).T());
    *si = (*C) * (*si) * ((*C).T());

//----------- Explicit computation of exponent of a complex matrix -i*diaH*dt -----------
    CMATRIX* X;    X = new CMATRIX(nstates,nstates);
    CMATRIX* pwX;  pwX = new CMATRIX(nstates,nstates);
    CMATRIX* expX; expX = new CMATRIX(nstates,nstates);

    for(nst=0;nst<nstates;nst++){  
      for(nst1=0;nst1<nstates;nst1++){  
        X->M[nst*nstates+nst1] = complex<double>(0.0, -dt*diaH->M[nst*nstates+nst1]);

        if(nst!=nst1){
          expX->M[nst*nstates+nst1] = complex<double>(0.0, 0.0);
        }
        else{
          expX->M[nst*nstates+nst1] = complex<double>(1.0, 0.0);
        }
      }
    }

    *pwX = *expX; // identity

    for(int term=1;term<40;term++){
        *pwX *= (1.0/float(term))*(*X);
        *expX += *pwX;
    }
  


    // Finally construct complex exp(-i*dt*H) matrix from real cs and si matrices
    for(nst=0;nst<nstates;nst++){  
      for(nst1=0;nst1<nstates;nst1++){  

//        expH[nst][nst1].M[nx] = complex<double>(cs->M[nst*nstates+nst1], -si->M[nst*nstates+nst1]);  // exp(-i*H*dt)

          expH[nst][nst1].M[nx] = expX->M[nst*nstates+nst1];

      }
    }//for nst

    delete expX;
    delete pwX;
    delete X;

//    expH[0][0].M[nx] = complex<double>( 1.0,   -7.07107e-05);
//    expH[1][0].M[nx] = expH[0][1].M[nx] = complex<double>(-4.39102e-17,   -0.000170711);
//    expH[1][1].M[nx] = complex<double>( 1.0,   7.07107e-05);    
//    cout<<"nx= \n"<<nx<<" *diaH= "<<*diaH<<" *adiH= "<<*adiH<<" *C= "<<*C
//        <<" expH[0][0]= "<<expH[0][0].M[nx]
//        <<" expH[0][1]= "<<expH[0][1].M[nx]
//        <<" expH[1][1]= "<<expH[1][1].M[nx]<<endl;

  }// for nx


  delete S;
  delete C;
  delete diaH;
  delete adiH;
  delete cs;
  delete si;


}// update_propagator_1D


void Wfcgrid::update_propagator_2D(double dt,double m0){
// This function updates real- and recoprocal-space propagators (diagonal matrices)
// working in atomic units: hbar = 1

  int nst, nst1;

  MATRIX* diaH; diaH = new MATRIX(nstates,nstates);
  MATRIX* adiH; adiH = new MATRIX(nstates,nstates);
  MATRIX* S; S = new MATRIX(nstates, nstates);  S->Init_Unit_Matrix(1.0);
  MATRIX* C; C = new MATRIX(nstates, nstates);  *C = 0.0;
  MATRIX* si; si = new MATRIX(nstates, nstates);  *si = 0.0; // cos(-dt*E), where E is adiabatic Ham.
  MATRIX* cs; cs = new MATRIX(nstates, nstates);  *cs = 0.0; // sin(-dt*E), where E is adiabatic Ham.



  // For each 2D grid point
  for(int nx=0;nx<Nx;nx++){
    for(int ny=0;ny<Ny;ny++){


      // Get diabatic Hamiltonian (in real form)
      for(nst=0;nst<nstates;nst++){  
        for(nst1=0;nst1<nstates;nst1++){  
          diaH->M[nst*nstates+nst1] = H[nst][nst1].M[nx*Ny+ny].real();
        }
      }

      // Transformation to adiabatic basis
      solve_eigen(nstates, diaH, S, adiH, C);  // diaH * C = S * C * adiH


      // Now compute sin and cos matrixes: diagonal
      for(int nst=0;nst<nstates;nst++){  
        cs->M[nst*nstates+nst] = std::cos(-dt*adiH->M[nst*nstates+nst]);
        si->M[nst*nstates+nst] = std::sin(-dt*adiH->M[nst*nstates+nst]);
      }
 
      // Transform cs and si according to matrix C:
      *cs = (*C) * (*cs) * ((*C).T());
      *si = (*C) * (*si) * ((*C).T());


      // Finally construct complex exp(-i*dt*H) matrix from real cs and si matrices
      for(nst=0;nst<nstates;nst++){  
        for(nst1=0;nst1<nstates;nst1++){  

          expH[nst][nst1].M[nx*Ny+ny] = complex<double>(cs->M[nst*nstates+nst1], si->M[nst*nstates+nst1]);  // exp(-i*H*dt)

        }
      }//for nst


    }// for ny
  }// for nx


  delete S;
  delete C;
  delete diaH;
  delete adiH;
  delete cs;
  delete si;


}// update_propagator_2D


void Wfcgrid::update_propagator_K_1D(double dt,double m0){
// This function updates real- and recoprocal-space propagators (diagonal matrices)
// working in atomic units: hbar = 1

  // Precompute H, d_ij, ... along grid
  for(int nx=0;nx<Nx;nx++){

    double kx_ = Kx->M[nx].real();
    double argg = -(2.0*M_PI*M_PI/m0)*(kx_*kx_)*dt;

    complex<double> scl(std::cos(argg),std::sin(argg));

    for(int nst=0;nst<nstates;nst++){    expK[nst].M[nx] = scl;    }//for nst

  }// for nx


}// update_propagator_K_1D


void Wfcgrid::update_propagator_K_2D(double dt,double m0){
// This function updates real- and recoprocal-space propagators (diagonal matrices)
// working in atomic units: hbar = 1

  // Precompute H, d_ij, ... along grid
  for(int nx=0;nx<Nx;nx++){
    for(int ny=0;ny<Ny;ny++){

      double kx_ = Kx->M[nx].real();
      double ky_ = Ky->M[ny].real();
      double argg = -(2.0*M_PI*M_PI/m0)*(kx_*kx_ + ky_*ky_)*dt;

      complex<double> scl(std::cos(argg),std::sin(argg));

      for(int nst=0;nst<nstates;nst++){    expK[nst].M[nx*Ny+ny] = scl;    }//for nst

    }// for ny
  }// for nx


}// update_propagator_K_2D





void Wfcgrid::propagate_exact_1D(int Nmts){

  int nst,nst1,kx,nx;

  // Auxiliary object
  CMATRIX psi(Nx,1);   psi  = 0.0; // is a matrix placeholder
  CMATRIX psi1(Nx,1);  psi1 = 0.0; // is a matrix placeholder
  CMATRIX psi2(Nx,1);  psi2 = 0.0; // is a matrix placeholder
  vector<CMATRIX> newPSI; newPSI = PSI;


  //===================== Wavefunction propagation part ==============================
  //--------------------- exp(-0.5*dt*i/hbar*H_loc) ---------------------
  // For each point on the 1D grid
  for(nx=0;nx<Nx;nx++){

    // For each new state
    for(nst=0;nst<nstates;nst++){

      complex<double> res(0.0, 0.0);     
      // Is a sum over all states for this point
      for(nst1=0;nst1<nstates;nst1++){      
        res += expH[nst][nst1].M[nx] * PSI[nst1].M[nx];
      }
    
      newPSI[nst].M[nx] = res;
                           
    }// for nst
  }// for nx
  PSI = newPSI;
    

  //--------------------- exp(-dt*i/hbar*H_non-loc) ----------------------
  // PSI(r)->PSI(k)=reciPSI
  ft_1D(PSI,reciPSI,1,xmin,kxmin,dx);


  // Propagate in reciprocal space
  for(nst=0;nst<nstates;nst++){   psi.dot(reciPSI[nst],expK[nst]);  reciPSI[nst] = psi;   }// for nst


  // PSI(k)=reciPSI -> PSI(r)
  ft_1D(reciPSI,PSI,2,xmin,kxmin,dx);


  //--------------------- exp(-0.5*dt*i/hbar*H_loc) ---------------------
  // For each point on the 1D grid
  for(nx=0;nx<Nx;nx++){

    // For each new state
    for(nst=0;nst<nstates;nst++){

      complex<double> res(0.0, 0.0);     
      // Is a sum over all states for this point
      for(nst1=0;nst1<nstates;nst1++){      
        res += expH[nst][nst1].M[nx] * PSI[nst1].M[nx];
      }
    
      newPSI[nst].M[nx] = res;
                           
    }// for nst
  }// for nx
  PSI = newPSI;

}// void Wfcgrid::propagate_exact_1D(int Nmts)



void Wfcgrid::propagate_exact_2D(int Nmts){

  int nst,nst1,kx,ky, nx, ny;

  // Auxiliary object
  CMATRIX psi(Nx,Ny);   psi  = 0.0; // is a matrix placeholder
  CMATRIX psi1(Nx,Ny);  psi1 = 0.0; // is a matrix placeholder
  CMATRIX psi2(Nx,Ny);  psi2 = 0.0; // is a matrix placeholder
  vector<CMATRIX> newPSI = PSI;


  //===================== Wavefunction propagation part ==============================
  //--------------------- exp(-0.5*dt*i/hbar*H_loc) ---------------------
  // For each point on the 2D grid
  for(nx=0;nx<Nx;nx++){
    for(ny=0;ny<Ny;ny++){

      // For each new state
      for(nst=0;nst<nstates;nst++){

        complex<double> res(0.0, 0.0);     
        // Is a sum over all states for this point
        for(nst1=0;nst1<nstates;nst1++){      
          res += expH[nst][nst1].M[nx*Ny+ny] * PSI[nst1].M[nx*Ny+ny];
        }
      
        newPSI[nst].M[nx*Ny+ny] = res;
                             
      }// for nst
    }// for ny
  }// for nx
  PSI = newPSI;
    
   
  //--------------------- exp(-dt*i/hbar*H_non-loc) ----------------------
  // PSI(r)->PSI(k)=reciPSI
  ft_2D(PSI,reciPSI,1,xmin,ymin,kxmin,kymin,dx,dy);


  // Propagate in reciprocal space
  for(nst=0;nst<nstates;nst++){   psi.dot(reciPSI[nst],expK[nst]);  reciPSI[nst] = psi;   }// for nst


  // PSI(k)=reciPSI -> PSI(r)
  ft_2D(reciPSI,PSI,2,xmin,ymin,kxmin,kymin,dx,dy);


  //--------------------- exp(-0.5*dt*i/hbar*H_loc) ---------------------
  // For each point on the 2D grid
  for(nx=0;nx<Nx;nx++){
    for(ny=0;ny<Ny;ny++){

      // For each new state
      for(nst=0;nst<nstates;nst++){

        complex<double> res(0.0, 0.0);     
        // Is a sum over all states for this point
        for(nst1=0;nst1<nstates;nst1++){      
          res += expH[nst][nst1].M[nx*Ny+ny] * PSI[nst1].M[nx*Ny+ny];
        }
      
        newPSI[nst].M[nx*Ny+ny] = res;
                             
      }// for nst
    }// for ny
  }// for nx
  PSI = newPSI;

}// void Wfcgrid::propagate_exact_2D(int Nmts)



void Wfcgrid::absorb_1D(double dL,vector<double>& Pops_l,vector<double>& Pops_r){
// dL - is a length of the absorbing layer
  int nL = dL/dx;  // how many points from each boundary to set to zero

  if(Pops_l.size()<nstates){ Pops_l = vector<double>(nstates,0.0);  } // Population in the left trapping region
  if(Pops_r.size()<nstates){ Pops_r = vector<double>(nstates,0.0);  }

  for(int nst=0;nst<nstates;nst++){

    // On the left
    for(int i=0;i<=nL;i++){
      Pops_l[nst] += dx*real(std::conj(PSI[nst].M[i])*PSI[nst].M[i]);
      PSI[nst].M[i] = 0.0;
    }

    // On the right
    for(i=0;i<=nL;i++){
      Pops_r[nst] += dx*real(std::conj(PSI[nst].M[(Nx-1-i)])*PSI[nst].M[(Nx-1-i)]);
      PSI[nst].M[Nx-1-i] = 0.0;
    }

  }// for st


  // Update reciprocal part
  // PSI(r)->PSI(k)=reciPSI
  ft_1D(PSI,reciPSI,1,xmin,kxmin,dx);

}


boost::python::list Wfcgrid::absorb_1D(double dL){

  vector<double> Pops_l(nstates,0.0);
  vector<double> Pops_r(nstates,0.0);

  boost::python::list p_l, p_r, res;

  absorb_1D(dL, Pops_l, Pops_r);

  for(int i=0;i<nstates;i++){
    p_l.append(Pops_l[i]);
    p_r.append(Pops_r[i]);
  }
  res.append(p_l);
  res.append(p_r);

  return res;

}// absorb_1D




}// namespace libwfcgrid
}// namespace libdyn


