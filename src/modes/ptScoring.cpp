/**
 *  @file ptScoring.cpp
 *  @brief Derived class to handle filtering mode 4: phrase-table cross-entropy
 *  @author Anthony Rousseau
 *  @version 1.2.0
 *  @date 19 August 2013
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
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

#include "ptScoring.h"

PTScoring::PTScoring() {
    
}

PTScoring::~PTScoring() {
    
}

int PTScoring::launch() {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab());
    sD->getVocabs()->getPtrTargetVoc()->initialize(opt->getTVocab());
    
    sD->getPTPairs()->getPtrInPT()->initialize(opt->getInPTable());
    sD->getPTPairs()->getPtrOutPT()->initialize(opt->getOutPTable());

    sD->getSourceCorps()->getPtrInCorp()->initialize(XenIO::writeSourcePhrases(sD->getPTPairs()->getPtrInPT()), opt->getSLang());
    sD->getSourceCorps()->getPtrOutCorp()->initialize(XenIO::writeSourcePhrases(sD->getPTPairs()->getPtrOutPT()), opt->getSLang());
    sD->getTargetCorps()->getPtrInCorp()->initialize(XenIO::writeTargetPhrases(sD->getPTPairs()->getPtrInPT()), opt->getTLang());
    sD->getTargetCorps()->getPtrOutCorp()->initialize(XenIO::writeTargetPhrases(sD->getPTPairs()->getPtrOutPT()), opt->getTLang());

    sD->getSourceLMs()->getPtrInLM()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getVocabs()->getPtrSourceVoc());
    sD->getSourceLMs()->getPtrOutLM()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getVocabs()->getPtrSourceVoc());
    sD->getTargetLMs()->getPtrInLM()->initialize(sD->getTargetCorps()->getPtrInCorp(), sD->getVocabs()->getPtrTargetVoc());
    sD->getTargetLMs()->getPtrOutLM()->initialize(sD->getTargetCorps()->getPtrOutCorp(), sD->getVocabs()->getPtrTargetVoc());
    
    if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) {
        std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl;
        return 1;
    }
	if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrOutLM()->getFileName())) {
        std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl;
        return 1;
    }
	if (!boost::filesystem::exists(sD->getTargetLMs()->getPtrInLM()->getFileName())) {
        std::cout << "Error: LM file " + sD->getTargetLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl;
        return 1;
    }
	if (!boost::filesystem::exists(sD->getTargetLMs()->getPtrOutLM()->getFileName())) {
        std::cout << "Error: LM file " + sD->getTargetLMs()->getPtrOutLM()->getFileName() + " does not exists!" << std::endl;
        return 1;
    }
    
    sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getSourceLMs()->getPtrInLM(), true);
    sD->getSourcePPLs()->getPtrInPPL()->calcPPLPhraseTable();
    sD->getSourcePPLs()->getPtrOutPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getSourceLMs()->getPtrOutLM(), true);
    sD->getSourcePPLs()->getPtrOutPPL()->calcPPLPhraseTable();
    sD->getTargetPPLs()->getPtrInPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getTargetLMs()->getPtrInLM(), false);
    sD->getTargetPPLs()->getPtrInPPL()->calcPPLPhraseTable();
    sD->getTargetPPLs()->getPtrOutPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getTargetLMs()->getPtrOutLM(), false);
    sD->getTargetPPLs()->getPtrOutPPL()->calcPPLPhraseTable();

    //---- Global scores ----
    std::cout << "Computing global scores." << std::endl;
    
    if (opt->getWFile()->getFileName().compare("") != 0)
        sD->getWeightsFile()->initialize(opt->getWFile());
    
    for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
		double resS = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getSourcePPLs()->getPtrOutPPL()->getXE(i));
		double resT = (sD->getTargetPPLs()->getPtrInPPL()->getXE(i) - sD->getTargetPPLs()->getPtrOutPPL()->getXE(i));
		double res = resS + resT;
        
        if (opt->getWFile()->getFileName().compare("") != 0)
            res = res * sD->getWeightsFile()->getWeight(i);
        
		sD->getScHold()->getPtrScores()->addScore(res);
	}
    
	sD->getScHold()->getPtrScores()->calibrate();
    sD->getScHold()->getPtrScores()->inverse();
    //-----------------------
    
    if (opt->getLocal()) {
        //---- Local scores ----
        std::cout << "Computing local scores." << std::endl;
        std::vector<SourcePhrase> srcPh = sD->getPTPairs()->getPtrOutPT()->getSrcPhrases();
        
        int offset = 0;
        
        for (unsigned int i = 0; i < srcPh.size(); i++) {
            SourcePhrase sP = srcPh[i];
            
            if (sP.getTargetSize() == 1) {
                sP.getScoresXE()->addScore((double)0);
            }
            else {
                for (unsigned int j = 0; j < sP.getTargetSize(); j++) {
                    double resS = (sD->getSourcePPLs()->getPtrInPPL()->getXE(j + offset) - sD->getSourcePPLs()->getPtrOutPPL()->getXE(j + offset));
                    double resT = (sD->getTargetPPLs()->getPtrInPPL()->getXE(j + offset) - sD->getTargetPPLs()->getPtrOutPPL()->getXE(j + offset));
                    double res = resS + resT;
                    sP.getScoresXE()->addScore(res);
                }
            }
            
            offset += sP.getTargetSize();
            if (sP.getTargetSize() > 1)
                sP.getScoresXE()->calibrate();
            sP.getScoresXE()->inverse();
            srcPh[i] = sP;
        }
        
        sD->getPTPairs()->getPtrOutPT()->setSrcPhrases(srcPh);
        //----------------------
    }
    
    std::cout << "NB Scores: " + XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Phrase-table: " + XenCommon::toString(sD->getPTPairs()->getPtrOutPT()->getSize()) << std::endl;
    
    if (XenCommon::toString(sD->getScHold()->getPtrScores()->getSize()) != XenCommon::toString(sD->getPTPairs()->getPtrOutPT()->getSize())) {
        std::cout << "Not the same number of scores, exiting." << std::endl;
        return 1;
    }
    
    XenIO::writeNewPT(sD->getPTPairs()->getPtrOutPT(), sD->getScHold()->getPtrScores());
    
    return 0;
}
