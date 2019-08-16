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
#if defined(__TURBOC__)
  #pragma hdrstop
  #include <alloc.h>
#endif

#include <stdlib.h>

#ifndef OPT_LIB
  #include <cassert>
  #include <climits>
#endif

/**
 * Description not yet available.
 * \param
 */
dvar_vector& dvar_vector::shift(int min)
{
  va += indexmin()-min;
  index_max=index_max-index_min+min;
  index_min=min;
  shape->shift(min);
  return *this;
}

/**
 * Description not yet available.
 * \param
 */
dvar_vector::dvar_vector(const independent_variables& t)
{
  allocate(t.indexmin(),t.indexmax());
  if (va)
  {
    for (int i=indexmin(); i<=indexmax(); i++)
    {
      va[i].x=(t.v)[i];
    }
    gradient_structure* gs = gradient_structure::get();
    if (gs)
    {
      gs->INDVAR_LIST.add(*this);
    }
  }
}
dvariable::dvariable(const independent_variables& t)
{
  if (t.size() >= 1)
  {
    gradient_structure* gs = gradient_structure::get();
    if (gs)
    {
      v = gs->GRAD_LIST.create();
      v->x = t(t.indexmin());
      gs->INDVAR_LIST.add(*this);
    }
  }
}

/**
 * Description not yet available.
 * \param
 */
 dvar_vector::dvar_vector(const dvector& t)
 {
   if (!t)
   {
     allocate();
   }
   else
   {
     va=NULL;
     allocate(t.indexmin(),t.indexmax());
     initialize();
     for (int i = indexmin(); i <= indexmax(); i++)
     {
       va[i].x=(t.v)[i];
     }
   }
 }


//#ifdef __BORLANDC__
 //prevariable dvar_vector::elem(int i) { return (va+i); }
//#endif

/**
 * Description not yet available.
 * \param
 */
dvar_vector::dvar_vector(unsigned int sz, double* x)
{
#ifndef OPT_LIB
  assert(sz > 0 && sz <= INT_MAX);
#endif
  allocate(0, (int)(sz - 1));
  for (unsigned int i = 0; i < sz; i++)
  {
    va[i].x = x[i];
  }
}

/**
 * Description not yet available.
 * \param
 */
 dvar_vector::dvar_vector(int ncl,int nch)
 {
   if (ncl>nch)
     allocate();
   else
   {
     va=NULL;
     allocate(ncl,nch);
   }
   #ifdef SAFE_INITIALIZE
     initialize();
   #endif

   #ifdef DIAG
     cout << " Allocating dvar_vector with ptr_address\n  "
         << &va << "  pointing at  " << (va+indexmin()) << "\n";
   #endif
 }

/*
 dvar_vector::dvar_vector(const ad_integer& ncl,const ad_integer& nch)
 {
   allocate(ncl,nch);
   #ifdef SAFE_INITIALIZE
     initialize();
   #endif

   #ifdef DIAG
     cout << " Allocating dvar_vector with ptr_address\n  "
         << &va << "  pointing at  " << (va+indexmin()) << "\n";
   #endif
 }
 */

/**
Default constructor
*/
dvar_vector::dvar_vector()
{
  allocate();
}

/**
 * Description not yet available.
 * \param
 */
void make_indvar_list(const dvar_vector& t)
{
  gradient_structure* gs = gradient_structure::get();
  if (gs)
  {
    gs->INDVAR_LIST.set_addresses(t);
  }
}
void indvar_offset_list::add(const dvariable& t)
{
  address.push_back(&(t.v->x));
}
void indvar_offset_list::add(const dvar_vector& t)
{
  clear();

  int min = t.indexmin();
  int max = t.indexmax();
  for (int i = min; i <= max; ++i)
  {
    address.push_back(&(t.va[i].x));
  }
}
void indvar_offset_list::set_addresses(const dvar_vector& t)
{
  add(t);
}

/**
 * Description not yet available.
 * \param
 */
void copy_status(const ostream& _s, const dvar_vector& v)
   {
     ostream& s= (ostream&) _s;
     s << " copy_flag ";
     s <<"\n";
   }

/**
Does not allocate, but initializes dvar_vector to empty.
*/
void dvar_vector::allocate()
{
  va = NULL;
  link_ptr = NULL;
  shape = NULL;
  index_min = 0;
  index_max = -1;
}
/**
Allocate dvar_vector using indexes from v1.
*/
void dvar_vector::allocate(const dvector& v1)
{
  allocate(v1.indexmin(), v1.indexmax());
}
/**
Allocate dvar_vector using indexes from v1.
*/
void dvar_vector::allocate(const dvar_vector& v1)
{
  allocate(v1.indexmin(), v1.indexmax());
}

/**
 * Description not yet available.
 * \param
 */
void dvar_vector::allocatec(const dvar_vector& t)
   {
     if (!(*this))
     {
       if (t.shape)
       {
         shape=t.shape;
         (shape->ncopies)++;
       }
       else
       {
         //cerr << "Making a copy of an unallocated dvar_vector"<<endl;
       }
       link_ptr=t.link_ptr;
       index_min=t.index_min;
       index_max=t.index_max;
       va = t.va;
     }
     else
     {
       cerr << "Trying to alocate to an already allocated dvar_vector" << endl;
     }
   }

/**
Allocate dvar_vector with indexmin = ncl and indexmax = nch.
*/
void dvar_vector::allocate(int ncl, int nch)
{
  if (ncl > nch)
  {
    allocate();
  }
  else
  {
    index_min=ncl;
    index_max=nch;
    unsigned int itemp = (unsigned int)(nch - ncl + 1);
/*
    if (itemp<=0)
    {
         cerr << "Error in dvar_vector constructor max index must be"
                 " >= minindex\n"
            << "minindex = " << ncl << " maxindex = " << nch <<endl;
         ad_exit(1);
    }
*/
    if ((va = arr_new(itemp)) == 0)
    {
      cerr << " Error trying to allocate memory for dvar_vector\n";
      ad_exit(1);
    }
    else
    {
      if ( (shape=new vector_shapex(ncl,nch,va)) == NULL)
      {
        cerr << "Error trying to allocate memory for dvar_vector\n";
        ad_exit(1);
      }
      link_ptr=* (arr_link **) va;
      va -= indexmin();
      // if ( ((int)va) %8) cerr << "Array is not QWORD alligned" << endl;
#ifdef DIAG
      myheapcheck("Leaving dvar_vector::allocate(ncl,nch)");
#endif
    }
  }
}

/**
 * Description not yet available.
 * \param
 */
   void dvar_vector::allocate(const ad_integer& ncl,const ad_integer& nch)
   {
     allocate(int(ncl),int(nch));
   }
