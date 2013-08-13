/**
 *  @file StaticData.h
 *  @brief File handling all data objects used by XenC in a static way
 *  @author Anthony Rousseau
 *  @version 1.1.0
 *  @date 13 August 2013
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 *  The XenC tool is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef STATICDATA_H_
#define STATICDATA_H_

#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "corpus.h"
#include "XenLMsri.h"
#include "xenvocab.h"
#include "ppl.h"
#include "similarity.h"
#include "wfile.h"

using namespace boost;

/**
 *  @class CorpusPair
 *  @brief Tiny class holding two related Corpus
 */
class CorpusPair {
public:
    /**
     *  @fn CorpusPair ()
     *  @brief Default constructor
     */
    CorpusPair() {
        ptrInCorp = boost::make_shared<Corpus>();
        ptrOutCorp = boost::make_shared<Corpus>();
    }
    
    /**
     *  @fn ~CorpusPair ()
     *  @brief Default destructor
     */
    ~CorpusPair() {
        
    }
    
    /**
     *  @fn boost::shared_ptr<Corpus> getPtrInCorp () const
     *  @brief Accessor to the in-domain corpus
     *
     *  @return the in-domain corpus
     */
    boost::shared_ptr<Corpus> getPtrInCorp() const { return ptrInCorp; }
    
    /**
     *  @fn boost::shared_ptr<Corpus> getPtrOutCorp () const
     *  @brief Accessor to the out-of-domain corpus
     *
     *  @return the out-of-domain corpus
     */
    boost::shared_ptr<Corpus> getPtrOutCorp() const { return ptrOutCorp; }
    
private:
    boost::shared_ptr<Corpus> ptrInCorp;    //!< Shared pointer to the in-domain Corpus
    boost::shared_ptr<Corpus> ptrOutCorp;   //!< Shared pointer to the out-of-domain Corpus
};

/**
 *  @class LMPair
 *  @brief Tiny class holding two related language models
 */
class LMPair {
public:
    /**
     *  @fn LMPair ()
     *  @brief Default constructor
     */
    LMPair() {
        ptrInLM = boost::make_shared<XenLMsri>();
        ptrOutLM = boost::make_shared<XenLMsri>();
    }
    
    /**
     *  @fn ~LMPair ()
     *  @brief Default destructor
     */
    ~LMPair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<XenLMsri> getPtrInLM () const
     *  @brief Accessor to the in-domain language model
     *
     *  @return the in-domain language model
     */
    boost::shared_ptr<XenLMsri> getPtrInLM() const { return ptrInLM; }
    
    /**
     *  @fn boost::shared_ptr<XenLMsri> getPtrOutLM () const
     *  @brief Accessor to the out-of-domain language model
     *
     *  @return the out-of-domain language model
     */
    boost::shared_ptr<XenLMsri> getPtrOutLM() const { return ptrOutLM; }
    
private:
    boost::shared_ptr<XenLMsri> ptrInLM;       //!< Shared pointer to the in-domain language model
    boost::shared_ptr<XenLMsri> ptrOutLM;      //!< Shared pointer to the out-of-domain language model
};

/**
 *  @class VocabPair
 *  @brief Tiny class holding the two vocabularies
 */
class VocabPair {
public:
    /**
     *  @fn VocabPair ()
     *  @brief Default constructor
     */
    VocabPair() {
        ptrSourceVoc = boost::make_shared<XenVocab>();
        ptrTargetVoc = boost::make_shared<XenVocab>();
    }
    
    /**
     *  @fn ~VocabPair ()
     *  @brief Default destructor
     */
    ~VocabPair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<XenVocab> getPtrSourceVoc () const
     *  @brief Accessor to the source vocabulary
     *
     *  @return the source vocabulary
     */
    boost::shared_ptr<XenVocab> getPtrSourceVoc() const { return ptrSourceVoc; }
    
    /**
     *  @fn boost::shared_ptr<XenVocab> getPtrTargetVoc () const
     *  @brief Accessor to the target vocabulary
     *
     *  @return the target vocabulary
     */
    boost::shared_ptr<XenVocab> getPtrTargetVoc() const { return ptrTargetVoc; }
    
private:
    boost::shared_ptr<XenVocab> ptrSourceVoc;       //!< Shared pointer to the source vocabulary
    boost::shared_ptr<XenVocab> ptrTargetVoc;       //!< Shared pointer to the target vocabulary
};

