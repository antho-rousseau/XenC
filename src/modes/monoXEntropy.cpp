/**
 *  @file monoXEntropy.cpp
 *  @brief Derived class to handle filtering mode 2: monolingual cross-entropy
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

#include "../../include/modes/monoXEntropy.h"

MonoXEntropy::MonoXEntropy() {

}

MonoXEntropy::~MonoXEntropy() {

}

int MonoXEntropy::launch() {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    // Init corpus
    sD->getSourceCorps()->getPtrInCorp()->initialize(opt->getInSData(), opt->getSLang());
    sD->getSourceCorps()->getPtrOutCorp()->initialize(opt->getOutSData(), opt->getSLang());
    
    // Init vocabs
    if (opt->getSVocab()->getFileName().compare("") == 0) {
        if (opt->getFullVocab())
            sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getSourceCorps()->getPtrOutCorp());
        else
            sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp());
    }
    else
        sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab());
    
    // If we need similarity computation
    if (opt->getSim() || opt->getSimOnly()) {
        sD->getSim()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getSourceCorps()->getPtrOutCorp(), sD->getVocabs()->getPtrSourceVoc());
        XenIO::dumpSimilarity(sD->getSourceCorps()->getPtrOutCorp(), sD->getSim()); //Dump not definitive, only for testing
    }
    
    if (!opt->getSimOnly()) {
        // Init in-domain source LM
        if (opt->getInSLM()->getFileName().compare("") == 0) {
            sD->getSourceLMs()->getPtrInLM()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getVocabs()->getPtrSourceVoc());
            sD->getSourceLMs()->getPtrInLM()->createLM();
            //sD->getSourceLMs()->getPtrInLM()->writeLM();
        }
        else {
            sD->getSourceLMs()->getPtrInLM()->initialize(opt->getInSLM(), sD->getVocabs()->getPtrSourceVoc());
            sD->getSourceLMs()->getPtrInLM()->loadLM();
        }
        
        // Init out-of-domain source LM
        if (opt->getOutSLM()->getFileName().compare("") == 0) {
            opt->setSampleSize(Mode::findSampleSize(sD->getSourceCorps()->getPtrInCorp(), sD->getSourceCorps()->getPtrOutCorp()));
            
            boost::shared_ptr<Corpus> ptrOutLMCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
            sD->getSourceLMs()->getPtrOutLM()->initialize(ptrOutLMCorp, sD->getVocabs()->getPtrSourceVoc());
            sD->getSourceLMs()->getPtrOutLM()->createLM();
            //sD->getSourceLMs()->getPtrOutLM()->writeLM();
        }
        else {
            sD->getSourceLMs()->getPtrOutLM()->initialize(opt->getOutSLM(), sD->getVocabs()->getPtrSourceVoc());
            sD->getSourceLMs()->getPtrOutLM()->loadLM();
        }
        
        // Check for LM estimation OK
        if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) {
            std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl;
            return 1;
        }
        
        if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrOutLM()->getFileName())) {
            std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl;
            return 1;
        }
        
        // Init Mean LMs if needed
        if (opt->getMean()) {
            boost::shared_ptr<Corpus> ptrOutLMCorp2 = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
            sD->getMeanSourceLMs()->getPtrOutLM2()->initialize(ptrOutLMCorp2, sD->getVocabs()->getPtrSourceVoc());
            sD->getMeanSourceLMs()->getPtrOutLM2()->createLM();
            //sD->getMeanSourceLMs()->getPtrOutLM2()->writeLM();
            
            boost::shared_ptr<Corpus> ptrOutLMCorp3 = boost::make_shared<Corpus>(Mode::extractSample(sD->getSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), opt->getMean()));
            sD->getMeanSourceLMs()->getPtrOutLM3()->initialize(ptrOutLMCorp3, sD->getVocabs()->getPtrSourceVoc());
            sD->getMeanSourceLMs()->getPtrOutLM3()->createLM();
            //sD->getMeanSourceLMs()->getPtrOutLM3()->writeLM();
            
            if (!boost::filesystem::exists(sD->getMeanSourceLMs()->getPtrOutLM2()->getFileName())) {
                std::cout << "Error: LM file " + sD->getMeanSourceLMs()->getPtrOutLM2()->getFileName() + " does not exists!" << std::endl;
                return 1;
            }
            if (!boost::filesystem::exists(sD->getMeanSourceLMs()->getPtrOutLM3()->getFileName())) {
                std::cout << "Error: LM file " + sD->getMeanSourceLMs()->getPtrOutLM3()->getFileName() + " does not exists!" << std::endl;
                return 1;
            }
        }

        // Init all Stem data if needed
        if (opt->getStem()) {
            sD->getStemSourceCorps()->getPtrInCorp()->initialize(opt->getInSStem(), opt->getSLang());
            sD->getStemSourceCorps()->getPtrOutCorp()->initialize(opt->getOutSStem(), opt->getSLang());
            
            sD->getStemVocabs()->getPtrSourceVoc()->initialize(sD->getStemSourceCorps()->getPtrInCorp());
            
            sD->getStemSourceLMs()->getPtrInLM()->initialize(sD->getStemSourceCorps()->getPtrInCorp(), sD->getStemVocabs()->getPtrSourceVoc());
            sD->getStemSourceLMs()->getPtrInLM()->createLM();
            //sD->getStemSourceLMs()->getPtrInLM()->writeLM();
            
            opt->setSampleSize(Mode::findSampleSize(sD->getStemSourceCorps()->getPtrInCorp(), sD->getStemSourceCorps()->getPtrOutCorp()));
            boost::shared_ptr<Corpus> ptrOutSourceStemCorp = boost::make_shared<Corpus>(Mode::extractSample(sD->getStemSourceCorps()->getPtrOutCorp(), opt->getSampleSize(), false));
            sD->getStemSourceLMs()->getPtrOutLM()->initialize(ptrOutSourceStemCorp, sD->getStemVocabs()->getPtrSourceVoc());
            sD->getStemSourceLMs()->getPtrOutLM()->createLM();
            //sD->getStemSourceLMs()->getPtrOutLM()->writeLM();
            
            if (!boost::filesystem::exists(sD->getStemSourceLMs()->getPtrInLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl; return 1; }
            if (!boost::filesystem::exists(sD->getStemSourceLMs()->getPtrOutLM()->getFileName())) { std::cout << "Error: LM file " + sD->getStemSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl; return 1; }
        }
        
        // Init all PPL objects
        sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrInLM());
        sD->getSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
        sD->getSourcePPLs()->getPtrOutPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrOutLM());
        sD->getSourcePPLs()->getPtrOutPPL()->calcPPLCorpus();
        
        // Same for Mean if needed
        if (opt->getMean()) {
            sD->getMeanSourcePPLs()->getPtrOutPPL2()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getMeanSourceLMs()->getPtrOutLM2());
            sD->getMeanSourcePPLs()->getPtrOutPPL2()->calcPPLCorpus();
            sD->getMeanSourcePPLs()->getPtrOutPPL3()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getMeanSourceLMs()->getPtrOutLM3());
            sD->getMeanSourcePPLs()->getPtrOutPPL3()->calcPPLCorpus();
        }
        
        // Same for Stem if needed
        if (opt->getStem()) {
            sD->getStemSourcePPLs()->getPtrInPPL()->initialize(sD->getStemSourceCorps()->getPtrOutCorp(), sD->getStemSourceLMs()->getPtrInLM());
            sD->getStemSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
            sD->getStemSourcePPLs()->getPtrOutPPL()->initialize(sD->getStemSourceCorps()->getPtrOutCorp(), sD->getStemSourceLMs()->getPtrOutLM());
            sD->getStemSourcePPLs()->getPtrOutPPL()->calcPPLCorpus();
        }
        
        // Init weight file if needed
        if (opt->getWFile()->getFileName().compare("") != 0)
            sD->getWeightsFile()->initialize(opt->getWFile());
        
        // Scores computation
        for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
            double res = 0;
            
            if (opt->getMean())
                res = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - ((sD->getSourcePPLs()->getPtrOutPPL()->getXE(i) + sD->getMeanSourcePPLs()->getPtrOutPPL2()->getXE(i) + sD->getMeanSourcePPLs()->getPtrOutPPL3()->getXE(i)) / 3));
            else
                res = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getSourcePPLs()->getPtrOutPPL()->getXE(i));
            
            if (opt->getStem()) {
                res += (sD->getStemSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getStemSourcePPLs()->getPtrOutPPL()->getXE(i));
            }
            
            if (opt->getWFile()->getFileName().compare("") != 0)
                res = res * sD->getWeightsFile()->getWeight(i);
            
            sD->getScHold()->getPtrScXenC()->addScore(res);
        }
    }
    
    // Fill score holder for similarity if needed
    if (opt->getSim() || opt->getSimOnly()) {
        for (unsigned int i = 0; i < sD->getSim()->getSize(); i++)
            sD->getScHold()->getPtrScSimil()->addScore(sD->getSim()->getSim(i));
        
        sD->getScHold()->getPtrScSimil()->inverse();
    }
    
    // Scores combination if needed ==> NEEDS TESTING
    if (opt->getSimOnly()) { // Similarity alone (calibrate or not??? NEEDS TESTING)
        std::cout << "Scoring with similarity only." << std::endl;
        
        for (unsigned int i = 0; i < sD->getScHold()->getPtrScSimil()->getSize(); i++)
            sD->getScHold()->getPtrScores()->addScore(sD->getScHold()->getPtrScSimil()->getScore(i));
    }
    else if (opt->getSim()) { // Cross-entropy and similarity combination
        std::cout << "Scoring with cross-entropy and similarity combination." << std::endl;
        
        sD->getScHold()->getPtrScXenC()->calibrate(); // Needed for combination? Not sure... More tests needed
        
        for (unsigned int i = 0; i < sD->getScHold()->getPtrScXenC()->getSize(); i++)
            sD->getScHold()->getPtrScores()->addScore(sD->getScHold()->getPtrScXenC()->getScore(i) + sD->getScHold()->getPtrScSimil()->getScore(i));
        
        sD->getScHold()->getPtrScores()->calibrate();
    }
    else { // Cross-entropy alone
        std::cout << "Scoring with cross-entropy only." << std::endl;
        
        for (unsigned int i = 0; i < sD->getScHold()->getPtrScXenC()->getSize(); i++)
            sD->getScHold()->getPtrScores()->addScore(sD->getScHold()->getPtrScXenC()->getScore(i));
        
        sD->getScHold()->getPtrScores()->calibrate();
    }
    
    if (opt->getInv()) { sD->getScHold()->getPtrScores()->inverse(); }
    
    // Result writing
	if (opt->getTLang().compare("") == 0) {
        std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (unclean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) << std::endl;
        XenIO::cleanCorpusMono(sD->getSourceCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
        std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (clean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) << std::endl;
        XenIO::writeMonoOutput(sD->getSourceCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
    }
	else {
        boost::shared_ptr<Corpus> ptrOTCorp = boost::make_shared<Corpus>();
        ptrOTCorp->initialize(opt->getOutTData(), opt->getTLang());
        std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (unclean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (unclean): " + XenCommon::toString(ptrOTCorp->getSize()) << std::endl;
        XenIO::cleanCorpusBi(sD->getSourceCorps()->getPtrOutCorp(), ptrOTCorp, sD->getScHold()->getPtrScores());
        std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (clean): " + XenCommon::toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (clean): " + XenCommon::toString(ptrOTCorp->getSize()) << std::endl;
        XenIO::writeBiOutput(sD->getSourceCorps()->getPtrOutCorp(), ptrOTCorp, sD->getScHold()->getPtrScores());
	}

	return 0;
}
