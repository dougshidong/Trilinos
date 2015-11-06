// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//         
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

/*! \file  test_07.cpp
    \brief Interior Point test using Hock & Schittkowski problem 32.
*/

#include "ROL_HS32.hpp"
#include "ROL_LogBarrierObjective.hpp"
#include "ROL_InteriorPoint.hpp"


template<class Real> 
void print_vector(const ROL::Vector<Real> &x) {
  typedef ROL::StdVector<Real> SV;
  using Teuchos::dyn_cast;
  const SV &xs = dyn_cast<const SV>(x);
  Teuchos::RCP<const std::vector<Real> > x_rcp = xs.getVector();
   
  for(int i=0;i<xs.dimension();++i) {
    std::cout << (*x_rcp)[i] << std::endl; 
  }
}

template<class Real> 
void print_subvector(const ROL::Vector<Real> &x, const int i){
  typedef ROL::PartitionedVector<Real> PV;
  typedef typename PV::size_type size_type;
  size_type n = static_cast<size_type>(i);
  using Teuchos::dyn_cast;
  const PV &xp = dyn_cast<const PV>(x);
  print_vector(*(xp.get(n)));
}

typedef double RealT;

int main(int argc, char *argv[]) {

  using Teuchos::RCP;
  using Teuchos::rcp; 

  typedef std::vector<RealT>            vec;
  typedef ROL::StdVector<RealT>         SV;
  typedef RCP<ROL::Vector<RealT> >      RCPV;

//  typedef ROL::PartitionedVector<RealT> PV;


  Teuchos::GlobalMPISession mpiSession(&argc, &argv);

  int iprint     = argc - 1;
  RCP<std::ostream> outStream;
  Teuchos::oblackholestream bhs; // outputs nothing
  if (iprint > 0)
    outStream = rcp(&std::cout, false);
  else
    outStream = rcp(&bhs, false);

  int errorFlag = 0;

  try {

    int xopt_dim  = 3; // Dimension of optimization vectors
    int ce_dim = 1;    // Dimension of equality constraint
    int ci_dim = 4;    // Dimension of inequality constraint

    RCP<vec> xopt_rcp = rcp( new vec(xopt_dim,0.0) );
    RCP<vec> dopt_rcp = rcp( new vec(xopt_dim,0.0) );
    RCP<vec> vopt_rcp = rcp( new vec(xopt_dim,0.0) );
   
    RCP<vec> vec_rcp  = rcp( new vec(ce_dim,1.0) );
    RCP<vec> vel_rcp  = rcp( new vec(ce_dim,1.0) );
   
    RCP<vec> vic_rcp  = rcp( new vec(ci_dim,0.0) );
    RCP<vec> vil_rcp  = rcp( new vec(ci_dim,0.0) );
     
    // Slack variables
    RCP<vec> xs_rcp = rcp( new vec(ci_dim,1.0) );    
    RCP<vec> vs_rcp = rcp( new vec(ci_dim,0.0) );
    RCP<vec> ds_rcp = rcp( new vec(ci_dim,0.0) );

    // Feasible initial guess
    (*xopt_rcp)[0] = 0.1;
    (*xopt_rcp)[1] = 0.7;
    (*xopt_rcp)[2] = 0.2;

    RealT left = -1e0, right = 1e0;
    for (int i=0; i<xopt_dim; i++) {
      (*dopt_rcp)[i] = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
      (*vopt_rcp)[i] = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
    }    

    for (int i=0; i<ci_dim; i++) {
      (*vic_rcp)[i] = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
      (*vil_rcp)[i] = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
      (*vs_rcp)[i]  = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
      (*ds_rcp)[i]  = ( (RealT)rand() / (RealT)RAND_MAX ) * (right - left) + left;
    }    

    RCPV xopt = rcp( new SV(xopt_rcp) );
    RCPV dopt = rcp( new SV(dopt_rcp) );
    RCPV vopt = rcp( new SV(vopt_rcp) );
    RCPV vec  = rcp( new SV(vec_rcp) );
    RCPV vel  = rcp( new SV(vel_rcp) );
    RCPV vic  = rcp( new SV(vic_rcp) );
    RCPV vil  = rcp( new SV(vil_rcp) );
    RCPV xs   = rcp( new SV(xs_rcp) );
    RCPV vs   = rcp( new SV(vs_rcp) );
    RCPV ds   = rcp( new SV(ds_rcp) );

    // Partitioned vectors of optimization and slack variables
    RCPV x = CreatePartitionedVector(xopt,xs);
    RCPV v = CreatePartitionedVector(vopt,vs);
    RCPV d = CreatePartitionedVector(dopt,ds);
    RCPV vc = CreatePartitionedVector(vic,vec);
    RCPV vl = CreatePartitionedVector(vil,vel);

    // Original obective
    ROL::ZOO::Objective_HS32<RealT> obj_hs32;

    // Barrier objective
    ROL::LogBarrierObjective<RealT> barrier;

    using ROL::InteriorPoint::PenalizedObjective;

    // Interior Point objective
    RCP<ROL::Objective<RealT> > ipobj = rcp( new PenalizedObjective<RealT>(obj_hs32,barrier,*x,1.0) );

    ROL::ZOO::EqualityConstraint_HS32<RealT> eqcon_hs32;
    
    ROL::ZOO::InequalityConstraint_HS32<RealT> incon_hs32;

    using ROL::InteriorPoint::CompositeConstraint;

    // Interior point constraint
    RCP<ROL::EqualityConstraint<RealT> > ipcon = rcp( new CompositeConstraint<RealT>(incon_hs32,eqcon_hs32,*vc) );

    *outStream << "\nChecking individual objectives and constraints separately\n" << std::endl;

    *outStream << "\nObjective\n" << std::endl;
    obj_hs32.checkGradient(*xopt,*dopt,true,*outStream);
    obj_hs32.checkHessVec(*xopt,*vopt,true,*outStream);

    
    *outStream << "\nEquality Constraint\n" << std::endl;
    eqcon_hs32.checkApplyJacobian(*xopt,*vopt,*vec,true,*outStream); 
    eqcon_hs32.checkApplyAdjointJacobian(*xopt,*vel,*vec,*xopt,true,*outStream); 

    *outStream << "\nInequality Constraint\n" << std::endl;
    incon_hs32.checkApplyJacobian(*xopt,*vopt,*vic,true,*outStream); 
    incon_hs32.checkApplyAdjointJacobian(*xopt,*vil,*vic,*xopt,true,*outStream); 
    incon_hs32.checkApplyAdjointHessian(*xopt,*vil,*dopt,*xopt,true,*outStream);


    *outStream << "\nCheck Interior Point objective\n" << std::endl;
    ipobj->checkGradient(*x,*d,true,*outStream);
    ipobj->checkHessVec(*x,*v,true,*outStream);

    *outStream << "\nCheck Interior Point constraints\n" << std::endl;
    ipcon->checkApplyJacobian(*x,*v,*vc,true,*outStream);
    ipcon->checkApplyAdjointJacobian(*x,*vl,*vc,*x,true,*outStream);
    ipcon->checkApplyAdjointHessian(*x,*vl,*d,*x,true,*outStream);    

  }
  catch (std::logic_error err) {
    *outStream << err.what() << "\n";
    errorFlag = -1000;
  }; // end try

  if (errorFlag != 0)
    std::cout << "End Result: TEST FAILED\n";
  else
    std::cout << "End Result: TEST PASSED\n";

  return 0;
}

