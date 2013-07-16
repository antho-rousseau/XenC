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
 * $Id: xenvocab.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "xenvocab.h"

/*
 *  Default constructor
 */
XenVocab::XenVocab() {

}

/*
 *  Initialization from a XenFile
 */
void XenVocab::initialize(shared_ptr<XenFile> file) {
	ptrFile = file;
    ptrVocab = shared_ptr<Vocab>(new Vocab);
    
	if (exists(ptrFile->getFullPath().c_str())) {
		cout << "Using existing vocab " << ptrFile->getFullPath() << endl;
        File file(ptrFile->getFullPath().c_str(), "r");
        ptrVocab->read(file);
	}
	else {
		cout << "Specified vocab " << ptrFile->getFullPath() << " does not exists!" << endl;
		exit (1);
	}
}

/*
 *  Initialization from a Corpus
 */
void XenVocab::initialize(shared_ptr<Corpus> ptrCorp) {
	ptrFile = shared_ptr<XenFile>(new XenFile);
    ptrFile->initialize(ptrCorp->getXenFile()->getPrefix() + ptrCorp->getLang() + ".vocab");
	ptrVocab = shared_ptr<Vocab>(new Vocab);
    
    makeVocab(ptrCorp);
    
	if (exists(ptrFile->getFullPath().c_str())) {
		cout << "Using existing vocab " << ptrFile->getFullPath() << endl;
	}
	else {
		cout << "Dumping vocab " << ptrFile->getFullPath() << endl;
        
		writeVocab();
        
        cout << "Vocab file " + ptrFile->getFullPath() + " has been dumped." << endl;
	}
}

/*
 *  Initialization from a XenResult
 */
void XenVocab::initialize(shared_ptr<XenResult> ptrXR) {
    XenOption* opt = XenOption::getInstance();
    
	ptrFile = shared_ptr<XenFile>(new XenFile);
    ptrFile->initialize(ptrXR->getXenFile()->getPrefix() + opt->getSLang() + ".vocab");
	ptrVocab = shared_ptr<Vocab>(new Vocab);
    
    makeVocab(ptrXR);
    
	if (exists(ptrFile->getFullPath().c_str())) {
		cout << "Using existing vocab " << ptrFile->getFullPath() << endl;
	}
	else {
		cout << "Dumping vocab " << ptrFile->getFullPath() << endl;
        
		writeVocab();
        
        cout << "Vocab file " + ptrFile->getFullPath() + " has been dumped." << endl;
	}
}

/*
 *  Destructor
 */
XenVocab::~XenVocab() {

}

//  ------
//  Public
//  ------

/*
 *  Returns the size of the vocabulary
 */
int XenVocab::getSize() const {
    return (int)ptrVocab->numWords();
}

/*
 *  Returns the XenFile associated to the vocab
 */
shared_ptr<XenFile> XenVocab::getXenFile() {
    return ptrFile;
}

/*
 *  Returns the SRI Vocab object
 */
shared_ptr<Vocab> XenVocab::getVocab() {
    return ptrVocab;
}

/*
 *  Returns the map containing the vocab
 */
map<string, int> XenVocab::getXenVocab() {
    return voc;
}

//  -------
//  Private
//  -------

/*
 *  Dumps the vocab from a corpus to the disk
 */
void XenVocab::writeVocab() {
	File file(ptrFile->getFullPath().c_str(), "w");
    ptrVocab->write(file);
}

/*
 *  Creates and fills the vocab from a Corpus
 */
void XenVocab::makeVocab(shared_ptr<Corpus> ptrCorp) {
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
        vector<string> lWords;
        string line = ptrCorp->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            ptrVocab->addWord(lWords[j].c_str());
            voc[lWords[j]] = 1;
        }
    }
}

/*
 *  Creates and fills the vocab from a XenResult
 */
void XenVocab::makeVocab(shared_ptr<XenResult> ptrXR) {
    for (unsigned int i = 0; i < ptrXR->getSize(); i++) {
        vector<string> lWords;
        string line = ptrXR->getTextLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            ptrVocab->addWord(lWords[j].c_str());
            voc[lWords[j]] = 1;
        }
    }
}
