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
 * $Id: StaticData.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle all data objects used by XenC
 *  Corpus, Vocab, ...
 */

#ifndef STATICDATA_H_
#define STATICDATA_H_

#include <cstdlib>

#include <boost/shared_ptr.hpp>

#include "corpus.h"
#include "XenLMsri.h"
#include "xenvocab.h"
#include "ppl.h"
#include "similarity.h"
#include "wfile.h"

using namespace boost;

class CorpusPair {
    public:
        CorpusPair() {
            ptrInCorp = shared_ptr<Corpus>(new Corpus);
            ptrOutCorp = shared_ptr<Corpus>(new Corpus);
        }
        ~CorpusPair() { }
        shared_ptr<Corpus> getPtrInCorp() { return ptrInCorp; }
        shared_ptr<Corpus> getPtrOutCorp() { return ptrOutCorp; }
    
    private:
        shared_ptr<Corpus> ptrInCorp;
        shared_ptr<Corpus> ptrOutCorp;
};

class LMPair {
    public:
        LMPair() {
            ptrInLM = shared_ptr<XenLMsri>(new XenLMsri);
            ptrOutLM = shared_ptr<XenLMsri>(new XenLMsri);
        }
        ~LMPair() { }
        shared_ptr<XenLMsri> getPtrInLM() { return ptrInLM; }
        shared_ptr<XenLMsri> getPtrOutLM() { return ptrOutLM; }
    
    private:
        shared_ptr<XenLMsri> ptrInLM;
        shared_ptr<XenLMsri> ptrOutLM;
};

class VocabPair {
    public:
        VocabPair() {
            ptrSourceVoc = shared_ptr<XenVocab>(new XenVocab);
            ptrTargetVoc = shared_ptr<XenVocab>(new XenVocab);
        }
        ~VocabPair() { }
        shared_ptr<XenVocab> getPtrSourceVoc() { return ptrSourceVoc; }
        shared_ptr<XenVocab> getPtrTargetVoc() { return ptrTargetVoc; }
    
    private:
        shared_ptr<XenVocab> ptrSourceVoc;
        shared_ptr<XenVocab> ptrTargetVoc;
};

class PPLPair {
    public:
        PPLPair() {
            ptrInPPL = shared_ptr<PPL>(new PPL);
            ptrOutPPL = shared_ptr<PPL>(new PPL);
        }
        ~PPLPair() { }
        shared_ptr<PPL> getPtrInPPL() { return ptrInPPL; }
        shared_ptr<PPL> getPtrOutPPL() { return ptrOutPPL; }
    
    private:
        shared_ptr<PPL> ptrInPPL;
        shared_ptr<PPL> ptrOutPPL;
};

class PhraseTablePair {
    public:
        PhraseTablePair() {
            ptrInPT = shared_ptr<PhraseTable>(new PhraseTable);
            ptrOutPT = shared_ptr<PhraseTable>(new PhraseTable);
        }
        ~PhraseTablePair() { }
        shared_ptr<PhraseTable> getPtrInPT() { return ptrInPT; }
        shared_ptr<PhraseTable> getPtrOutPT() { return ptrOutPT; }
    
    private:
        shared_ptr<PhraseTable> ptrInPT;
        shared_ptr<PhraseTable> ptrOutPT;
};

class MeanLMPair {
    public:
        MeanLMPair() {
            ptrOutLM2 = shared_ptr<XenLMsri>(new XenLMsri);
            ptrOutLM3 = shared_ptr<XenLMsri>(new XenLMsri);
        }
        ~MeanLMPair() { }
        shared_ptr<XenLMsri> getPtrOutLM2() { return ptrOutLM2; }
        shared_ptr<XenLMsri> getPtrOutLM3() { return ptrOutLM3; }
    
    private:
        shared_ptr<XenLMsri> ptrOutLM2;
        shared_ptr<XenLMsri> ptrOutLM3;
    
};

class MeanPPLPair {
    public:
        MeanPPLPair() {
            ptrOutPPL2 = shared_ptr<PPL>(new PPL);
            ptrOutPPL3 = shared_ptr<PPL>(new PPL);
        }
        ~MeanPPLPair() { }
        shared_ptr<PPL> getPtrOutPPL2() { return ptrOutPPL2; }
        shared_ptr<PPL> getPtrOutPPL3() { return ptrOutPPL3; }
    
