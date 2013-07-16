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
 * $Id: score.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle the XenC scores
 */

#ifndef SCORE_H_
#define SCORE_H_

#include "utils/common.h"

#include <boost/shared_ptr.hpp>

using namespace boost;

class Score {
    public:
        Score();
        ~Score();
        // ------
        void addScore(double);
        void removeScore(int);
        double getScore(int) const;
        bool getPrint(int) const;
        unsigned int getSize() const;
        double getMean() const;
        void calibrate();
        void inverse();
        
        
    
    private:
        shared_ptr<vector<double> > ptrScores;
        shared_ptr<vector<int> > ptrPrint;
};

#endif
