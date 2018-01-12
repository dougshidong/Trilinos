// @HEADER
// ************************************************************************
//
//        Phalanx: A Partial Differential Equation Field Evaluation 
//       Kernel for Flexible Management of Complex Dependency Chains
//                    Copyright 2008 Sandia Corporation
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
// Questions? Contact Roger Pawlowski (rppawlo@sandia.gov), Sandia
// National Laboratories.
//
// ************************************************************************
// @HEADER

#ifndef PHX_INTEGRATE_DIFFUSION_TERM_HPP
#define PHX_INTEGRATE_DIFFUSION_TERM_HPP

#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_DeviceEvaluator.hpp"
#include "Phalanx_MDField.hpp"
#include "Dimension.hpp"

template<typename EvalT, typename Traits>
class IntegrateDiffusionTerm : public PHX::EvaluatorWithBaseImpl<Traits>,
                               public PHX::EvaluatorDerived<EvalT, Traits>  {

  using ScalarT = typename EvalT::ScalarT;
  PHX::MDField<const ScalarT,CELL,QP,DIM> flux;
  PHX::MDField<ScalarT,CELL,BASIS> residual;

public:
  struct MyDevEvalResidual : public PHX::DeviceEvaluator<Traits> {
    PHX::View<const ScalarT***> flux;
    PHX::AtomicView<ScalarT**> residual;
    KOKKOS_FUNCTION MyDevEvalResidual(const PHX::View<const ScalarT***>& in_flux,
			      const PHX::View<ScalarT**>& in_residual) :
      flux(in_flux), residual(in_residual) {}
    KOKKOS_FUNCTION MyDevEvalResidual(const MyDevEvalResidual& src) = default;
    KOKKOS_FUNCTION void evaluate(const typename PHX::DeviceEvaluator<Traits>::member_type& team,
                                  typename Traits::EvalData workset) override;
  };

  struct MyDevEvalJacobian : public PHX::DeviceEvaluator<Traits> {
    PHX::View<const ScalarT***> flux;
#ifdef PHX_ENABLE_KOKKOS_AMT
    // Make residual atomic so that AMT mode can sum diffusion and source terms at same time
    Kokkos::View<ScalarT**,typename PHX::DevLayout<ScalarT>::type,PHX::Device,Kokkos::MemoryTraits<Kokkos::Atomic>> residual;
#else
    PHX::View<ScalarT**> residual;
#endif
    KOKKOS_FUNCTION MyDevEvalJacobian(const PHX::View<const ScalarT***>& in_flux,
			      const PHX::View<ScalarT**>& in_residual) :
      flux(in_flux), residual(in_residual) {}
    KOKKOS_FUNCTION MyDevEvalJacobian(const MyDevEvalJacobian& src) = default;
    KOKKOS_FUNCTION void evaluate(const typename PHX::DeviceEvaluator<Traits>::member_type& team,
                                  typename Traits::EvalData workset) override;
  };
  
  IntegrateDiffusionTerm(const std::string& flux_name,
                         const Teuchos::RCP<PHX::DataLayout>& flux_layout,
                         const std::string& residual_name,
                         const Teuchos::RCP<PHX::DataLayout>& residual_layout);
  PHX::DeviceEvaluator<Traits>* createDeviceEvaluator() const override;
  void evaluateFields(typename Traits::EvalData workset) override;
};

#endif
