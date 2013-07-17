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
 * $Id: corpus.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "corpus.h"
#include "utils/xenio.h"

/*
 *  Default constructor
 */
Corpus::Corpus() {
    
}

/*
 *  Initialization from an already instanciated XenFile
 */
void Corpus::initialize(shared_ptr<XenFile> data, string lg) {
    ptrFile = data;
    lang = lg;
    
    try {
        if (exists(ptrFile->getFullPath().c_str())) {
            cout << "Specified corpus " << ptrFile->getFullPath() << " exists! We continue..." << endl;
        
            loadText();
            wc = Corpus::wordCount();
        }
        else
            throw XenCommon::XenCEption("Specified corpus " + ptrFile->getFullPath() + " does not exists! Exiting.");
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

/*
 *  Initialization from a string containing a valid path/file name
 */
void Corpus::initialize(string data, string lg) {
    ptrFile = shared_ptr<XenFile>(new XenFile);
    ptrFile->initialize(data);
    lang = lg;
    
    try {
        if (exists(ptrFile->getFullPath().c_str())) {
            cout << "Specified corpus " << ptrFile->getFullPath() << " exists! We continue..." << endl;
            
            loadText();
            wc = Corpus::wordCount();
        }
        else
            throw XenCommon::XenCEption("Specified corpus " + ptrFile->getFullPath() + " does not exists! Exiting.");
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

/* 
 *  Destructor
 */
Corpus::~Corpus() {

}

//  ------
//  Public
//  ------

/*
 *  Return XenFile associated to the corpus
 */
XenFile* Corpus::getXenFile() {
    return &*ptrFile;
}

/*
 *  Returns nth line of the Corpus
 */
string Corpus::getLine(int n) {
	return ptrText->operator[](n);
}

/*
 *  Returns Corpus number of lines
 */
unsigned int Corpus::getSize() {
	return (unsigned int)ptrText->size();
}

/*
 *  Return corpus language
 */
string Corpus::getLang() const {
	return lang;
}

/*
 *  Returns whether the specified line
 *  should be printed in final filtered file
 */
bool Corpus::getPrint(int n) {
    if (ptrPrint->operator[](n) == 0)
        return false;
    else
        return true;
}

/*
 *  Returns Corpus word count
 */
int Corpus::getWC() const {
	return wc;
}

/*
 *  Removes the nth line from the corpus
 *  (put print[i] to zero so it will not be printed)
 */
void Corpus::removeLine(int n) {
    ptrPrint->operator[](n) = 0;
}

//  -------
//  Private
//  -------

/*
 *  Loads Corpus data in memory
 */
void Corpus::loadText() {
        ptrText = shared_ptr<vector<string> >(new vector<string>(XenIO::read(ptrFile)));
        ptrPrint = shared_ptr<vector<int> >(new vector<int>(ptrText->size(), 1));
}

/*
 *  Counts number of tokens in the corpus
 */
int Corpus::wordCount() {
	int res = 0;
    
	for (unsigned int i = 0; i < ptrText->size(); i++) {
        res = res + XenCommon::wordCount(ptrText->operator[](i));
    }
    
	return res;
}
