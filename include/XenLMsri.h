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
 * $Id: XenLMsri.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle SRI LM estimation, querying, ...
 */

#ifndef XENLMSRI_H_
#define XENLMSRI_H_

#include <boost/algorithm/string.hpp>
#include <boost/shared_array.hpp>

// SRILM include
#include "Ngram.h"

#include "corpus.h"
#include "xenvocab.h"
#include "xenoption.h"
#include "xenresult.h"

#define MAX_ORDER 9
#define MAX_WORDS 16384
#define MAX_CHARS MAX_WORDS * 16

using namespace boost;

class XenLMsri {
    public:
        XenLMsri();
        void initialize(shared_ptr<Corpus>, shared_ptr<XenVocab>);
        void initialize(shared_ptr<XenFile>, shared_ptr<XenVocab>);
        void initialize(shared_ptr<XenResult>, shared_ptr<XenVocab>, int, string name = "");
        ~XenLMsri();
        // ------------
        int createLM();
        int loadLM();
        int writeLM();
        string getFileName() const;
        TextStats getSentenceStats(string);
        TextStats getDocumentStats(shared_ptr<Corpus>);
    
    private:
        shared_ptr<Corpus> ptrCorp;
        shared_ptr<XenVocab> ptrVoc;
        shared_ptr<Vocab> ptrVocab;
        shared_ptr<XenResult> ptrXR;
        int pc;
        shared_ptr<NgramStats> ptrNStats;
        shared_array<Discount*> ptrDiscounts;
        shared_ptr<Ngram> ptrLM;
        char* textFile;
        char* lmFile;
        unsigned order;
        int interpolate[10];
        int kndiscount[10];
        unsigned gtmin[10];
        int writeBinaryLM;
        int keepunk;
        
        // ---------------------------
        string makeLMname(XenOption*);
};

#endif
