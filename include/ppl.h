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
 * $Id: ppl.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle the various PPL/Xentropy computations
 */

#ifndef PPL_H_
#define PPL_H_

#include "utils/common.h"
#include "utils/threadpool.hpp"

#include "corpus.h"
#include "phrasetable.h"
#include "XenLMsri.h"

using namespace boost::threadpool;

void taskCalcPPL(int, string, shared_ptr<vector<double> >, shared_ptr<XenLMsri>);  // Thread-safe function for parallel PPL computation

class PPL {
    public:
        PPL();
        void initialize(shared_ptr<PhraseTable>, shared_ptr<XenLMsri>, bool);
        void initialize(shared_ptr<Corpus>, shared_ptr<XenLMsri>);
        ~PPL();
        // --------------------------
        unsigned int getSize() const;
        double getPPL(int);
        double getXE(int);
        double getCorpPPL();
        void calcPPLCorpus();
        void calcPPLPhraseTable();
    
    private:
        shared_ptr<XenLMsri> ptrLM;
        shared_ptr<Corpus> ptrCorp;
        shared_ptr<PhraseTable> ptrPT;
        bool source;
        shared_ptr<vector<double> > ptrPPL;
        pool threadPool;
        // -------------
        static double crossEntropy(double);
};

#endif
