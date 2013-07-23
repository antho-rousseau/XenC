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
 * $Id: XenLMsri.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "XenLMsri.h"

/*
 *  Default constructor
 */
XenLMsri::XenLMsri() {
    XenOption* opt = XenOption::getInstance();

    order = opt->getOrder();
    discount = opt->getDiscount();
    writeBinaryLM = opt->getBinLM();
    
    for (int i = 0; i < MAX_ORDER + 1; i++)
        gtmin[i] = 0;
    
    gtmax[0] = 5; gtmax[1] = 1;
    
    for (int i = 2; i < MAX_ORDER + 1; i++)
        gtmax[i] = 7;
    
    ptrCorp = shared_ptr<Corpus>(new Corpus);
    ptrVoc = shared_ptr<XenVocab>(new XenVocab);
    ptrXR = shared_ptr<XenResult>(new XenResult);
    ptrVocab = shared_ptr<Vocab>(new Vocab);
    ptrNStats = shared_ptr<NgramStats>(new NgramStats(*ptrVocab, order));
    ptrDiscounts = shared_array<Discount*>(new Discount*[order]);
    ptrLM = shared_ptr<Ngram>(new Ngram(*ptrVocab, order));
    textFile = NULL;
    lmFile = NULL;
}

/*
 *  Initialization from a Corpus & a XenVocab
 */
