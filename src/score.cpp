/**
 *  @file score.cpp
 *  @brief Class holding the XenC scores representation
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

#include "score.h"

Score::Score() {
    ptrScores = boost::make_shared<vector<double> >();
    ptrPrint = boost::make_shared<vector<int> >();
}

Score::~Score() {
    
}

void Score::addScore(double sc) {
    ptrScores->push_back(sc);
    ptrPrint->push_back(1);
}

void Score::removeScore(int n) {
    ptrPrint->operator[](n) = 0;
}

double Score::getScore(int n) const {
    return ptrScores->operator[](n);
}

bool Score::getPrint(int n) const {
    return (bool)ptrPrint->operator[](n);
}

unsigned int Score::getSize() const {
    return (unsigned int)ptrScores->size();
}

void Score::calibrate() {
	double min = 0;
	double max = 0;
    
	for (std::vector<double>::iterator iter = ptrScores->begin(); iter != ptrScores->end(); ++iter) {
		if (*iter > max) { max = *iter; }
		if (*iter < min) { min = *iter; }
	}
    
	for (unsigned int i = 0; i < ptrScores->size(); i++) {
		ptrScores->operator[](i) = ((ptrScores->operator[](i) - min) / (max - min));
	}
}

void Score::inverse() {
	for (unsigned int i = 0; i < ptrScores->size(); i++) {
		ptrScores->operator[](i) = (double)(1 - ptrScores->operator[](i));
	}
}
