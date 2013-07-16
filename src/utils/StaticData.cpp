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
 * $Id: StaticData.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "StaticData.h"

StaticData* StaticData::_instance = NULL;

shared_ptr<CorpusPair> StaticData::ptrSourceCorp;
shared_ptr<CorpusPair> StaticData::ptrTargetCorp;
shared_ptr<LMPair> StaticData::ptrSourceLM;
shared_ptr<LMPair> StaticData::ptrTargetLM;
shared_ptr<VocabPair> StaticData::ptrVocabs;
shared_ptr<PPLPair> StaticData::ptrSourcePPL;
shared_ptr<PPLPair> StaticData::ptrTargetPPL;
shared_ptr<PhraseTablePair> StaticData::ptrPTPair;
shared_ptr<MeanLMPair> StaticData::ptrMeanSourceLM;
shared_ptr<MeanLMPair> StaticData::ptrMeanTargetLM;
shared_ptr<MeanPPLPair> StaticData::ptrMeanSourcePPL;
shared_ptr<MeanPPLPair> StaticData::ptrMeanTargetPPL;
shared_ptr<CorpusPair> StaticData::ptrStemSourceCorp;
shared_ptr<CorpusPair> StaticData::ptrStemTargetCorp;
shared_ptr<LMPair> StaticData::ptrStemSourceLM;
shared_ptr<LMPair> StaticData::ptrStemTargetLM;
shared_ptr<VocabPair> StaticData::ptrStemVocabs;
shared_ptr<PPLPair> StaticData::ptrStemSourcePPL;
shared_ptr<PPLPair> StaticData::ptrStemTargetPPL;
shared_ptr<Similarity> StaticData::ptrSim;
shared_ptr<ScoreHolder> StaticData::ptrScHold;
shared_ptr<Wfile> StaticData::ptrWeightsFile;
shared_ptr<XenResult> StaticData::ptrXenResult;
shared_ptr<Corpus> StaticData::ptrDevCorp;

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
    StaticData::ptrSourceCorp = shared_ptr<CorpusPair>(new CorpusPair);
    StaticData::ptrTargetCorp = shared_ptr<CorpusPair>(new CorpusPair);
    StaticData::ptrSourceLM = shared_ptr<LMPair>(new LMPair);
    StaticData::ptrTargetLM = shared_ptr<LMPair>(new LMPair);
    StaticData::ptrVocabs = shared_ptr<VocabPair>(new VocabPair);
    StaticData::ptrSourcePPL = shared_ptr<PPLPair>(new PPLPair);
    StaticData::ptrTargetPPL = shared_ptr<PPLPair>(new PPLPair);
    StaticData::ptrPTPair = shared_ptr<PhraseTablePair>(new PhraseTablePair);
    StaticData::ptrMeanSourceLM = shared_ptr<MeanLMPair>(new MeanLMPair);
    StaticData::ptrMeanTargetLM = shared_ptr<MeanLMPair>(new MeanLMPair);
    StaticData::ptrMeanSourcePPL = shared_ptr<MeanPPLPair>(new MeanPPLPair);
    StaticData::ptrMeanTargetPPL = shared_ptr<MeanPPLPair>(new MeanPPLPair);
    StaticData::ptrStemSourceCorp = shared_ptr<CorpusPair>(new CorpusPair);
    StaticData::ptrStemTargetCorp = shared_ptr<CorpusPair>(new CorpusPair);
    StaticData::ptrSourceLM = shared_ptr<LMPair>(new LMPair);
    StaticData::ptrTargetLM = shared_ptr<LMPair>(new LMPair);
    StaticData::ptrStemVocabs = shared_ptr<VocabPair>(new VocabPair);
    StaticData::ptrStemSourcePPL = shared_ptr<PPLPair>(new PPLPair);
    StaticData::ptrStemTargetPPL = shared_ptr<PPLPair>(new PPLPair);
    StaticData::ptrSim = shared_ptr<Similarity>(new Similarity);
    StaticData::ptrScHold = shared_ptr<ScoreHolder>(new ScoreHolder);
    StaticData::ptrWeightsFile = shared_ptr<Wfile>(new Wfile);
    StaticData::ptrXenResult = shared_ptr<XenResult>(new XenResult);
    StaticData::ptrDevCorp = shared_ptr<Corpus>(new Corpus);
}

StaticData::~StaticData() {
    
}

shared_ptr<CorpusPair> StaticData::getSourceCorps() { return ptrSourceCorp; }
shared_ptr<CorpusPair> StaticData::getTargetCorps() { return ptrTargetCorp; }
shared_ptr<LMPair> StaticData::getSourceLMs() { return ptrSourceLM; }
shared_ptr<LMPair> StaticData::getTargetLMs() { return ptrTargetLM; }
shared_ptr<VocabPair> StaticData::getVocabs() { return ptrVocabs; }
shared_ptr<PPLPair> StaticData::getSourcePPLs() { return ptrSourcePPL; }
shared_ptr<PPLPair> StaticData::getTargetPPLs() { return ptrTargetPPL; }
shared_ptr<PhraseTablePair> StaticData::getPTPairs() { return ptrPTPair; }
shared_ptr<MeanLMPair> StaticData::getMeanSourceLMs() { return ptrMeanSourceLM; }
shared_ptr<MeanLMPair> StaticData::getMeanTargetLMs() { return ptrMeanTargetLM; }
shared_ptr<MeanPPLPair> StaticData::getMeanSourcePPLs() { return ptrMeanSourcePPL; }
shared_ptr<MeanPPLPair> StaticData::getMeanTargetPPLs() { return ptrMeanTargetPPL; }
shared_ptr<CorpusPair> StaticData::getStemSourceCorps() { return ptrStemSourceCorp; }
shared_ptr<CorpusPair> StaticData::getStemTargetCorps() { return ptrStemTargetCorp; }
shared_ptr<LMPair> StaticData::getStemSourceLMs() { return ptrStemSourceLM; }
shared_ptr<LMPair> StaticData::getStemTargetLMs() { return ptrStemTargetLM; }
shared_ptr<VocabPair> StaticData::getStemVocabs() { return ptrStemVocabs; }
shared_ptr<PPLPair> StaticData::getStemSourcePPLs() { return ptrStemSourcePPL; }
shared_ptr<PPLPair> StaticData::getStemTargetPPLs() { return ptrStemTargetPPL; }
shared_ptr<Similarity> StaticData::getSim() { return ptrSim; }
shared_ptr<ScoreHolder> StaticData::getScHold() { return ptrScHold; }
shared_ptr<Wfile> StaticData::getWeightsFile() { return ptrWeightsFile; }
shared_ptr<XenResult> StaticData::getXenResult() { return ptrXenResult; }
shared_ptr<Corpus> StaticData::getDevCorp() { return ptrDevCorp; }
