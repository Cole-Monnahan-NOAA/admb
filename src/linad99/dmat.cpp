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

/**
 * Description not yet available.
 * \param
 */
 struct dvec_ptr_ptr
 {
   void ** m;
 };

/**
 * Description not yet available.
 * \param
 */
 dmatrix::dmatrix( int nrl,  int nrh, int ncl,  int nch)
 {
   allocate(nrl,nrh,ncl,nch);
 }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(int nrl,int nrh,int ncl,int nch)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   m -= rowmin();
   for (int i=rowmin(); i<=rowmax(); i++)
   {
     m[i].allocate(ncl,nch);
   }
 }

/**
 * Description not yet available.
 * \param
 */
 dmatrix dmatrix::sub(int nrl,int nrh)
 {
   if (allocated(*this))
   {
     dmatrix tmp(nrl,nrh);
     for (int i=nrl; i<=nrh; i++)
     {
       tmp[i].shallow_copy((*this)(i));
     }
     return tmp;
   }
   else
   {
     return *this;
   }
 }

/**
 * Description not yet available.
 * \param
 */
 dmatrix::dmatrix(int nrl,int nrh)
 {
   allocate(nrl,nrh);
 }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(int nrl,int nrh)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   m -= rowmin();
 }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(ad_integer nrl,ad_integer nrh)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   m -= rowmin();
 }
/**
Returns the minimum value of matrix m.
Note: m is an allocated matrix.

\param m scalar matrix
*/
double min(const dmatrix& m)
{
  double minimum = min(m(m.rowmin()));
  for (int i = m.rowmin() + 1; i <= m.rowmax(); ++i)
  {
    double value = min(m(i));
    if (value < minimum) minimum = value;
  }
  return minimum;
}
/**
Returns the minimum value of matrix m.
Note: m is an allocated matrix.

\param m scalar matrix
*/
double max(const dmatrix& m)
{
  double maximum = max(m(m.rowmin()));
  for (int i = m.rowmin() + 1; i <= m.rowmax(); ++i)
  {
    double value = max(m(i));
    if (value > maximum) maximum = value;
  }
  return maximum;
}
/**
 * Description not yet available.
 * \param
 */
void dmatrix::allocate(const dmatrix& dm)
{
  int nrl=dm.rowmin();
  int nrh=dm.rowmax();
  index_min=nrl;
  index_max=nrh;

  if ( (m = new dvector [rowsize()]) == 0)
  {
    cerr << " Error allocating memory in dmatrix contructor" << endl;
    ad_exit(21);
  }

  if ( (shape = new mat_shapex(m))== 0)
  {
    cerr << " Error allocating memory in dmatrix contructor" << endl;
    ad_exit(21);
  }
  m -= rowmin();
  for (int i=rowmin(); i<=rowmax(); i++)
  {
    m[i].allocate(dm(i));
  }
}

/**
 * Description not yet available.
 * \param
 */
