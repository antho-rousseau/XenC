/**
 *  @file simplePPL.h
 *  @brief Derived class to handle filtering mode 1: simple perplexity
 *  @author Anthony Rousseau
 *  @version 1.0.0
 *  @date 27 July 2013
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 *  The XenC tool is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef SIMPLEPPL_H_
#define SIMPLEPPL_H_

#include <boost/make_shared.hpp>

#include "mode.h"       //!< Inherit boost::filesystem from here
#include "../utils/StaticData.h"    //!< Inherit boost::shared_ptr from here

using namespace boost;

/**
 *  @class SimplePPL
 *  @brief Filtering mode 1: simple perplexity
 *
 *  This class derived from Mode handles the first filtering mode:
 *  simple perplexity
 */
class SimplePPL : public Mode {
public:
    /**
     *  @fn SimplePPL ()
     *  @brief Default constructor
     */
    SimplePPL();
    
    /**
     *  @fn ~SimplePPL ()
     *  @brief Default destructor
     */
    ~SimplePPL();
    
    /**
     *  @fn int launch ()
     *  @brief Function in charge of launching the filtering mode
     *
     *  @return 0 if the filtering succeeds
     */
    int launch();
};

#endif