    private:
        shared_ptr<PPL> ptrOutPPL2;
        shared_ptr<PPL> ptrOutPPL3;
};

class ScoreHolder {
    public:
        ScoreHolder() {
            ptrScores = shared_ptr<Score>(new Score);
            ptrScSimil = shared_ptr<Score>(new Score);
            ptrScXenC = shared_ptr<Score>(new Score);
        }
        ~ScoreHolder() { }
        shared_ptr<Score> getPtrScores() { return ptrScores; }
        shared_ptr<Score> getPtrScSimil() { return ptrScSimil; }
        shared_ptr<Score> getPtrScXenC() { return ptrScXenC; }
    
    private:
        shared_ptr<Score> ptrScores;
        shared_ptr<Score> ptrScSimil;
        shared_ptr<Score> ptrScXenC;
};

class StaticData {
    public:
        static StaticData* getInstance();
        static void deleteInstance();
        // ------------------------
        static shared_ptr<CorpusPair> getSourceCorps();
        static shared_ptr<CorpusPair> getTargetCorps();
        static shared_ptr<LMPair> getSourceLMs();
        static shared_ptr<LMPair> getTargetLMs();
        static shared_ptr<VocabPair> getVocabs();
        static shared_ptr<PPLPair> getSourcePPLs();
        static shared_ptr<PPLPair> getTargetPPLs();
        static shared_ptr<PhraseTablePair> getPTPairs();
        static shared_ptr<MeanLMPair> getMeanSourceLMs();
        static shared_ptr<MeanLMPair> getMeanTargetLMs();
        static shared_ptr<MeanPPLPair> getMeanSourcePPLs();
        static shared_ptr<MeanPPLPair> getMeanTargetPPLs();
        static shared_ptr<CorpusPair> getStemSourceCorps();
        static shared_ptr<CorpusPair> getStemTargetCorps();
        static shared_ptr<LMPair> getStemSourceLMs();
        static shared_ptr<LMPair> getStemTargetLMs();
        static shared_ptr<VocabPair> getStemVocabs();
        static shared_ptr<PPLPair> getStemSourcePPLs();
        static shared_ptr<PPLPair> getStemTargetPPLs();
        static shared_ptr<Similarity> getSim();
        static shared_ptr<ScoreHolder> getScHold();
        static shared_ptr<Wfile> getWeightsFile();
        static shared_ptr<XenResult> getXenResult();
        static shared_ptr<Corpus> getDevCorp();
    
    private:
        StaticData();
        ~StaticData();
        // -----------
        static StaticData* _instance;
        static shared_ptr<CorpusPair> ptrSourceCorp;
        static shared_ptr<CorpusPair> ptrTargetCorp;
        static shared_ptr<LMPair> ptrSourceLM;
        static shared_ptr<LMPair> ptrTargetLM;
        static shared_ptr<VocabPair> ptrVocabs;
        static shared_ptr<PPLPair> ptrSourcePPL;
        static shared_ptr<PPLPair> ptrTargetPPL;
        static shared_ptr<PhraseTablePair> ptrPTPair;
        static shared_ptr<MeanLMPair> ptrMeanSourceLM;
        static shared_ptr<MeanLMPair> ptrMeanTargetLM;
        static shared_ptr<MeanPPLPair> ptrMeanSourcePPL;
        static shared_ptr<MeanPPLPair> ptrMeanTargetPPL;
        static shared_ptr<CorpusPair> ptrStemSourceCorp;
        static shared_ptr<CorpusPair> ptrStemTargetCorp;
        static shared_ptr<LMPair> ptrStemSourceLM;
        static shared_ptr<LMPair> ptrStemTargetLM;
        static shared_ptr<VocabPair> ptrStemVocabs;
        static shared_ptr<PPLPair> ptrStemSourcePPL;
        static shared_ptr<PPLPair> ptrStemTargetPPL;
        static shared_ptr<Similarity> ptrSim;
        static shared_ptr<ScoreHolder> ptrScHold;
        static shared_ptr<Wfile> ptrWeightsFile;
        static shared_ptr<XenResult> ptrXenResult;
        static shared_ptr<Corpus> ptrDevCorp;
};

#endif
