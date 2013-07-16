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
 * $Id: xenresult.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "xenresult.h"
#include "utils/xenio.h"

/*
 *  Default constructor
 */
XenResult::XenResult() {

}

/*
 *  Initialization from a XenFile
 */
void XenResult::initialize(shared_ptr<XenFile> f) {
    ptrResult = f;
    
    vector<string> tmp = XenIO::read(ptrResult);
    
    for (unsigned int i = 0; i < tmp.size(); i++) {
        vector<string> strs;
        boost::split(strs, tmp[i], is_any_of("\t"));
        
        double s = atof(strs[0].c_str());
        string t = strs[1];
        
        sc.push_back(s);
        txt.push_back(t);
    }
}

XenResult::~XenResult() {
    
}

//  ------
//  Public
//  ------

/*
 *  Returns the whole sorted text
 *  (not the scores)
 */
vector<string> XenResult::getSortedText() const {
    return txt;
}

/*
 *  Returns the nth text line
 */
string XenResult::getTextLine(int n) const {
    return txt[n];
}

/*
 *  Returns the text size
 */
unsigned int XenResult::getSize() const {
    return (unsigned int)txt.size();
}

/*
 *  Returns the associated XenFile
 */
XenFile* XenResult::getXenFile() {
    return &*ptrResult;
}
