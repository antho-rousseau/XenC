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
 * $Id: xenvocab.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle the XenC vocabulary
 */

#ifndef XENVOCAB_H_
#define XENVOCAB_H_

#include <boost/algorithm/string.hpp>

#include "utils/common.h"

#include "corpus.h"
#include "xenresult.h"
#include "xenoption.h"

// SRILM include
#include "Vocab.h"

using namespace boost;

class XenVocab {
    public:
        XenVocab();
        void initialize(shared_ptr<XenFile>);
        void initialize(shared_ptr<Corpus>);
        void initialize(shared_ptr<XenResult>);
        ~XenVocab();
        // --------------------
        shared_ptr<Vocab> getVocab();
        map<string, int> getXenVocab();
        shared_ptr<XenFile> getXenFile();
        int getSize() const;
    
    private:
        shared_ptr<XenFile> ptrFile;
        shared_ptr<Vocab> ptrVocab;
        map<string, int> voc;
        // -------------------------
        void writeVocab();
        void makeVocab(shared_ptr<Corpus>);
        void makeVocab(shared_ptr<XenResult>);
};

#endif
