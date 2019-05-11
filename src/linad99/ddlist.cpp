/**
\file

Author: David Fournier
Copyright (c) 2008-2012 Regents of the University of California
*/
#include "fvar.hpp"

#include <stdlib.h>
#ifndef OPT_LIB
  #include <cassert>
#endif

/**
Default constructor
*/
dlist::dlist()
{
  int on,nopt = 0;
  if ( (on=option_match(ad_comm::argc,ad_comm::argv,"-mdl",nopt))>-1)
  {
    if (nopt == 1)
    {
      int i = atoi(ad_comm::argv[on+1]);
      if (i > 0)
        gradient_structure::MAX_DLINKS = (unsigned int)i;
    }
    else
    {
      cerr << "Wrong number of options to -mdl -- must be 1"
        " you have " << nopt << endl;
      ad_exit(1);
    }
  }

  nlinks = 0;
  ddlist_space = (char*)malloc(gradient_structure::MAX_DLINKS * sizeof(double_and_int));
  variables_save = new double[gradient_structure::MAX_DLINKS];

#ifndef OPT_LIB
  //fails for insufficient memory to allocate space for dvariables save buffer
  assert(variables_save != NULL);
#endif
}
/**
Destructor
*/
dlist::~dlist()
{
  if (ddlist_space)
  {
    ::free(ddlist_space);
    ddlist_space = NULL;
  }
  if (variables_save)
  {
    delete [] variables_save;
    variables_save = NULL;
  }
}
/**
Return new unlinked node.
*/
double_and_int* dlist::create()
{
  double_and_int* ptr = get_unused();
  if (ptr == NULL)
  {
#ifndef OPT_LIB
    //If fails, then need to increase the maximum number of dlinks.
    assert(nlinks < gradient_structure::MAX_DLINKS);
#endif

    ptr = get(nlinks);

    //Keep track of the links so you can zero them out (ie gradcalc).
    ++nlinks;
  }

#ifndef OPT_LIB
    assert(ptr);
#endif

  return ptr;
}
/**
If list is not empty, pop and return last node.

\return 0 empty list.
*/
double_and_int* dlist::get_unused()
{
  double_and_int* ptr = NULL;
  if (unused.size() > 0)
  {
    ptr = unused.back();
    unused.pop_back();
  }
  return ptr;
}
double_and_int* dlist::get(const int i) const
{
  return (double_and_int*)(&ddlist_space[sizeof(double_and_int) * i]);
}

/**
Append link to list.

\param link node
*/
double_and_int* dlist::add_unused(double_and_int* ptr)
{
  unused.push_back(ptr);
  return ptr;
}
void dlist::initialize()
{
  memset(ddlist_space, 0, sizeof(double_and_int) * nlinks);
}
/**
Save variables to a buffer.
*/
void dlist::save_variables()
{
  memcpy(variables_save, ddlist_space, sizeof(double_and_int) * nlinks);
}
/**
Restore variables from buffer.
*/
void dlist::restore_variables()
{
  memcpy(ddlist_space, variables_save, sizeof(double_and_int) * nlinks);
}
