/**
 *  @file biXEntropy.cpp
 *  @brief Derived class to handle filtering mode 3: bilingual cross-entropy
 *  @author Anthony Rousseau
 *  @version 1.0.0
 *  @date 27 July 2013
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

#include "biXEntropy.h"

BiXEntropy::BiXEntropy() {

}

BiXEntropy::~BiXEntropy() {

}

int BiXEntropy::launch() {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    // Init corpus
    sD->getSourceCorps()->getPtrInCorp()->initialize(opt->getInSData(), opt->getSLang());
    sD->getSourceCorps()->getPtrOutCorp()->initialize(opt->getOutSData(), opt->getSLang());
    sD->getTargetCorps()->getPtrInCorp()->initialize(opt->getInTData(), opt->getTLang());
    sD->getTargetCorps()->getPtrOutCorp()->initialize(opt->getOutTData(), opt->getTLang());

    // Init vocabs
    if (opt->getSVocab()->getFileName().compare("") == 0) { sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp()); }
	else { sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab()); }
    if (opt->getTVocab()->getFileName().compare("") == 0) { sD->getVocabs()->getPtrTargetVoc()->initialize(sD->getTargetCorps()->getPtrInCorp()); }
	else { sD->getVocabs()->getPtrTargetVoc()->initialize(opt->getTVocab()); }
    
    // Init in-domain source LM
    if (opt->getInSLM()->getFileName().compare("") == 0) {
        sD->getSourceLMs()->getPtrInLM()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrInLM()->createLM();
        sD->getSourceLMs()->getPtrInLM()->writeLM();
    }
    else {
        sD->getSourceLMs()->getPtrInLM()->initialize(opt->getInSLM(), sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrInLM()->loadLM();
    }
    
    // Init in-domain target LM
    if (opt->getInTLM()->getFileName().compare("") == 0) {
        sD->getTargetLMs()->getPtrInLM()->initialize(sD->getTargetCorps()->getPtrInCorp(), sD->getVocabs()->getPtrTargetVoc());
        sD->getTargetLMs()->getPtrInLM()->createLM();
        sD->getTargetLMs()->getPtrInLM()->writeLM();
    }
    else {
        sD->getTargetLMs()->getPtrInLM()->initialize(opt->getInTLM(), sD->getVocabs()->getPtrTargetVoc());
        sD->getTargetLMs()->getPtrInLM()->loadLM();
    }
    
    // Init out-of-domain source LM
    if (opt->getOutSLM()->getFileName().compare("") == 0) {
        opt->setSampleSize(Mode::findSampleSize(sD->getSourceCorps()->getPtrInCorp(), sD->getSourceCorps()->getPtrOutCorp()));
        
        boost::shared_ptr<Corpus> ptrOutLMCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getSourceLMs()->getPtrOutLM()->initialize(ptrOutLMCorp, sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrOutLM()->createLM();
        sD->getSourceLMs()->getPtrOutLM()->writeLM();
    }
    else {
        sD->getSourceLMs()->getPtrOutLM()->initialize(opt->getOutSLM(), sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrOutLM()->loadLM();
    }
    
    // Init out-of-domain target LM
    if (opt->getOutTLM()->getFileName().compare("") == 0) {
        opt->setSampleSize(Mode::findSampleSize(sD->getTargetCorps()->getPtrInCorp(), sD->getTargetCorps()->getPtrOutCorp()));
        
        boost::shared_ptr<Corpus> ptrOutLMCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getTargetCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getTargetLMs()->getPtrOutLM()->initialize(ptrOutLMCorp, sD->getVocabs()->getPtrTargetVoc());
        sD->getTargetLMs()->getPtrOutLM()->createLM();
        sD->getTargetLMs()->getPtrOutLM()->writeLM();
    }
    else {
        sD->getTargetLMs()->getPtrOutLM()->initialize(opt->getOutSLM(), sD->getVocabs()->getPtrTargetVoc());
        sD->getTargetLMs()->getPtrOutLM()->loadLM();
    }
    
    // Check for LM estimation OK
    if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) { std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl; return 1; }
    if (!boost::filesystem::exists(sD->getTargetLMs()->getPtrInLM()->getFileName())) { std::cout << "Error: LM file " + sD->getTargetLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl; return 1; }
    if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrOutLM()->getFileName())) { std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl; return 1; }
    if (!boost::filesystem::exists(sD->getTargetLMs()->getPtrOutLM()->getFileName())) { std::cout << "Error: LM file " + sD->getTargetLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl; return 1; }

    // Init Mean LMs if needed
    if (opt->getMean()) {
        boost::shared_ptr<Corpus> ptrSourceOutLMCorp2 = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getMeanSourceLMs()->getPtrOutLM2()->initialize(ptrSourceOutLMCorp2, sD->getVocabs()->getPtrSourceVoc());
        sD->getMeanSourceLMs()->getPtrOutLM2()->createLM();
        sD->getMeanSourceLMs()->getPtrOutLM2()->writeLM();
        
        boost::shared_ptr<Corpus> ptrTargetOutLMCorp2 = boost::make_shared<Corpus>(Mode::extractSample(sD->getTargetCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getMeanTargetLMs()->getPtrOutLM2()->initialize(ptrTargetOutLMCorp2, sD->getVocabs()->getPtrTargetVoc());
        sD->getMeanTargetLMs()->getPtrOutLM2()->createLM();
        sD->getMeanTargetLMs()->getPtrOutLM2()->writeLM();
        
        boost::shared_ptr<Corpus> ptrSourceOutLMCorp3 = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getMeanSourceLMs()->getPtrOutLM3()->initialize(ptrSourceOutLMCorp3, sD->getVocabs()->getPtrSourceVoc());
        sD->getMeanSourceLMs()->getPtrOutLM3()->createLM();
        sD->getMeanSourceLMs()->getPtrOutLM3()->writeLM();
        
        boost::shared_ptr<Corpus> ptrTargetOutLMCorp3 = boost::make_shared<Corpus>(Mode::extractSample(sD->getTargetCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
        sD->getMeanTargetLMs()->getPtrOutLM3()->initialize(ptrTargetOutLMCorp3, sD->getVocabs()->getPtrTargetVoc());
        sD->getMeanTargetLMs()->getPtrOutLM3()->createLM();
        sD->getMeanTargetLMs()->getPtrOutLM3()->writeLM();
        
        if (!boost::filesystem::exists(sD->getMeanSourceLMs()->getPtrOutLM2()->getFileName())) { std::cout << "Error: LM file " + sD->getMeanSourceLMs()->getPtrOutLM2()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getMeanTargetLMs()->getPtrOutLM2()->getFileName())) { std::cout << "Error: LM file " + sD->getMeanTargetLMs()->getPtrOutLM2()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getMeanSourceLMs()->getPtrOutLM3()->getFileName())) { std::cout << "Error: LM file " + sD->getMeanSourceLMs()->getPtrOutLM3()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getMeanTargetLMs()->getPtrOutLM3()->getFileName())) { std::cout << "Error: LM file " + sD->getMeanTargetLMs()->getPtrOutLM3()->getFileName() + " does not exists!" << std::endl; return 1; }
    }

    // Init all Stem data if needed
    if (opt->getStem()) {
        sD->getStemSourceCorps()->getPtrInCorp()->initialize(opt->getInSStem(), opt->getSLang());
        sD->getStemTargetCorps()->getPtrInCorp()->initialize(opt->getInTStem(), opt->getTLang());
        sD->getStemSourceCorps()->getPtrOutCorp()->initialize(opt->getOutSStem(), opt->getSLang());
        sD->getStemTargetCorps()->getPtrOutCorp()->initialize(opt->getOutTStem(), opt->getTLang());
        
        sD->getStemVocabs()->getPtrSourceVoc()->initialize(sD->getStemSourceCorps()->getPtrInCorp());
        sD->getStemVocabs()->getPtrTargetVoc()->initialize(sD->getStemTargetCorps()->getPtrInCorp());

        sD->getStemSourceLMs()->getPtrInLM()->initialize(sD->getStemSourceCorps()->getPtrInCorp(), sD->getStemVocabs()->getPtrSourceVoc());
        sD->getStemSourceLMs()->getPtrInLM()->createLM();
        sD->getStemSourceLMs()->getPtrInLM()->writeLM();
        
        sD->getStemTargetLMs()->getPtrInLM()->initialize(sD->getStemTargetCorps()->getPtrInCorp(), sD->getStemVocabs()->getPtrTargetVoc());
        sD->getStemTargetLMs()->getPtrInLM()->createLM();
        sD->getStemTargetLMs()->getPtrInLM()->writeLM();
        
        opt->setSampleSize(Mode::findSampleSize(sD->getStemSourceCorps()->getPtrInCorp(), sD->getStemSourceCorps()->getPtrOutCorp()));
        boost::shared_ptr<Corpus> ptrOutSourceStemCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getStemSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), false));
        sD->getStemSourceLMs()->getPtrOutLM()->initialize(ptrOutSourceStemCorp, sD->getStemVocabs()->getPtrSourceVoc());
        sD->getStemSourceLMs()->getPtrOutLM()->createLM();
        sD->getStemSourceLMs()->getPtrOutLM()->writeLM();
        
        opt->setSampleSize(Mode::findSampleSize(sD->getStemTargetCorps()->getPtrInCorp(), sD->getStemTargetCorps()->getPtrOutCorp()));
        boost::shared_ptr<Corpus> ptrOutTargetStemCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getStemTargetCorps()->getPtrOutCorp(), opt->getSampleSize(), false));
        sD->getStemTargetLMs()->getPtrOutLM()->initialize(ptrOutTargetStemCorp, sD->getStemVocabs()->getPtrTargetVoc());
        sD->getStemTargetLMs()->getPtrOutLM()->createLM();
        sD->getStemTargetLMs()->getPtrOutLM()->writeLM();
        
        if (!boost::filesystem::exists(sD->getStemSourceLMs()->getPtrInLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getStemTargetLMs()->getPtrInLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemTargetLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getStemSourceLMs()->getPtrOutLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl; return 1; }
        if (!boost::filesystem::exists(sD->getStemTargetLMs()->getPtrOutLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemTargetLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl; return 1; }
    }
    
    // Init all PPL objects
    sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrInLM());
    sD->getSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
    sD->getSourcePPLs()->getPtrOutPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrOutLM());
    sD->getSourcePPLs()->getPtrOutPPL()->calcPPLCorpus();
    sD->getTargetPPLs()->getPtrInPPL()->initialize(sD->getTargetCorps()->getPtrOutCorp(), sD->getTargetLMs()->getPtrInLM());
    sD->getTargetPPLs()->getPtrInPPL()->calcPPLCorpus();
    sD->getTargetPPLs()->getPtrOutPPL()->initialize(sD->getTargetCorps()->getPtrOutCorp(), sD->getTargetLMs()->getPtrOutLM());
    sD->getTargetPPLs()->getPtrOutPPL()->calcPPLCorpus();
    
    // Same for Mean if needed
    if (opt->getMean()) {
        sD->getMeanSourcePPLs()->getPtrOutPPL2()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getMeanSourceLMs()->getPtrOutLM2());
        sD->getMeanSourcePPLs()->getPtrOutPPL2()->calcPPLCorpus();
        sD->getMeanSourcePPLs()->getPtrOutPPL3()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getMeanSourceLMs()->getPtrOutLM3());
        sD->getMeanSourcePPLs()->getPtrOutPPL3()->calcPPLCorpus();
        sD->getMeanTargetPPLs()->getPtrOutPPL2()->initialize(sD->getTargetCorps()->getPtrOutCorp(), sD->getMeanTargetLMs()->getPtrOutLM2());
        sD->getMeanTargetPPLs()->getPtrOutPPL2()->calcPPLCorpus();
        sD->getMeanTargetPPLs()->getPtrOutPPL3()->initialize(sD->getTargetCorps()->getPtrOutCorp(), sD->getMeanTargetLMs()->getPtrOutLM3());
        sD->getMeanTargetPPLs()->getPtrOutPPL3()->calcPPLCorpus();
    }
    
    // Same for Stem if needed
    if (opt->getStem()) {
        sD->getStemSourcePPLs()->getPtrInPPL()->initialize(sD->getStemSourceCorps()->getPtrOutCorp(), sD->getStemSourceLMs()->getPtrInLM());
        sD->getStemSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
        sD->getStemSourcePPLs()->getPtrOutPPL()->initialize(sD->getStemSourceCorps()->getPtrOutCorp(), sD->getStemSourceLMs()->getPtrOutLM());
        sD->getStemSourcePPLs()->getPtrOutPPL()->calcPPLCorpus();
        sD->getStemTargetPPLs()->getPtrInPPL()->initialize(sD->getStemTargetCorps()->getPtrOutCorp(), sD->getStemTargetLMs()->getPtrInLM());
        sD->getStemTargetPPLs()->getPtrInPPL()->calcPPLCorpus();
        sD->getStemTargetPPLs()->getPtrOutPPL()->initialize(sD->getStemTargetCorps()->getPtrOutCorp(), sD->getStemTargetLMs()->getPtrOutLM());
        sD->getStemTargetPPLs()->getPtrOutPPL()->calcPPLCorpus();
    }
    
    // Init weight file if needed
    if (opt->getWFile()->getFileName().compare("") != 0)
        sD->getWeightsFile()->initialize(opt->getWFile());
    
    // Scores computation
	for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
		double resS = 0;
		double resT = 0;
        
        if (opt->getMean()) {
            resS = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - ((sD->getSourcePPLs()->getPtrOutPPL()->getXE(i) + sD->getMeanSourcePPLs()->getPtrOutPPL2()->getXE(i) + sD->getMeanSourcePPLs()->getPtrOutPPL3()->getXE(i)) / 3));
            resT = (sD->getTargetPPLs()->getPtrInPPL()->getXE(i) - ((sD->getTargetPPLs()->getPtrOutPPL()->getXE(i) + sD->getMeanTargetPPLs()->getPtrOutPPL2()->getXE(i) + sD->getMeanTargetPPLs()->getPtrOutPPL3()->getXE(i)) / 3));
        }
        else {
            resS = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getSourcePPLs()->getPtrOutPPL()->getXE(i));
            resT = (sD->getTargetPPLs()->getPtrInPPL()->getXE(i) - sD->getTargetPPLs()->getPtrOutPPL()->getXE(i));
        }
        
        if (opt->getStem()) {
            resS += (sD->getStemSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getStemSourcePPLs()->getPtrOutPPL()->getXE(i));
            resT += (sD->getStemTargetPPLs()->getPtrInPPL()->getXE(i) - sD->getStemTargetPPLs()->getPtrOutPPL()->getXE(i));
        }
		
        double res = resS + resT;
        
        if (opt->getWFile()->getFileName().compare("") != 0)
            res = res * sD->getWeightsFile()->getWeight(i);
        
		sD->getScHold()->getPtrScores()->addScore(res);
	}
    
    // Calibration
	sD->getScHold()->getPtrScores()->calibrate();
    
    if (opt->getInv()) { sD->getScHold()->getPtrScores()->inverse(); }
    
    // Result writing
    std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (unclean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (unclean): " + XenCommon::toString(sD->getTargetCorps()->getPtrOutCorp()->getSize()) << std::endl;
    XenIO::cleanCorpusBi(sD->getSourceCorps()->getPtrOutCorp(), sD->getTargetCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
    std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (clean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (clean): " + XenCommon::toString(sD->getTargetCorps()->getPtrOutCorp()->getSize()) << std::endl;
    XenIO::writeBiOutput(sD->getSourceCorps()->getPtrOutCorp(), sD->getTargetCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
    
	return 0;
}