/**
 *  @class PPLPair
 *  @brief Tiny class holding two related PPL objects
 */
class PPLPair {
public:
    /**
     *  @fn PPLPair ()
     *  @brief Default constructor
     */
    PPLPair() {
        ptrInPPL = boost::make_shared<PPL>();
        ptrOutPPL = boost::make_shared<PPL>();
    }
    
    /**
     *  @fn ~PPLPair ()
     *  @brief Default destructor
     */
    ~PPLPair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<PPL> getPtrInPPL () const
     *  @brief Accessor to the in-domain PPL object
     *
     *  @return the in-domain PPL object
     */
    boost::shared_ptr<PPL> getPtrInPPL() const { return ptrInPPL; }
    
    /**
     *  @fn boost::shared_ptr<PPL> getPtrOutPPL () const
     *  @brief Accessor to the out-of-domain PPL object
     *
     *  @return the out-of-domain PPL object
     */
    boost::shared_ptr<PPL> getPtrOutPPL() const { return ptrOutPPL; }
    
private:
    boost::shared_ptr<PPL> ptrInPPL;       //!< Shared pointer to the in-domain PPL object
    boost::shared_ptr<PPL> ptrOutPPL;      //!< Shared pointer to the out-of-domain PPL object
};

/**
 *  @class PhraseTablePair
 *  @brief Tiny class holding the two phrase-tables
 */
class PhraseTablePair {
public:
    /**
     *  @fn PhraseTablePair ()
     *  @brief Default constructor
     */
    PhraseTablePair() {
        ptrInPT = boost::make_shared<PhraseTable>();
        ptrOutPT = boost::make_shared<PhraseTable>();
    }
    
    /**
     *  @fn ~PhraseTablePair ()
     *  @brief Default destructor
     */
    ~PhraseTablePair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<PhraseTable> getPtrInPT () const
     *  @brief Accessor to the in-domain phrase-table
     *
     *  @return the in-domain phrase-table
     */
    boost::shared_ptr<PhraseTable> getPtrInPT() const { return ptrInPT; }
    
    /**
     *  @fn boost::shared_ptr<PhraseTable> getPtrOutPT () const
     *  @brief Accessor to the out-of-domain phrase-table
     *
     *  @return the out-of-domain phrase-table
     */
    boost::shared_ptr<PhraseTable> getPtrOutPT() const { return ptrOutPT; }
    
private:
    boost::shared_ptr<PhraseTable> ptrInPT;        //!< Shared pointer to the in-domain phrase-table
    boost::shared_ptr<PhraseTable> ptrOutPT;       //!< Shared pointer to the out-of-domain phrase-table
};

/**
 *  @class MeanLMPair
 *  @brief Tiny class holding two additional LMs for mean scoring feature
 */
class MeanLMPair {
public:
    /**
     *  @fn MeanLMPair ()
     *  @brief Default constructor
     */
    MeanLMPair() {
        ptrOutLM2 = boost::make_shared<XenLMsri>();
        ptrOutLM3 = boost::make_shared<XenLMsri>();
    }
    
    /**
     *  @fn ~MeanLMPair ()
     *  @brief Default Destructor
     */
    ~MeanLMPair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<XenLMsri> getPtrOutLM2 () const
     *  @brief Accessor to the second out-of-domain language model
     *
     *  @return the second out-of-domain language model
     */
    boost::shared_ptr<XenLMsri> getPtrOutLM2() const { return ptrOutLM2; }
    
    /**
     *  @fn boost::shared_ptr<XenLMsri> getPtrOutLM3 () const
     *  @brief Accessor to the third out-of-domain language model
     *
     *  @return the third out-of-domain language model
     */
    boost::shared_ptr<XenLMsri> getPtrOutLM3() const { return ptrOutLM3; }
    
private:
    boost::shared_ptr<XenLMsri> ptrOutLM2;      //!< Shared pointer to the second out-of-domain language model
    boost::shared_ptr<XenLMsri> ptrOutLM3;      //!< Shared pointer to the third out-of-domain language model
    
};

/**
 *  @class MeanPPLPair
 *  @brief Tiny class holding two additional PPL objects for mean scoring feature
 */
class MeanPPLPair {
public:
    /**
     *  @fn MeanPPLPair ()
     *  @brief Default constructor
     */
    MeanPPLPair() {
        ptrOutPPL2 = boost::make_shared<PPL>();
        ptrOutPPL3 = boost::make_shared<PPL>();
    }
    
