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
 * $Id: wfile.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "wfile.h"

/*
 *  Default constructor
 */
Wfile::Wfile() {
    
}

/*
 *  Initialization from a XenFile
 */
void Wfile::initialize(shared_ptr<XenFile> xf) {
    if (exists(xf->getFullPath().c_str())) {
		cout << "Using existing weights file " << xf->getFullPath() << endl;
	}
	else {
		cout << "Specified weights file " << xf->getFullPath() << " does not exists!" << endl;
		exit (1);
	}
    
    ptrFile = xf;
}

/*
 *  Destructor
 */
Wfile::~Wfile() {
    
}

//  ------
//  Public
//  ------

/*
 *  Returns the nth score from the weight file
 */
double Wfile::getScore(int n) {
    if (ptrWeights->size() == 0) { loadWeights(); }
    
    return ptrWeights->operator[](n);
}

/*
 *  Returns the size of the weights list
 */
unsigned int Wfile::getSize() const {
    return (unsigned int)ptrWeights->size();
}

//  -------
//  Private
//  -------

/*
 *  Loads the weights from the file
 */
void Wfile::loadWeights() {
    vector<string> tmp = XenIO::read(ptrFile);
    
    for (unsigned int i = 0; i < tmp.size(); i++) {
        ptrWeights->push_back(toDouble(tmp[i]));
    }
    
    calibrate();
}

/*
 *  Calibrates the weights from 0 to 1
 */
void Wfile::calibrate() {
	cout << "Starting weights calibration." << endl;
    
	double min = 0;
	double max = 0;
    
	for (std::vector<double>::iterator iter = ptrWeights->begin(); iter != ptrWeights->end(); ++iter) {
		if (*iter > max) { max = *iter; }
		if (*iter < min) { min = *iter; }
	}
    
	for (unsigned int i = 0; i < ptrWeights->size(); i++) {
		ptrWeights->operator[](i) = ((ptrWeights->operator[](i) - min) / (max - min));
	}
    
	cout << "Weights calibration complete." << endl;
}
