/*
 * This file is part of the cross-entropy tool for data selection (XenC)
 * aimed at speech recognition and statistical machine translation.
 *
 * Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 * The XenC tool is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: Xen.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Main class of XenC
 *  Controls execution
 */

#ifndef XEN_H_
#define XEN_H_

#include <boost/program_options.hpp>

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
using namespace boost;

namespace po = boost::program_options;

string sanityCheck(XenOption*);
string getOutName(XenOption*);

#endif
