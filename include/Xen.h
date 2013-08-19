/**
 *  @file Xen.h
 *  @brief Main file of XenC, controls execution
 *  @author Anthony Rousseau
 *  @version 1.2.0
 *  @date 19 August 2013
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

#ifndef XEN_H_
#define XEN_H_

#include <boost/program_options.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "utils/common.h"
#include "utils/xenio.h"
#include "modes/simplePPL.h"
#include "modes/monoXEntropy.h"
#include "modes/biXEntropy.h"
#include "modes/ptScoring.h"
#include "eval.h"
#include "mode.h"
#include "xenoption.h"
#include "utils/StaticData.h"

using namespace boost::program_options;
using namespace boost::filesystem;
using namespace boost;

namespace po = boost::program_options;

/**
 *  @fn int main (int argc, char* argv[])
 *  @brief Main function of XenC
 *
 *  @param argc :   number of arguments
 *  @param argv :   passed arguments to the program
 *  @return 0 if execution ended well
 */
int main(int argc, char* argv[]);

/**
 *  @fn std::string sanityCheck (XenOption* opt)
 *  @brief Controls the mandatory options
 *
 *  @param opt :    XenOption object containing all the passed options
 *  @return 0 if all is good, an error message otherwise
 */
std::string sanityCheck(XenOption* opt);

/**
 *  @fn std::string getOutName (XenOption* opt)
 *  @brief Computes the output file name
 *
 *  @param opt :    XenOption object containing all the passed options
 *  @return the output file name
 */
std::string getOutName(XenOption* opt);

#endif