    /**
     *  @fn ~MeanPPLPair ()
     *  @brief Default Destructor
     */
    ~MeanPPLPair() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<PPL> getPtrOutPPL2 () const
     *  @brief Accessor to the second out-of-domain PPL object
     *
     *  @return the second out-of-domain PPL object
     */
    boost::shared_ptr<PPL> getPtrOutPPL2() const { return ptrOutPPL2; }
    
    /**
     *  @fn boost::shared_ptr<PPL> getPtrOutPPL3 () const
     *  @brief Accessor to the third out-of-domain PPL object
     *
     *  @return the third out-of-domain PPL object
     */
    boost::shared_ptr<PPL> getPtrOutPPL3() const { return ptrOutPPL3; }
    
private:
    boost::shared_ptr<PPL> ptrOutPPL2;      //!< Shared pointer to the second out-of-domain PPL object
    boost::shared_ptr<PPL> ptrOutPPL3;      //!< Shared pointer to the third out-of-domain PPL object
};

/**
 *  @class ScoreHolder
 *  @brief Tiny class holding three Score objects (global scores, similarity, cross-entropy)
 */
class ScoreHolder {
public:
    /**
     *  @fn ScoreHolder ()
     *  @brief Default constructor
     */
    ScoreHolder() {
        ptrScores = boost::make_shared<Score>();
        ptrScSimil = boost::make_shared<Score>();
        ptrScXenC = boost::make_shared<Score>();
    }
    
    /**
     *  @fn ~ScoreHolder ()
     *  @brief Default Destructor
     */
    ~ScoreHolder() {
    
    }
    
    /**
     *  @fn boost::shared_ptr<Score> getPtrScores () const
     *  @brief Accessor to the global Score object
     *
     *  @return the global Score object
     */
    boost::shared_ptr<Score> getPtrScores() const { return ptrScores; }
    
    /**
     *  @fn boost::shared_ptr<Score> getPtrScSimil () const
     *  @brief Accessor to the similarity measures Score object
     *
     *  @return the similarity measures Score object
     */
    boost::shared_ptr<Score> getPtrScSimil() const { return ptrScSimil; }
    
    /**
     *  @fn boost::shared_ptr<Score> getPtrScXenC () const
     *  @brief Accessor to the cross-entropy Score object
     *
     *  @return the cross-entropy Score object
     */
    boost::shared_ptr<Score> getPtrScXenC() const { return ptrScXenC; }
    
private:
    boost::shared_ptr<Score> ptrScores;         //!< Shared pointer to the global Score object
    boost::shared_ptr<Score> ptrScSimil;        //!< Shared pointer to the similarity measures Score object
    boost::shared_ptr<Score> ptrScXenC;         //!< Shared pointer to the cross-entropy Score object
};

/**
 *  @class StaticData
 *  @brief Class gathering all data used and generated by XenC
 */
class StaticData {
public:
    /**
     *  @fn static StaticData* getInstance ()
     *  @brief Accessor to the instance of the singleton StaticData object
     *
     *  @return the StaticData unique instance
     */
    static StaticData* getInstance();
    
    /**
     *  @fn static void deleteInstance ()
     *  @brief Deletes the unique instance of the StaticData singleton
     */
    static void deleteInstance();
    
    /**
     *  @fn static boost::shared_ptr<CorpusPair> getSourceCorps ()
     *  @brief Accessor to the source language Corpus Pair
     *
     *  @return the source language Corpus Pair
     */
    static boost::shared_ptr<CorpusPair> getSourceCorps();
    
    /**
     *  @fn static boost::shared_ptr<CorpusPair> getTargetCorps ()
     *  @brief Accessor to the target language Corpus
     *
     *  @return the target language Corpus Pair
     */
    static boost::shared_ptr<CorpusPair> getTargetCorps();
    
    /**
     *  @fn static boost::shared_ptr<LMPair> getSourceLMs ()
     *  @brief Accessor to the source language models
     *
     *  @return the source language models
     */
    static boost::shared_ptr<LMPair> getSourceLMs();
    
    /**
     *  @fn static boost::shared_ptr<LMPair> getTargetLMs ()
     *  @brief Accessor to the target language models
     *
     *  @return the target language models
     */
    static boost::shared_ptr<LMPair> getTargetLMs();
    
