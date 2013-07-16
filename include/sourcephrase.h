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
 * $Id: sourcephrase.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Simple class to handle the source phrases structure
 */

#ifndef SOURCEPHRASE_H_
#define SOURCEPHRASE_H_

#include "utils/common.h"

#include "score.h"

class SourcePhrase {
    public:
        SourcePhrase(string);
        ~SourcePhrase();
        // -------------
        string getSource() const;
        unsigned int getTargetSize() const;
        Score* getScoresXE();
        void addTarget(string);
        void addScores(string);
        void addAlignments(string);
        void addCounts(string);
    private:
        string source;
        shared_ptr<vector<string> > ptrTargets;
        shared_ptr<vector<string> > ptrScores;
        shared_ptr<vector<string> > ptrAlignments;
        shared_ptr<vector<string> > ptrCounts;
        shared_ptr<Score> ptrScoresXE;
};

#endif