int ivector_check(const ivector& v, int l, int u)
  {
    if (v.indexmin()!=l||v.indexmax()!=u) return 1;
    return 0;
  }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(int nrl,int nrh,const ivector& ncl,const ivector& nch)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }

   if (nrl !=ncl.indexmin() || nrh !=ncl.indexmax() ||
     nrl !=nch.indexmin() || nrh !=nch.indexmax())
   {
     cerr << "Incompatible array bounds in "
     "dmatrix(int nrl,int nrh,const ivector& ncl,const ivector& nch)\n";
     ad_exit(1);
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }

   m -= rowmin();
   for (int i=rowmin(); i<=rowmax(); i++)
   {
     m[i].allocate(ncl(i),nch(i));
   }
 }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(int nrl,int nrh,int ncl,const ivector& nch)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }
   if (nrl !=nch.indexmin() || nrh !=nch.indexmax())
   {
     cerr << "Incompatible array bounds in dmatrix(int nrl,int nrh,"
       "int ncl,const ivector& nch)\n";
     ad_exit(1);
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   m -= rowmin();
   for (int i=rowmin(); i<=rowmax(); i++)
   {
     m[i].allocate(ncl,nch(i));
   }
 }

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::allocate(int nrl,int nrh,const ivector& ncl,int nch)
 {
   if (nrh<nrl)
   {
     allocate();
     return;
   }
   if (nrl !=ncl.indexmin() || nrh !=ncl.indexmax())
   {
     cerr << "Incompatible array bounds in dmatrix(int nrl,int nrh,"
       " int ncl,const ivector& nch)\n";
     ad_exit(1);
   }
   index_min=nrl;
   index_max=nrh;
   if ( (m = new dvector [rowsize()]) == 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   if ( (shape = new mat_shapex(m))== 0)
   {
     cerr << " Error allocating memory in dmatrix contructor\n";
     ad_exit(21);
   }
   m -= rowmin();
   for (int i=rowmin(); i<=rowmax(); i++)
   {
     m[i].allocate(ncl(i),nch);
   }
 }

/*
 dmatrix::dmatrix(void)
 {
   #ifdef DIAG
     myheapcheck("Entering dmatrix(nrl,nrh,ncl,nch)" );
   #endif

   shape = NULL;
   m=NULL;
 }
*/

/**
 * Description not yet available.
 * \param
 */
dmatrix::dmatrix(int nrl, int nrh, const ivector& ncl, const ivector& nch)
 {
   allocate(nrl,nrh,ncl,nch);
 }

/**
 * Description not yet available.
 * \param
 */
dmatrix::dmatrix(int nrl, int nrh, int ncl, const ivector& nch)
 {
   allocate(nrl,nrh,ncl,nch);
 }

/**
 * Description not yet available.
 * \param
 */
dmatrix::dmatrix(const dmatrix& m2)
 {
   index_min=m2.index_min;
   index_max=m2.index_max;
   shape=m2.shape;
   if (shape)
   {
     (shape->ncopies)++;
   }
#ifdef SAFE_ALL
   else
   {
     cerr << "Making a copy of an unallocated dmatrix"<<endl;
   }
#endif
   m = m2.m;
 }

/**
 * Description not yet available.
 * \param
 */
void dmatrix::shallow_copy(const dmatrix& m2)
 {
   index_min=m2.index_min;
   index_max=m2.index_max;
   shape=m2.shape;
   if (shape)
   {
     (shape->ncopies)++;
   }
#ifdef SAFE_ALL
   else
   {
     cerr << "Making a copy of an unallocated dmatrix"<<endl;
   }
#endif
   m = m2.m;
 }

/// Destructor
dmatrix::~dmatrix()
{
  deallocate();
}
/**
Returns dvector with cube value of each element in vec.
Note: vec is allocated vector.

\param vec scalar vector
*/
dvector cube(const dvector& vec)
{
  dvector results;
  results.allocate(vec);
  for (int i = results.indexmin(); i <= results.indexmax(); ++i)
  {
    results(i) = cube(vec(i));
  }
  return results;
}
/**
Returns dmatrix with cube value of each element in mat.
Note: mat is allocated matrix.

\param mat scalar matrix.
*/
dmatrix cube(const dmatrix& mat)
{
  dmatrix results;
  results.allocate(mat);
  for (int i = results.rowmin(); i <= results.rowmax(); ++i)
  {
    results(i) = cube(mat(i));
  }
  return results;
}

/**
 * Description not yet available.
 * \param
 */
 void dmatrix::deallocate()
 {
/*
   static int testflag=0;
   static int ycounter=0;
   if (testflag)
   {
     ycounter++;
     cout << " A " << ycounter << endl;
     test_the_pointer();
   }
*/
   if (shape)
   {
     if (shape->ncopies)
     {
       (shape->ncopies)--;
     }
     else
     {
       m= (dvector*) (shape->get_pointer());
       delete [] m;
       m=NULL;
       delete shape;
       shape=NULL;
     }
   }
/*
   if (testflag)
   {
     cout << " B " << ycounter << endl;
     test_the_pointer();
   }
*/
#ifdef SAFE_ALL
   else
   {
     cerr << "Warning -- trying to deallocate an unallocated dmatrix"<<endl;
   }
#endif
 }
