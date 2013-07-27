/**
 *  @file wfile.cpp
 *  @brief Class handling a file with values intended at weighting XenC scores
 *  @author Anthony Rousseau
 *  @version 1.0.0
 *  @date 27 July 2013
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

#include "wfile.h"

Wfile::Wfile() {
    
}

void Wfile::initialize(boost::shared_ptr<XenFile> ptrFile) {
    try {
        if (boost::filesystem::exists(ptrFile->getFullPath().c_str())) {
            std::cout << "Using existing weights file " << ptrFile->getFullPath() << std::endl;
            
            this->ptrFile = ptrFile;
            
            loadWeights();
            calibrate();
        }
        else {
            throw XenCommon::XenCEption("Specified weights file " + ptrFile->getFullPath() + " does not exists!");
        }
    }
    catch (XenCommon::XenCEption &e) {
        throw;
    }
}

Wfile::~Wfile() {
    
}

double Wfile::getWeight(int n) {
    return ptrWeights->operator[](n);
}

unsigned int Wfile::getSize() const {
    return (unsigned int)ptrWeights->size();
}

void Wfile::loadWeights() {
    std::vector<std::string> tmp = XenIO::read(ptrFile);
    
    for (unsigned int i = 0; i < tmp.size(); i++)
        ptrWeights->push_back(XenCommon::toDouble(tmp[i]));
}

void Wfile::calibrate() {
    std::cout << "Starting weights calibration." << std::endl;
    
	double min = 0;
	double max = 0;
    
	for (std::vector<double>::iterator iter = ptrWeights->begin(); iter != ptrWeights->end(); ++iter) {
		if (*iter > max) { max = *iter; }
		if (*iter < min) { min = *iter; }
	}
    
	for (unsigned int i = 0; i < ptrWeights->size(); i++) {
		ptrWeights->operator[](i) = ((ptrWeights->operator[](i) - min) / (max - min));
	}
    
    std::cout << "Weights calibration complete." << std::endl;
}
