/**
 *  @file phrasetable.cpp
 *  @brief Class handling phrase-table related functionalities
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

#include "phrasetable.h"
#include "utils/xenio.h"

PhraseTable::PhraseTable() {
    size = 0;
}

void PhraseTable::initialize(boost::shared_ptr<XenFile> ptrData) {
    XenOption* opt = XenOption::getInstance();
	ptrFile = ptrData;
    dlm = " ||| ";
    lineSpl = *new XenCommon::Splitter("", dlm);
    
    try {
        if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
            std::string cmd = "";
            if (ptrFile->isGZ())
                cmd = "zcat " + ptrFile->getFullPath() + " | wc -l";
            else
                cmd = "wc -l " + ptrFile->getFullPath();
            
            size = XenCommon::toInt(XenCommon::getStdoutFromCommand(cmd));
            
            std::cout << "Specified phrase-table " << ptrFile->getFullPath() << " exists! We continue..." << std::endl;
            std::cout << "Size = " << size << std::endl;
            
            loadTable();
            
            if (opt->getLocal())
                mergePhrasesBySource();
        }
        else {
            throw XenCommon::XenCEption("Specified phrase-table " + ptrFile->getFullPath() + " does not exists! Exiting.");
        }
    }
    catch (XenCommon::XenCEption &e) {
        throw;
    }
}

PhraseTable::~PhraseTable() {

}

boost::shared_ptr<XenFile> PhraseTable::getXenFile() const {
    return ptrFile;
}

std::string PhraseTable::getSource(int n) {
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[0];
}

std::string PhraseTable::getTarget(int n) {
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[1];
}

std::string PhraseTable::getScores(int n) {
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[2];
}

std::string PhraseTable::getAlignment(int n) {
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[3];
}

std::string PhraseTable::getCounts(int n) {
    lineSpl.reset(ptrPhPairs->operator[](n), dlm);
    return lineSpl[4];
}

std::vector<SourcePhrase> PhraseTable::getSrcPhrases() {
    if (ptrSrcPhrases->empty()) { mergePhrasesBySource(); }
    return *ptrSrcPhrases;
}

void PhraseTable::setSrcPhrases(vector<SourcePhrase> vSP) {
    ptrSrcPhrases = boost::make_shared<vector<SourcePhrase> >(vSP);
}

unsigned int PhraseTable::getSize() const {
    return (unsigned int)ptrPhPairs->size();
}

void PhraseTable::loadTable() {
    if (ptrPhPairs->empty()) {
        ptrPhPairs = boost::make_shared<vector<string> >(XenIO::read(ptrFile));
        
        std::cout << "Table size = " << ptrPhPairs->size() << std::endl;
        
        if (ptrPhPairs->size() == size + 1)
            ptrPhPairs->pop_back();
    }
}

void PhraseTable::mergePhrasesBySource() {
    std::cout << "Merging phrases by source." << std::endl;
    
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
    
    std::cout << "Phrases have been merged by source." << std::endl;
}
