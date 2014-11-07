/**
 *  @file sourcephrase.cpp
 *  @brief Class holding a merged source phrase and all associated data
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
 *  under the terms of the GNU Lesser General Public License version 3 as
 *  published by the Free Software Foundation
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "sourcephrase.h"

SourcePhrase::SourcePhrase(std::string src) : source(src) {
    ptrTargets = boost::make_shared<vector<string> >();
    ptrScores = boost::make_shared<vector<string> >();
    ptrAlignments = boost::make_shared<vector<string> >();
    ptrCounts = boost::make_shared<vector<string> >();
    ptrScoresXE = boost::make_shared<Score>();
}

SourcePhrase::~SourcePhrase() {

}

std::string SourcePhrase::getSource() const {
    return source;
}

unsigned int SourcePhrase::getTargetSize() const {
    return (unsigned int)ptrTargets->size();
}

void SourcePhrase::addTarget(std::string s) {
    ptrTargets->push_back(s);
}

void SourcePhrase::addScores(std::string s) {
    ptrScores->push_back(s);
}

void SourcePhrase::addAlignments(std::string s) {
    ptrAlignments->push_back(s);
}

void SourcePhrase::addCounts(std::string s) {
    ptrCounts->push_back(s);
}

boost::shared_ptr<Score> SourcePhrase::getScoresXE() const {
    return ptrScoresXE;
}
