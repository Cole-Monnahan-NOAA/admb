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
  ddlist_space =
    (char*)malloc(gradient_structure::MAX_DLINKS * sizeof(dlink));
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
dlink* dlist::create()
{
#ifndef OPT_LIB
  //If fails, then need to increase the maximum number of dlinks.
  assert(nlinks < gradient_structure::MAX_DLINKS);
#endif

  dlink* link = (dlink*)(&ddlist_space[sizeof(dlink) * nlinks]);

#ifndef OPT_LIB
  assert(link);
#endif

  //Do not add to list.
  link->prev = NULL;

  //Keep track of the links so you can zero them out (ie gradcalc).
  dlink_addresses.push_back(link);
  ++nlinks;

  return link;
}
/**
If list is not empty, pop and return last node.

\return 0 empty list.
*/
dlink* dlist::last_remove()
{
  dlink* last = NULL;
  if (unused.size() > 0)
  {
    last = unused.back();
    unused.pop_back();
  }
  return last;
}
/**
Append link to list.

\param link node
*/
dlink* dlist::append(dlink* link)
{
  unused.push_back(link);
  return link;
}
void dlist::initialize()
{
  for (dlink* src: dlink_addresses)
  {
    (*src).di.x = 0;
  }
}
/**
Save variables to a buffer.
*/
void dlist::save_variables()
{
  double* dest = variables_save;
  for (dlink* src: dlink_addresses)
  {
    *dest = (*src).di.x;
    ++dest;
  }
}
/**
Restore variables from buffer.
*/
void dlist::restore_variables()
{
  double* src = variables_save;
  for (dlink* dest: dlink_addresses)
  {
    (*dest).di.x = *src;
    ++src;
  }
}
/**
Get total addresses stored.
*/
size_t dlist::total_addresses() const
{
  return dlink_addresses.size();
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
