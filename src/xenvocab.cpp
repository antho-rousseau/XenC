/**
 *  @file xenvocab.cpp
 *  @brief Class handling a XenC vocabulary
 *  @author Anthony Rousseau
 *  @version 2.0.0
 *  @date 18 March 2016
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013-2016, Anthony Rousseau, LIUM, University of Le Mans, France
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

#include "../include/xenvocab.h"
#include "../include/utils/xenio.h"

XenVocab::XenVocab() {

}

void XenVocab::initialize(boost::shared_ptr<XenFile> ptrFile) {
	this->ptrFile = ptrFile;

    try {
        if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
            std::cout << "Using existing vocab " << ptrFile->getFullPath() << std::endl;
        }
        else {
            throw XenCommon::XenCEption("Specified vocab " + ptrFile->getFullPath() + " does not exists!");
        }
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

void XenVocab::initialize(boost::shared_ptr<Corpus> ptrCorp) {
	ptrFile = boost::make_shared<XenFile>();
    ptrFile->initialize(ptrCorp->getXenFile()->getPrefix() + ptrCorp->getLang() + ".vocab");

    makeVocab(ptrCorp);
    
	if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
        std::cout << "Using existing vocab " << ptrFile->getFullPath() << std::endl;
	}
	else {
        std::cout << "Dumping vocab " << ptrFile->getFullPath() << std::endl;
        
		writeVocab();
        
        std::cout << "Vocab file " + ptrFile->getFullPath() + " has been dumped." << std::endl;
	}
}

void XenVocab::initialize(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp) {
    ptrFile = boost::make_shared<XenFile>();
    ptrFile->initialize(ptrInCorp->getXenFile()->getPrefix() + "-" + ptrOutCorp->getXenFile()->getPrefix() + ptrInCorp->getLang() + ".vocab");

    makeVocab(ptrInCorp, ptrOutCorp);
    
	if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
        std::cout << "Using existing vocab " << ptrFile->getFullPath() << std::endl;
	}
	else {
        std::cout << "Dumping vocab " << ptrFile->getFullPath() << std::endl;
        
		writeVocab();
        
        std::cout << "Vocab file " + ptrFile->getFullPath() + " has been dumped." << std::endl;
	}
}

void XenVocab::initialize(boost::shared_ptr<XenResult> ptrXenRes) {
    XenOption* opt = XenOption::getInstance();
    
	ptrFile = boost::make_shared<XenFile>();
    ptrFile->initialize(ptrXenRes->getXenFile()->getPrefix() + opt->getSLang() + ".vocab");

    makeVocab(ptrXenRes);
    
	if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
        std::cout << "Using existing vocab " << ptrFile->getFullPath() << std::endl;
	}
	else {
        std::cout << "Dumping vocab " << ptrFile->getFullPath() << std::endl;
        
		writeVocab();
        
        std::cout << "Vocab file " + ptrFile->getFullPath() + " has been dumped." << std::endl;
	}
}

XenVocab::~XenVocab() {

}

unsigned int XenVocab::getSize() const {
    return (unsigned int)voc.size();
}

boost::shared_ptr<XenFile> XenVocab::getXenFile() const {
    return ptrFile;
}

std::map<std::string, int> XenVocab::getXenVocab() const {
    return voc;
}

void XenVocab::writeVocab() {
    XenIO::writeVocab(voc, ptrFile->getFullPath());
}

void XenVocab::makeVocab(boost::shared_ptr<XenFile> ptrFile) {
    std::vector<std::string> vec = XenIO::read(ptrFile);

    for (unsigned int i = 0; i < vec.size(); i++)
            voc[vec[i]] = 1;
}

void XenVocab::makeVocab(boost::shared_ptr<Corpus> ptrCorp) {
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrCorp->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            voc[lWords[j]] = 1;
        }
    }
}

void XenVocab::makeVocab(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp) {
    for (unsigned int i = 0; i < ptrInCorp->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrInCorp->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            voc[lWords[j]] = 1;
        }
    }
    
    for (unsigned int i = 0; i < ptrOutCorp->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrOutCorp->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            voc[lWords[j]] = 1;
        }
    }
}

void XenVocab::makeVocab(boost::shared_ptr<XenResult> ptrXenRes) {
    for (unsigned int i = 0; i < ptrXenRes->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrXenRes->getTextLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            voc[lWords[j]] = 1;
        }
    }
}
