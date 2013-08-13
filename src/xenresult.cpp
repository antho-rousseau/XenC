/**
 *  @file xenresult.cpp
 *  @brief Class handling a XenC sorted result file for evaluation/best point
 *  @author Anthony Rousseau
 *  @version 1.1.0
 *  @date 13 August 2013
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

#include "xenresult.h"
#include "utils/xenio.h"

XenResult::XenResult() {

}

void XenResult::initialize(boost::shared_ptr<XenFile> ptrFile) {
    ptrResult = ptrFile;
    
    std::vector<std::string> tmp = XenIO::read(ptrResult);
    
    for (unsigned int i = 0; i < tmp.size(); i++) {
        std::vector<std::string> strs;
        boost::split(strs, tmp[i], boost::is_any_of("\t"));

        txt.push_back(strs[1]);
    }
}

XenResult::~XenResult() {
    
}

std::vector<std::string> XenResult::getSortedText() const {
    return txt;
}

std::string XenResult::getTextLine(int n){
    return txt[n];
}

unsigned int XenResult::getSize() const {
    return (unsigned int)txt.size();
}

boost::shared_ptr<XenFile> XenResult::getXenFile() const {
    return ptrResult;
}
