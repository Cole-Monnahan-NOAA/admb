/*
 * $Id$
 *
 * Author: David Fournier
 * Copyright (c) 2008-2012 Regents of the University of California
 */
/**
 * \file
 * Description not yet available.
 */
#include "fvar.hpp"

/** Cube of a number; variable object.
Optimized for derivative computations.
\ingroup misc
\param m number to be cubed
\return \f$m^3\f$
*/
prevariable& cube(const prevariable& v1)
{
  double x=value(v1);
  double x2=x*x;
  gradient_structure* gs = gradient_structure::get();
  dvariable* RETURN_PTR = gs->get_RETURN_PTR();
  RETURN_PTR->v->x=x2*x;
  gs->GRAD_STACK1->set_gradient_stack(default_evaluation2,
     &(RETURN_PTR->v->x), &(v1.v->x),3*x2 );
   return *RETURN_PTR;
 }

/** Cube of a number; variable object.
Optimized for derivative computations.
\ingroup misc
\param m number to be cubed
\return \f$m^4\f$
*/
prevariable& fourth(const prevariable& v1)
{
  double x=value(v1);
  double x2=x*x;
  gradient_structure* gs = gradient_structure::get();
  dvariable* RETURN_PTR = gs->get_RETURN_PTR();
  RETURN_PTR->v->x=x2*x2;
  gs->GRAD_STACK1->set_gradient_stack(default_evaluation2,
     &(RETURN_PTR->v->x), &(v1.v->x),4*x2*x );
   return *RETURN_PTR;
 }
