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
 * $Id: phrasetable.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "phrasetable.h"
#include "utils/xenio.h"

/*
 *  Default constructor
 */
PhraseTable::PhraseTable() {
    
}

/*
 *  Initialization from an already instanciated XenFile
 */
void PhraseTable::initialize(shared_ptr<XenFile> data) {
    XenOption* opt = XenOption::getInstance();
	ptrFile = data;
    dlm = " ||| ";
    lineSpl = *new Splitter("", dlm);
    
    if (exists(ptrFile->getFullPath().c_str())) {
        string cmd = "";
        if (ptrFile->isGZ())
            cmd = "zcat " + ptrFile->getFullPath() + " | wc -l";
        else
            cmd = "wc -l " + ptrFile->getFullPath();
        
        num = toInt(XenCommon::getStdoutFromCommand(cmd));
        
		cout << "Specified phrase-table " << ptrFile->getFullPath() << " exists! We continue..." << endl;
        cout << "Size = " << num << endl;
        
        if (opt->getLocal())
            mergePhrasesBySource();
    }
	else {
		cout << "Specified phrase-table " << ptrFile->getFullPath() << " does not exists! Exiting." << endl;
		exit (1);
	}
}

/*
 *  Destructor
 */
PhraseTable::~PhraseTable() {

}

//  ------
//  Public
//  ------

/*
 *  Returns the associated XenFile
 */
XenFile* PhraseTable::getXenFile() {
    return &*ptrFile;
}

/*
 *  Returns the nth source phrase
 */
string PhraseTable::getSource(int n) {
    if (ptrPhPairs->empty()) { loadTable(); }
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[0];
}

/*
 *  Returns the nth target phrase
 */
string PhraseTable::getTarget(int n) {
    if (ptrPhPairs->empty()) { loadTable(); }
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[1];
}

/*
 *  Returns the nth scores list
 */
string PhraseTable::getScores(int n) {
    if (ptrPhPairs->empty()) { loadTable(); }
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[2];
}

/*
 *  Returns the nth aligment info
 */
string PhraseTable::getAlignment(int n) {
    if (ptrPhPairs->empty()) { loadTable(); }
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[3];
}

/*
 *  Returns the nth counts info
 */
string PhraseTable::getCounts(int n) {
    if (ptrPhPairs->empty()) { loadTable(); }
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[4];
}

/*
 *  Returns a vector containing all sources phrases
 *  and their associated target phrases, scores,
 *  alignments and counts
 */
vector<SourcePhrase> PhraseTable::getSrcPhrases() {
    if (ptrSrcPhrases->empty()) { mergePhrasesBySource(); }
    return *ptrSrcPhrases;
}

/*
 *  Sets a new vector of source phrases
 */
void PhraseTable::setSrcPhrases(vector<SourcePhrase> vSP) {
    ptrSrcPhrases = shared_ptr<vector<SourcePhrase> >(new vector<SourcePhrase>(vSP));
}

/*
 *  Returns the phrase table size
 */
unsigned int PhraseTable::getSize() {
    if (ptrPhPairs->empty()) { loadTable(); }
    return (unsigned int)ptrPhPairs->size();
}

//  -------
//  Private
//  -------

/*
 *  Reads the phrase table file and
 *  loads it into memory
 */
void PhraseTable::loadTable() {
    if (ptrPhPairs->empty()) {
        ptrPhPairs = shared_ptr<vector<string> >(new vector<string>(XenIO::read(ptrFile)));
        
        cout << "Table size = " << ptrPhPairs->size() << endl;
        
        if (ptrPhPairs->size() == num + 1)
            ptrPhPairs->pop_back();
    }
}

/*
 *  Merges the phrase table elements by
 *  source phrase in a dedicated structure
 *  (see SourcePhrase.h/cpp), including
 *  target phrases, scores, alignments and counts
 */
void PhraseTable::mergePhrasesBySource() {
    cout << "Merging phrases by source." << endl;
    
    if (ptrPhPairs->empty()) { loadTable(); }
    
    for (unsigned int i = 0; i < ptrPhPairs->size(); i++) {
        SourcePhrase src(getSource(i));

        unsigned int j = i;

        while (j < ptrPhPairs->size() && getSource(j).compare(src.getSource()) == 0) {
            src.addTarget(getTarget(j));
            src.addScores(getScores(j));
            src.addAlignments(getAlignment(j));
            src.addCounts(getCounts(j));
            j++;
        }

        ptrSrcPhrases->push_back(src);
        
        i = j - 1;
    }
    
    cout << "Phrases have been merged by source." << endl;
}
