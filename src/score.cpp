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
 * $Id: score.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "score.h"

/*
 *  Constructor
 */
Score::Score() {
    ptrScores = shared_ptr<vector<double> >(new vector<double>);
    ptrPrint = shared_ptr<vector<int> >(new vector<int>);
}

/*
 *  Destructor
 */
Score::~Score() {
    
}

//  ------
//  Public
//  ------

/*
 *  Adds a score to the scores list
 */
void Score::addScore(double d) {
    ptrScores->push_back(d);
    ptrPrint->push_back(1);
}

/*
 *  Removes the nth score from the score list
 */
void Score::removeScore(int n) {
    ptrPrint->operator[](n) = 0;
}

/*
 *  Returns the nth score from the list
 */
double Score::getScore(int n) const {
    return ptrScores->operator[](n);
}

/*
 *  Returns the arithmetic mean of all scores
 */
double Score::getMean() const {
    double total = 0.0;
    
    for (unsigned int i = 0; i < ptrScores->size(); i++) {
		total = total + ptrScores->operator[](i);
	}
    
    return (double)(total / ptrScores->size());
}

/*
 *  Tells if the nth score should figure in the
 *  final sorted file
 */
bool Score::getPrint(int n) const {
    if (ptrPrint->operator[](n) == 0)
        return false;
    else
        return true;
}

/*
 *  Returns the size of the scores list
 */
unsigned int Score::getSize() const {
    return (unsigned int)ptrScores->size();
}

/*
 *  Calibrates the scores list from 0 to 1
 */
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

/*
 *  Inverts the scores in the list (1 - score)
 */
void Score::inverse() {
	for (unsigned int i = 0; i < ptrScores->size(); i++) {
		ptrScores->operator[](i) = (double)(1 - ptrScores->operator[](i));
	}
}
