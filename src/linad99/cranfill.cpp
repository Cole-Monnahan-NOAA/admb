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
#include <fvar.hpp>

#ifdef __TURBOC__
  #pragma hdrstop
  #include <iostream.h>
#endif

#ifdef __ZTC__
  #include <iostream.hpp>
#endif

#include <math.h>

/**
Returns the computed variance of vec.

\param vec dvector
*/
double var(const dvector& vec)
{
  double x = norm(vec) / sqrt(static_cast<double>(vec.size()));
  double y = mean(vec);
  return x * x - y * y;
}
/**
Returns the computed standand deviation of vec.

\param vec dvector
*/
double std_dev(const dvector& vec)
{
  return sqrt(var(vec));
}
/**
Returns computed mean of vec.

\param vec dvector
*/
double mean(const dvector& vec)
{
  double sum = 0;
  for (int i = vec.indexmin(); i <= vec.indexmax(); ++i)
  {
    sum += vec.elem(i);
  }
  return sum / vec.size();
}
/**
Fills dvector elements with values starting from base and incremented by offset. 

\param base initial value
\param offset incremental value
*/
void dvector::fill_seqadd(const double base, const double offset)
{
  double sequence = base;
  for (int i = indexmin(); i <= indexmax(); ++i)
  {
    elem(i) = sequence;
    sequence += offset;
  }
}
/**
Fills ivector elements with values starting from base and incremented by offset. 

\param base initial value
\param offset incremental value
*/
void ivector::fill_seqadd(int base, int offset)
{
  int sequence = base;
  for (int i = indexmin(); i <= indexmax(); ++i)
  {
    elem(i) = sequence;
    sequence += offset;
  }
}
/**
Fills ivector elements with values starting from base and incremented by offset. 

\param base initial value
\param offset incremental value
*/
void lvector::fill_seqadd(const AD_LONG_INT& base, const AD_LONG_INT& offset)
{
  AD_LONG_INT sequence = base;
  for (int i = indexmin(); i <= indexmax(); ++i)
  {
    elem(i) = sequence;
    sequence += offset;
  }
}

/**
 * Description not yet available.
 * \param
 */
void dmatrix::colfill_seqadd(const int& j, const double base,
  const double offset)
  {
    double temp=0;
    for (int i=rowmin(); i<=rowmax(); i++)
    {
      elem(i,j)=base+temp;
      temp=temp+offset;
    }
  }

/**
 * Description not yet available.
 * \param
 */
void dmatrix::colfill(int j, const dvector& v)
  {
    for (int i=rowmin(); i<=rowmax(); i++)
    {
      (*this)[i][j]=v[i];
    }
  }
/**
Fills dmatrix row at index with values from vec.

\param index of row
\param vec values to copy
*/
void dmatrix::rowfill(int index, const dvector& vec)
{
  elem(index) = vec;
}
/**
Fills dmatrix row at index with elements with values starting from base
and incremented by offset. 

\param index of row
\param base initial value
\param offset incremental value
*/
void dmatrix::rowfill_seqadd(
  const int& index,
  const double base,
  const double offset)
{
  elem(index).fill_seqadd(base, offset);
}