    /**
     *  @fn static boost::shared_ptr<VocabPair> getVocabs ()
     *  @brief Accessor to the vocabularies
     *
     *  @return the vocabularies
     */
    static boost::shared_ptr<VocabPair> getVocabs();
    
    /**
     *  @fn static boost::shared_ptr<PPLPair> getSourcePPLs ()
     *  @brief Accessor to the source language PPL objects
     *
     *  @return the source language PPL objects
     */
    static boost::shared_ptr<PPLPair> getSourcePPLs();
    
    /**
     *  @fn static boost::shared_ptr<PPLPair> getTargetPPLs ()
     *  @brief Accessor to the target language PPL objects
     *
     *  @return the target language PPL objects
     */
    static boost::shared_ptr<PPLPair> getTargetPPLs();
    
    /**
     *  @fn static boost::shared_ptr<PhraseTablePair> getPTPairs ()
     *  @brief Accessor to the phrase-tables
     *
     *  @return the phrase-tables
     */
    static boost::shared_ptr<PhraseTablePair> getPTPairs();
    
    /**
     *  @fn static boost::shared_ptr<MeanLMPair> getMeanSourceLMs ()
     *  @brief Accessor to the mean source language models
     *
     *  @return the mean source language models
     */
    static boost::shared_ptr<MeanLMPair> getMeanSourceLMs();
    
    /**
     *  @fn static boost::shared_ptr<MeanLMPair> getMeanTargetLMs ()
     *  @brief Accessor to the mean target language models
     *
     *  @return the mean target language models
     */
    static boost::shared_ptr<MeanLMPair> getMeanTargetLMs();
    
    /**
     *  @fn static boost::shared_ptr<MeanPPLPair> getMeanSourcePPLs ()
     *  @brief Accessor to the mean source PPL objects
     *
     *  @return the mean source PPL objects
     */
    static boost::shared_ptr<MeanPPLPair> getMeanSourcePPLs();
    
    /**
     *  @fn static boost::shared_ptr<MeanPPLPair> getMeanTargetPPLs ()
     *  @brief Accessor to the mean target PPL objects
     *
     *  @return the mean target PPL objects
     */
    static boost::shared_ptr<MeanPPLPair> getMeanTargetPPLs();
    
    /**
     *  @fn static boost::shared_ptr<CorpusPair> getStemSourceCorps ()
     *  @brief Accessor to the source language stem Corpus Pair
     *
     *  @return the source language stem Corpus Pair
     */
    static boost::shared_ptr<CorpusPair> getStemSourceCorps();
    
    /**
     *  @fn static boost::shared_ptr<CorpusPair> getStemTargetCorps ()
     *  @brief Accessor to the target language stem Corpus Pair
     *
     *  @return the target language stem Corpus Pair
     */
    static boost::shared_ptr<CorpusPair> getStemTargetCorps();
    
    /**
     *  @fn static boost::shared_ptr<LMPair> getStemSourceLMs ()
     *  @brief Accessor to the source language stem language models
     *
     *  @return the source language stem language models
     */
    static boost::shared_ptr<LMPair> getStemSourceLMs();
    
    /**
     *  @fn static boost::shared_ptr<LMPair> getStemTargetLMs ()
     *  @brief Accessor to the target language stem language models
     *
     *  @return the target language stem language models
     */
    static boost::shared_ptr<LMPair> getStemTargetLMs();
    
    /**
     *  @fn static boost::shared_ptr<VocabPair> getStemVocabs ()
     *  @brief Accessor to the stem vocabularies
     *
     *  @return the stem vocabularies
     */
    static boost::shared_ptr<VocabPair> getStemVocabs();
    
    /**
     *  @fn static boost::shared_ptr<PPLPair> getStemSourcePPLs ()
     *  @brief Accessor to the source language stem PPL objects
     *
     *  @return the source language stem PPL objects
     */
    static boost::shared_ptr<PPLPair> getStemSourcePPLs();
    
    /**
     *  @fn static boost::shared_ptr<PPLPair> getStemTargetPPLs ()
     *  @brief Accessor to the target language stem PPL objects
     *
     *  @return the target language stem PPL objects
     */
    static boost::shared_ptr<PPLPair> getStemTargetPPLs();
    