void XenLMsri::initialize(shared_ptr<Corpus> c, shared_ptr<XenVocab> v) {
    XenOption* opt = XenOption::getInstance();

    ptrCorp = c;
    ptrVoc = v;

    keepunk = 1;
    
    ptrVocab->operator=(*ptrVoc->getVocab());
    ptrVocab->unkIsWord() = keepunk ? true : false;
    
    ptrXR = shared_ptr<XenResult>(new XenResult);
    pc = 0;
    
    interpolate[0] = 1;
    
    textFile = new char[ptrCorp->getXenFile()->getFullPath().length() + 1];
    strcpy(textFile, ptrCorp->getXenFile()->getFullPath().c_str());

    lmFile = new char[makeLMname(opt).length() + 1];
    strcpy(lmFile, makeLMname(opt).c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

/*
 *  Initialization from a XenFile & a XenVocab
 */
void XenLMsri::initialize(shared_ptr<XenFile> f, shared_ptr<XenVocab> v) {
    ptrCorp = shared_ptr<Corpus>(new Corpus);
    ptrVoc = v;
    
    keepunk = 1;
    
    ptrVocab->operator=(*ptrVoc->getVocab());
    ptrVocab->unkIsWord() = keepunk ? true : false;

    ptrXR = shared_ptr<XenResult>(new XenResult);
    pc = 0;

    interpolate[0] = 1;
    
    textFile = NULL;
    
    lmFile = new char[f->getFullPath().length() + 1];
    strcpy(lmFile, f->getFullPath().c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

/*
 *  Initialization from a XenResult (for eval)
 */
void XenLMsri::initialize(shared_ptr<XenResult> ptrXenRes, shared_ptr<XenVocab> v, int p, string name) {
    ptrCorp = shared_ptr<Corpus>(new Corpus);
    ptrVoc = v;
    
    keepunk = 1;
    
    ptrVocab->operator=(*ptrVoc->getVocab());
    ptrVocab->unkIsWord() = keepunk ? true : false;
    
    ptrXR = ptrXenRes;
    pc = p;
    
    interpolate[0] = 1;
    
    textFile = NULL;
    
    lmFile = new char[name.length() + 1];
    strcpy(lmFile, name.c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

/*
 *  Destructor
 */
XenLMsri::~XenLMsri() {

}

//  ------
//  Public
//  ------

/*
 *  Estimates a new LM in memory
 */
int XenLMsri::createLM() {
    if (exists(lmFile))
        cout << "LM file already here, reusing..." << endl;
    else {
        assert(ptrVocab);
        
#define USE_STATS(what) (ptrNStats->what)
        assert(ptrNStats != 0);
        
        USE_STATS(vocab) = *ptrVocab;
        USE_STATS(openVocab) = false;

        // LM estimation from a file
        if (ptrXR->getSize() == 0) {
            cout << "Estimating LM from file on disk..." << endl;
            
            File file(textFile, "r");
            USE_STATS(countFile(file, 0));
        }
        // LM estimation from a vector of strings in memory
        else {
            cout << "Estimating LM from data in memory..." << endl;
            
            char str[MAX_CHARS];
            shared_array<VocabString> vstr = shared_array<VocabString>(new VocabString[MAX_WORDS + 1]);
            
            unsigned int nbLines = (unsigned int)(ptrXR->getSize() / 100) * pc;
            
            try {
                for (unsigned int i = 0; i < nbLines; i++) {
                    strcpy(str, ptrXR->getTextLine(i).c_str());
                    
                    int nw = ptrVocab->parseWords(str, vstr.get(), MAX_WORDS + 1);
                    
                    if (nw == MAX_WORDS + 1)
                        throw XenCommon::XenCEption("Too many words in one hypothesis.");
                    
                    USE_STATS(countSentence(vstr.get()));
                }
            } catch (XenCommon::XenCEption &e) {
                throw;
            }
        }

        assert(ptrDiscounts != 0);
        unsigned i;
        for (i = 0; i < order; i ++) {
            ptrDiscounts.get()[i] = 0;
        }

        try {
            for (i = 1; i <= order; i++) {
                unsigned useorder = (i > MAX_ORDER) ? 0 : i;
                Discount* dcnt = 0;
                
                switch (discount) {
                    case 0:
                        dcnt = new ModKneserNey(gtmin[useorder], 0, 0);
                        break;
                        
                    case 1:
                        dcnt = new GoodTuring(gtmin[useorder], gtmax[useorder]);
                        break;
                        
                    case 2:
                        dcnt = new WittenBell(gtmin[useorder]);
                        break;
                        
                    default:
                        std::cout << "Discounting method ID must be 0 (ModKneserNey), 1 (GoodTuring) or 2 (WittenBell). Using default (ModKneserNey)." << std::endl;
                        dcnt = new ModKneserNey(gtmin[useorder], 0, 0);
                        break;
                }
                
                assert(dcnt);
                
                if (dcnt) {
                    if (interpolate[0] || interpolate[useorder]) {
                        dcnt->interpolate = true;
                    }
                    
                    if (!dcnt->estimate(*ptrNStats, i))
                        throw XenCommon::XenCEption("SRILM says: \"Error in discount estimator for order " + toString(i) + "\". Data too small for ModKN (0)? Try GT (1) or WB (2) as discounting method.");
                    
                    ptrDiscounts.get()[i-1] = dcnt;
                }
            }
        } catch (XenCommon::XenCEption &e) {
            throw;
        }
        
        if (lmFile) {
            assert(ptrLM != 0);
            
            try {
                if (!ptrLM->estimate(*ptrNStats, ptrDiscounts.get())) {
                    throw XenCommon::XenCEption("LM estimation failed.");
                }
            } catch (XenCommon::XenCEption &e) {
                throw;
            }
        }
        
        cout << "LM estimation done." << endl;
    }
    
    return 0;
}

/*
 *  Loads a LM into memory
 */
int XenLMsri::loadLM() {
    assert(ptrVocab);
    
    ptrVocab->unkIsWord() = keepunk ? true : false;
    
    #define USE_STATS(what) (ptrNStats->what)
    assert(ptrNStats != 0);
    
    USE_STATS(vocab) = *ptrVocab;
    USE_STATS(openVocab) = false;
    
    try {
        File file(lmFile, "r");
        
        if (!ptrLM->read(file)) {
            throw XenCommon::XenCEption("LM reading failed.");
        }
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    return(0);
}

/*
 *  Writes a LM on disk
 */
int XenLMsri::writeLM() {
    try {
        if (!exists(lmFile)) {
            if (writeBinaryLM) {
                File file(lmFile, "wb");
                
                if (!ptrLM->writeBinary(file))
                    throw XenCommon::XenCEption("LM binary writing failed.");
            }
            else {
                File file(lmFile, "w");
                
                if (!ptrLM->write(file))
                    throw XenCommon::XenCEption("LM writing failed.");
            }
        }
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    return (0);
}

/*
 *  Returns the LM file name
 */
string XenLMsri::getFileName() const {
    return lmFile;
}

TextStats XenLMsri::getSentenceStats(string s) {
    TextStats tstats;
    
    try {
        char str[MAX_CHARS];
        VocabString vstr[MAX_WORDS + 1];
        
        strcpy(str, s.c_str());
        
        int nw = ptrVocab->parseWords(str, vstr, MAX_WORDS + 1);
        
        if (nw == MAX_WORDS + 1)
            throw XenCommon::XenCEption("Too many words in one hypothesis.");
        
        ptrLM->sentenceProb(vstr, tstats);
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    return tstats;
}

TextStats XenLMsri::getDocumentStats(shared_ptr<Corpus> c) {
    TextStats documentStats;
    
    for (unsigned int i = 0; i < c->getSize(); i++) {
        documentStats.increment(getSentenceStats(c->getLine(i)));
    }
    
    return documentStats;
}

//  -------
//  Private
//  -------

/*
 *  Constructs the LM name from the program options
 */
string XenLMsri::makeLMname(XenOption* opt) {
    string ret = opt->getInSData()->getDirName() + "/" + ptrCorp->getXenFile()->getPrefix() + "." + ptrVoc->getXenFile()->getPrefix() + "." + toString(opt->getOrder()) + "g." + opt->getSmooth() + "." + opt->getCutoff() + ".";
    if (opt->getBinLM()) { ret = ret + "sblm"; }
    else { ret = ret + "arpa.gz"; }
    
    return ret;
}
