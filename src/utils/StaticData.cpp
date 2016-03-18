/**
 *  @file StaticData.cpp
 *  @brief File handling all data objects used by XenC in a static way
 *  @author Anthony Rousseau
 *  @version 2.0.0
 *  @date 18 March 2016
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013-2016, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 *  Development of the XenC tool has been partially funded by the
 *  European Commission under the MateCat project.
 *
 *  The XenC tool is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License version 3 as
 *  published by the Free Software Foundation
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "../../include/utils/StaticData.h"

StaticData* StaticData::_instance = NULL;

boost::shared_ptr<CorpusPair> StaticData::ptrSourceCorp;
boost::shared_ptr<CorpusPair> StaticData::ptrTargetCorp;
boost::shared_ptr<LMPair> StaticData::ptrSourceLM;
boost::shared_ptr<LMPair> StaticData::ptrTargetLM;
boost::shared_ptr<VocabPair> StaticData::ptrVocabs;
boost::shared_ptr<PPLPair> StaticData::ptrSourcePPL;
boost::shared_ptr<PPLPair> StaticData::ptrTargetPPL;
boost::shared_ptr<PhraseTablePair> StaticData::ptrPTPair;
boost::shared_ptr<MeanLMPair> StaticData::ptrMeanSourceLM;
boost::shared_ptr<MeanLMPair> StaticData::ptrMeanTargetLM;
boost::shared_ptr<MeanPPLPair> StaticData::ptrMeanSourcePPL;
boost::shared_ptr<MeanPPLPair> StaticData::ptrMeanTargetPPL;
boost::shared_ptr<CorpusPair> StaticData::ptrStemSourceCorp;
boost::shared_ptr<CorpusPair> StaticData::ptrStemTargetCorp;
boost::shared_ptr<LMPair> StaticData::ptrStemSourceLM;
boost::shared_ptr<LMPair> StaticData::ptrStemTargetLM;
boost::shared_ptr<VocabPair> StaticData::ptrStemVocabs;
boost::shared_ptr<PPLPair> StaticData::ptrStemSourcePPL;
boost::shared_ptr<PPLPair> StaticData::ptrStemTargetPPL;
boost::shared_ptr<Similarity> StaticData::ptrSim;
boost::shared_ptr<ScoreHolder> StaticData::ptrScHold;
boost::shared_ptr<Wfile> StaticData::ptrWeightsFile;
boost::shared_ptr<XenResult> StaticData::ptrXenResult;
boost::shared_ptr<Corpus> StaticData::ptrDevCorp;

StaticData* StaticData::getInstance() {
    if (_instance == NULL)
        _instance =  new StaticData();
    
    return _instance;
}

void StaticData::deleteInstance() {
    delete _instance;
    _instance = NULL;
}

StaticData::StaticData() {
    StaticData::ptrSourceCorp = boost::make_shared<CorpusPair>();
    StaticData::ptrTargetCorp = boost::make_shared<CorpusPair>();
    StaticData::ptrSourceLM = boost::make_shared<LMPair>();
    StaticData::ptrTargetLM = boost::make_shared<LMPair>();
    StaticData::ptrVocabs = boost::make_shared<VocabPair>();
    StaticData::ptrSourcePPL = boost::make_shared<PPLPair>();
    StaticData::ptrTargetPPL = boost::make_shared<PPLPair>();
    StaticData::ptrPTPair = boost::make_shared<PhraseTablePair>();
    StaticData::ptrMeanSourceLM = boost::make_shared<MeanLMPair>();
    StaticData::ptrMeanTargetLM = boost::make_shared<MeanLMPair>();
    StaticData::ptrMeanSourcePPL = boost::make_shared<MeanPPLPair>();
    StaticData::ptrMeanTargetPPL = boost::make_shared<MeanPPLPair>();
    StaticData::ptrStemSourceCorp = boost::make_shared<CorpusPair>();
    StaticData::ptrStemTargetCorp = boost::make_shared<CorpusPair>();
    StaticData::ptrSourceLM = boost::make_shared<LMPair>();
    StaticData::ptrTargetLM = boost::make_shared<LMPair>();
    StaticData::ptrStemVocabs = boost::make_shared<VocabPair>();
    StaticData::ptrStemSourcePPL = boost::make_shared<PPLPair>();
    StaticData::ptrStemTargetPPL = boost::make_shared<PPLPair>();
    StaticData::ptrSim = boost::make_shared<Similarity>();
    StaticData::ptrScHold = boost::make_shared<ScoreHolder>();
    StaticData::ptrWeightsFile = boost::make_shared<Wfile>();
    StaticData::ptrXenResult = boost::make_shared<XenResult>();
    StaticData::ptrDevCorp = boost::make_shared<Corpus>();
}

StaticData::~StaticData() {
    
}

boost::shared_ptr<CorpusPair> StaticData::getSourceCorps() { return ptrSourceCorp; }
boost::shared_ptr<CorpusPair> StaticData::getTargetCorps() { return ptrTargetCorp; }
boost::shared_ptr<LMPair> StaticData::getSourceLMs() { return ptrSourceLM; }
boost::shared_ptr<LMPair> StaticData::getTargetLMs() { return ptrTargetLM; }
boost::shared_ptr<VocabPair> StaticData::getVocabs() { return ptrVocabs; }
boost::shared_ptr<PPLPair> StaticData::getSourcePPLs() { return ptrSourcePPL; }
boost::shared_ptr<PPLPair> StaticData::getTargetPPLs() { return ptrTargetPPL; }
boost::shared_ptr<PhraseTablePair> StaticData::getPTPairs() { return ptrPTPair; }
boost::shared_ptr<MeanLMPair> StaticData::getMeanSourceLMs() { return ptrMeanSourceLM; }
boost::shared_ptr<MeanLMPair> StaticData::getMeanTargetLMs() { return ptrMeanTargetLM; }
boost::shared_ptr<MeanPPLPair> StaticData::getMeanSourcePPLs() { return ptrMeanSourcePPL; }
boost::shared_ptr<MeanPPLPair> StaticData::getMeanTargetPPLs() { return ptrMeanTargetPPL; }
boost::shared_ptr<CorpusPair> StaticData::getStemSourceCorps() { return ptrStemSourceCorp; }
boost::shared_ptr<CorpusPair> StaticData::getStemTargetCorps() { return ptrStemTargetCorp; }
boost::shared_ptr<LMPair> StaticData::getStemSourceLMs() { return ptrStemSourceLM; }
boost::shared_ptr<LMPair> StaticData::getStemTargetLMs() { return ptrStemTargetLM; }
boost::shared_ptr<VocabPair> StaticData::getStemVocabs() { return ptrStemVocabs; }
boost::shared_ptr<PPLPair> StaticData::getStemSourcePPLs() { return ptrStemSourcePPL; }
boost::shared_ptr<PPLPair> StaticData::getStemTargetPPLs() { return ptrStemTargetPPL; }
boost::shared_ptr<Similarity> StaticData::getSim() { return ptrSim; }
boost::shared_ptr<ScoreHolder> StaticData::getScHold() { return ptrScHold; }
boost::shared_ptr<Wfile> StaticData::getWeightsFile() { return ptrWeightsFile; }
boost::shared_ptr<XenResult> StaticData::getXenResult() { return ptrXenResult; }
boost::shared_ptr<Corpus> StaticData::getDevCorp() { return ptrDevCorp; }
