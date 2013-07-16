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
 * $Id: sourcephrase.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "sourcephrase.h"

/*
 *  Constructor from a string representing the source phrase
 */
SourcePhrase::SourcePhrase(string src) : source(src) {
    ptrTargets = shared_ptr<vector<string> >(new vector<string>);
    ptrScores = shared_ptr<vector<string> >(new vector<string>);
    ptrAlignments = shared_ptr<vector<string> >(new vector<string>);
    ptrCounts = shared_ptr<vector<string> >(new vector<string>);
    ptrScoresXE = shared_ptr<Score>(new Score);
}

/*
 *  Destructor
 */
SourcePhrase::~SourcePhrase() { }

//  ------
//  Public
//  ------

/*
 *  Returns the source phrase
 */
string SourcePhrase::getSource() const {
    return source;
}

/*
 *  Returns the target phrases size for this source
 */
unsigned int SourcePhrase::getTargetSize() const {
    return (unsigned int)ptrTargets->size();
}

/*
 *  Adds a target phrase to this source
 */
void SourcePhrase::addTarget(string s) {
    ptrTargets->push_back(s);
}

/*
 *  Adds scores to this source
 */
void SourcePhrase::addScores(string s) {
    ptrScores->push_back(s);
}

/*
 *  Adds alignments to this source
 */
void SourcePhrase::addAlignments(string s) {
    ptrAlignments->push_back(s);
}

/*
 *  Adds counts to this source
 */
void SourcePhrase::addCounts(string s) {
    ptrCounts->push_back(s);
}

/*
 *  Returns the Score object for this source
 */
Score* SourcePhrase::getScoresXE() {
    return &*ptrScoresXE;
}
