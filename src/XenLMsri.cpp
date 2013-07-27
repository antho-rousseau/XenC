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
    
    textFile = NULL;
    lmFile = NULL;
    
    pc = 0;
}

void XenLMsri::initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc) {
    XenOption* opt = XenOption::getInstance();
    
    this->ptrCorp = ptrCorp;
    this->ptrVoc = ptrVoc;

    ptrVocab = ptrVoc->getVocab();
    ptrVocab->unkIsWord() = true;
    
    ptrNStats = boost::shared_ptr<NgramStats>(new NgramStats(*ptrVocab, order));
    ptrDiscounts = boost::shared_array<Discount*>(new Discount*[order]);
    ptrLM = boost::shared_ptr<Ngram>(new Ngram(*ptrVocab, order));

    ptrXR = boost::make_shared<XenResult>();    //!< No use for a XenResult here
    pc = 0;
    
    textFile = new char[ptrCorp->getXenFile()->getFullPath().length() + 1];
    std::strcpy(textFile, ptrCorp->getXenFile()->getFullPath().c_str());

    std::string name = makeLMname(opt);
    lmFile = new char[name.length() + 1];
    std::strcpy(lmFile, name.c_str());
    
    std::setlocale(LC_CTYPE, "");
    std::setlocale(LC_COLLATE, "");
}

void XenLMsri::initialize(boost::shared_ptr<XenFile> ptrFile, boost::shared_ptr<XenVocab> ptrVoc) {
    ptrCorp = boost::make_shared<Corpus>();     //!< No use for a corpus here
    this->ptrVoc = ptrVoc;
    
    ptrVocab = ptrVoc->getVocab();
    ptrVocab->unkIsWord() = true;

    ptrNStats = boost::shared_ptr<NgramStats>(new NgramStats(*ptrVocab, order));
    ptrDiscounts = boost::shared_array<Discount*>(new Discount*[order]);
    ptrLM = boost::shared_ptr<Ngram>(new Ngram(*ptrVocab, order));
    
    ptrXR = boost::make_shared<XenResult>();    //!< No use for a XenResult here
    pc = 0;

    textFile = new char[1];
    std::strcpy(textFile, "");
    
    lmFile = new char[ptrFile->getFullPath().length() + 1];
    std::strcpy(lmFile, ptrFile->getFullPath().c_str());
    
    std::setlocale(LC_CTYPE, "");
    std::setlocale(LC_COLLATE, "");
}

void XenLMsri::initialize(boost::shared_ptr<XenResult> ptrXenRes, boost::shared_ptr<XenVocab> ptrVoc, int pc, std::string name) {
    ptrCorp = boost::make_shared<Corpus>();     //!< No use for a corpus here
    this->ptrVoc = ptrVoc;
    
    ptrVocab = ptrVoc->getVocab();
    ptrVocab->unkIsWord() = true;
    
    ptrNStats = boost::shared_ptr<NgramStats>(new NgramStats(*ptrVocab, order));
    ptrDiscounts = boost::shared_array<Discount*>(new Discount*[order]);
    ptrLM = boost::shared_ptr<Ngram>(new Ngram(*ptrVocab, order));
    
    ptrXR = ptrXenRes;
    this->pc = pc;

    textFile = new char[1];
    std::strcpy(textFile, "");
    
    lmFile = new char[name.length() + 1];
    std::strcpy(lmFile, name.c_str());
    
    std::setlocale(LC_CTYPE, "");
    std::setlocale(LC_COLLATE, "");
}

XenLMsri::~XenLMsri() {
    delete[] lmFile;
    delete[] textFile;
}

int XenLMsri::createLM() {
    if (boost::filesystem::exists(lmFile))
        std::cout << "LM file already here, reusing..." << std::endl;
    else {
        assert(ptrVocab);
        
#define USE_STATS(what) (ptrNStats->what)
        assert(ptrNStats != 0);
        
        USE_STATS(vocab) = *ptrVocab;
        USE_STATS(openVocab) = false;

        // LM estimation from a file
        if (ptrXR->getSize() == 0) {
            std::cout << "Estimating LM from file on disk..." << std::endl;
            
            File file(textFile, "r");
            USE_STATS(countFile(file, 0));
        }
        // LM estimation from a vector of strings in memory
        else {
            std::cout << "Estimating LM from data in memory..." << std::endl;
            
            try {
                char str[MAX_CHARS];
                boost::shared_array<VocabString> vstr = boost::shared_array<VocabString>(new VocabString[MAX_WORDS + 1]);
                unsigned int nbLines = (unsigned int)(ptrXR->getSize() / 100) * pc;
                
                for (unsigned int i = 0; i < nbLines; i++) {
                    std::strcpy(str, ptrXR->getTextLine(i).c_str());
                    
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
                    dcnt->interpolate = true;
                    
                    if (!dcnt->estimate(*ptrNStats, i))
                        throw XenCommon::XenCEption("SRILM says: \"Error in discount estimator for order " + XenCommon::toString(i) + "\". Data too small for ModKN (0)? Try GT (1) or WB (2) as discounting method.");
                    
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
        
        std::cout << "LM estimation done." << std::endl;
    }
    
    return 0;
}

int XenLMsri::loadLM() {
    assert(ptrVocab);
    
    ptrVocab->unkIsWord() = true;
    
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
    
    return 0;
}

int XenLMsri::writeLM() {
    try {
        if (!boost::filesystem::exists(lmFile)) {
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
    
    return 0;
}

std::string XenLMsri::getFileName() const {
    return lmFile;
}

TextStats XenLMsri::getSentenceStats(std::string sent) {
    TextStats tstats;
    
    try {
        char str[MAX_CHARS];
        VocabString vstr[MAX_WORDS + 1];
        
        std::strcpy(str, sent.c_str());
        
        int nw = ptrVocab->parseWords(str, vstr, MAX_WORDS + 1);
        
        if (nw == MAX_WORDS + 1)
            throw XenCommon::XenCEption("Too many words in one hypothesis.");
        
        ptrLM->sentenceProb(vstr, tstats);
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    return tstats;
}

TextStats XenLMsri::getDocumentStats(boost::shared_ptr<Corpus> ptrCorp) {
    TextStats documentStats;
    
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
        documentStats.increment(getSentenceStats(ptrCorp->getLine(i)));
    }
    
    return documentStats;
}

std::string XenLMsri::makeLMname(XenOption* opt) {
    //!< Common name
    std::string ret = opt->getInSData()->getDirName() + "/" + ptrCorp->getXenFile()->getPrefix() + "." + ptrVoc->getXenFile()->getPrefix() + "." + XenCommon::toString(opt->getOrder()) + "g.";
    
    //!< Discounting method
    switch (discount) {
        case 0:
            ret = ret + "kn-int.";
            break;
            
        case 1:
            ret = ret + "gt-int.";
            break;
            
        case 2:
            ret = ret + "wb-int.";
            break;
            
        default:
            ret = ret + "kn-int.";
            break;
    }
    
    //!< Cut-offs
    for (unsigned int i = 1; i < order; i++)
        ret = ret + "0-";
    ret = ret + "0.";
    
    //!< Output format (extension)
    if (opt->getBinLM()) { ret = ret + "sblm"; }
    else { ret = ret + "arpa.gz"; }
    
    return ret;
}
