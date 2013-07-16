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
 * $Id: similarity.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle similarity scores computation
 *  WARNING: untested + experimental
 */

#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <boost/algorithm/string.hpp>

#include "utils/common.h"

#include "corpus.h"
#include "xenvocab.h"
#include "xenoption.h"

using namespace boost;

typedef map<int, float> SimMap;

class Similarity {
    public:
        Similarity();
        void initialize(shared_ptr<Corpus>, shared_ptr<Corpus>, shared_ptr<XenVocab>);
        ~Similarity();
        // ---------------
        float getSim(int);
        int getSize() const;
    
    private:
        shared_ptr<XenVocab> ptrVoc;
        shared_ptr<Corpus> ptrID;
        shared_ptr<Corpus> ptrOOD;
        shared_ptr<map<string, float> > ptrIdTfIdf;
        shared_ptr<map<string, float> > ptrOodTfIdf;
        shared_ptr<vector<string> > ptrWords;
        shared_ptr<map<string, int> > ptrIdTf;
        shared_ptr<map<string, int> > ptrOodTf;
        shared_ptr<vector<string> > ptrVecWords;
        shared_ptr<vector<float> > ptrIdVecTfIdf;
        shared_ptr<vector<float> > ptrOodVecIdf;
        shared_ptr<SimMap> ptrOodSimilarity;
        // ------------------
        void loadWords();
        void computeInDomainTFIDF();
        void computeOutOfDomainTFIDF();
        void buildIDVector();
        void computeOutOfDomainIDF();
        void computeSimilarity();
};

#endif
