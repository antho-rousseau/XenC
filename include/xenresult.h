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
 * $Id: xenresult.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle a XenC result file (*.sorted.gz)
 */

#ifndef XENRESULT_H_
#define XENRESULT_H_

#include <boost/algorithm/string.hpp>

#include "utils/common.h"

#include "xenfile.h"

using namespace boost;

class XenIO;

class XenResult {
    public:
        XenResult();
        void initialize(shared_ptr<XenFile>);
        ~XenResult();
        // ----------------
        vector<string> getSortedText() const;
        string getTextLine(int) const;
        unsigned int getSize() const;
        XenFile* getXenFile();
    
    private:
        shared_ptr<XenFile> ptrResult;
        vector<double> sc;
        vector<string> txt;
};

#endif