    /**
     *  @fn static boost::shared_ptr<Similarity> getSim ()
     *  @brief Accessor to the Similarity measures object
     *
     *  @return the Similarity measures object
     */
    static boost::shared_ptr<Similarity> getSim();
    
    /**
     *  @fn static boost::shared_ptr<ScoreHolder> getScHold ()
     *  @brief Accessor to the ScoreHolder object
     *
     *  @return the ScoreHolder object
     */
    static boost::shared_ptr<ScoreHolder> getScHold();
    
    /**
     *  @fn static boost::shared_ptr<Wfile> getWeightsFile ()
     *  @brief Accessor to the weights file
     *
     *  @return the weights file
     */
    static boost::shared_ptr<Wfile> getWeightsFile();
    
    /**
     *  @fn static boost::shared_ptr<XenResult> getXenResult ()
     *  @brief Accessor to the filtering result file
     *
     *  @return the filtering result file
     */
    static boost::shared_ptr<XenResult> getXenResult();
    
    /**
     *  @fn static boost::shared_ptr<Corpus> getDevCorp ()
     *  @brief Accessor to the development Corpus
     *
     *  @return the development Corpus
     */
    static boost::shared_ptr<Corpus> getDevCorp();
    
private:
    /**
     *  @fn StaticData ()
     *  @brief Singleton private default constructor
     */
    StaticData();
    
    /**
     *  @fn ~StaticData ()
     *  @brief Singleton private default destructor
     */
    ~StaticData();

    static StaticData* _instance;                               //!< The Singleton unique instance
    static boost::shared_ptr<CorpusPair> ptrSourceCorp;         //!< Shared pointer on the source language Corpus
    static boost::shared_ptr<CorpusPair> ptrTargetCorp;         //!< Shared pointer on the target language Corpus
    static boost::shared_ptr<LMPair> ptrSourceLM;               //!< Shared pointer on the source language models
    static boost::shared_ptr<LMPair> ptrTargetLM;               //!< Shared pointer on the target language models
    static boost::shared_ptr<VocabPair> ptrVocabs;              //!< Shared pointer on the vocabularies
    static boost::shared_ptr<PPLPair> ptrSourcePPL;             //!< Shared pointer on the source language PPL objects
    static boost::shared_ptr<PPLPair> ptrTargetPPL;             //!< Shared pointer on the target language PPL objects
    static boost::shared_ptr<PhraseTablePair> ptrPTPair;        //!< Shared pointer on the phrase-tables
    static boost::shared_ptr<MeanLMPair> ptrMeanSourceLM;       //!< Shared pointer on the mean source language models
    static boost::shared_ptr<MeanLMPair> ptrMeanTargetLM;       //!< Shared pointer on the mean target language models
    static boost::shared_ptr<MeanPPLPair> ptrMeanSourcePPL;     //!< Shared pointer on the mean source PPL objects
    static boost::shared_ptr<MeanPPLPair> ptrMeanTargetPPL;     //!< Shared pointer on the mean target PPL objects
    static boost::shared_ptr<CorpusPair> ptrStemSourceCorp;     //!< Shared pointer on the source language stem Corpus Pair
    static boost::shared_ptr<CorpusPair> ptrStemTargetCorp;     //!< Shared pointer on the target language stem Corpus Pair
    static boost::shared_ptr<LMPair> ptrStemSourceLM;           //!< Shared pointer on the source language stem language models
    static boost::shared_ptr<LMPair> ptrStemTargetLM;           //!< Shared pointer on the target language stem language models
    static boost::shared_ptr<VocabPair> ptrStemVocabs;          //!< Shared pointer on the stem vocabularies
    static boost::shared_ptr<PPLPair> ptrStemSourcePPL;         //!< Shared pointer on the source language stem PPL objects
    static boost::shared_ptr<PPLPair> ptrStemTargetPPL;         //!< Shared pointer on the target language stem PPL objects
    static boost::shared_ptr<Similarity> ptrSim;                //!< Shared pointer on the Similarity measures object
    static boost::shared_ptr<ScoreHolder> ptrScHold;            //!< Shared pointer on the ScoreHolder object
    static boost::shared_ptr<Wfile> ptrWeightsFile;             //!< Shared pointer on the weights file
    static boost::shared_ptr<XenResult> ptrXenResult;           //!< Shared pointer on the filtering result file
    static boost::shared_ptr<Corpus> ptrDevCorp;                //!< Shared pointer on the development Corpus
};

#endif
