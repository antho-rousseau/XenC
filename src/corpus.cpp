/**
 *  @file corpus.cpp
 *  @brief Class handling corpus-related functionalities
 *  @author Anthony Rousseau
 *  @version 1.2.0
 *  @date 19 August 2013
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 *  Development of the XenC tool has been partially funded by the
 *  European Commission under the MateCat project.
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

#include "corpus.h"
#include "utils/xenio.h"

Corpus::Corpus() {
    wc = 0;
}

void Corpus::initialize(boost::shared_ptr<XenFile> ptrData, std::string lg) {
    ptrFile = ptrData;
    lang = lg;
    
    try {
        if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
            std::cout << "Specified corpus " << ptrFile->getFullPath() << " exists! We continue..." << std::endl;
            
            loadText();
            wc = Corpus::wordCount();
        }
        else
            throw XenCommon::XenCEption("Specified corpus " + ptrFile->getFullPath() + " does not exists! Exiting.");
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

void Corpus::initialize(std::string filePath, std::string lg) {
    ptrFile = boost::make_shared<XenFile>();
    ptrFile->initialize(filePath);
    lang = lg;
    
    try {
        if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
            std::cout << "Specified corpus " << ptrFile->getFullPath() << " exists! We continue..." << std::endl;
            
            loadText();
            wc = Corpus::wordCount();
        }
        else
            throw XenCommon::XenCEption("Specified corpus " + ptrFile->getFullPath() + " does not exists! Exiting.");
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

Corpus::~Corpus() {
    
}

boost::shared_ptr<XenFile> Corpus::getXenFile() const {
    return ptrFile;
}

std::string Corpus::getLine(int line) {
	return ptrText->operator[](line);
}

unsigned int Corpus::getSize() const {
	return (unsigned int)ptrText->size();
}

std::string Corpus::getLang() const {
	return lang;
}

bool Corpus::getPrint(int line) {
    if (ptrPrint->operator[](line) == 0)
        return false;
    else
        return true;
}

int Corpus::getWC() const {
	return wc;
}

void Corpus::removeLine(int line) {
    ptrPrint->operator[](line) = 0;
}

void Corpus::loadText() {
    ptrText = boost::make_shared<std::vector<std::string> >(XenIO::read(ptrFile));
    ptrPrint = boost::make_shared<std::vector<int> >(ptrText->size(), 1);
}

int Corpus::wordCount() {
	int res = 0;
    
	for (unsigned int i = 0; i < ptrText->size(); i++) {
        res = res + XenCommon::wordCount(ptrText->operator[](i));
    }
    
	return res;
}
