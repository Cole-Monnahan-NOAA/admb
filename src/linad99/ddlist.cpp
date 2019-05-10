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
  double_and_int* link = last_remove();
  if (link == NULL)
  {
#ifndef OPT_LIB
    //If fails, then need to increase the maximum number of dlinks.
    assert(nlinks < gradient_structure::MAX_DLINKS);
#endif

    link = (double_and_int*)(&ddlist_space[sizeof(double_and_int) * nlinks]);

#ifndef OPT_LIB
    assert(link);
#endif

    //Keep track of the links so you can zero them out (ie gradcalc).
    ++nlinks;
  }

  return link;
}
/**
If list is not empty, pop and return last node.

\return 0 empty list.
*/
double_and_int* dlist::last_remove()
{
  double_and_int* last = NULL;
  if (unused.size() > 0)
  {
    last = unused.back();
    unused.pop_back();
  }
  return last;
}
double* dlist::get(const int i) const
{
  double_and_int* ret = (double_and_int*)(&ddlist_space[sizeof(double_and_int) * i]);
  return &(ret->x);
}

/**
Append link to list.

\param link node
*/
double_and_int* dlist::append(double_and_int* link)
{
  unused.push_back(link);
  return link;
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
/**
Get total addresses stored.
*/
size_t dlist::total_addresses() const
{
  return nlinks - unused.size();
}
/**
Check link list integrity.
*/
/*
void dlist::check_list(void)
{
  dlink* tmp_last=last;

  unsigned int count=0;
  while(tmp_last && count <=nlinks)
  {
    count+=1;
    if (count > nlinks)
    {
      cerr << "In check_list() number of links > number created\n";
      cerr << " The number created was "<< nlinks << endl;
    }

    dlink* tmp = tmp_last->prev;

//  cout << "last =" << _farptr_tolong(last) << "\n";
//  cout << "last->prev =" << _farptr_tolong(last->prev) << "\n";
//  cout << "deleted dlink with address" << _farptr_tolong(last) << "\n";

    tmp_last = tmp;
  }
  cerr << "In check_list() number of free links is " << count << endl;
}
*/
