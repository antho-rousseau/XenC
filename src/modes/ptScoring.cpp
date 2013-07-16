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
 * $Id: ptScoring.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "ptScoring.h"

PTScoring::PTScoring() {
    
}

PTScoring::~PTScoring() {
    
}

//  ------
//  Public
//  ------

/*
 *  Launches operations for this mode
 */
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
    
    if (!exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) {
        cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << endl;
        return 1;
    }
	if (!exists(sD->getSourceLMs()->getPtrOutLM()->getFileName())) {
        cout << "Error: LM file " + sD->getSourceLMs()->getPtrOutLM()->getFileName() + " does not exists!" << endl;
        return 1;
    }
	if (!exists(sD->getTargetLMs()->getPtrInLM()->getFileName())) {
        cout << "Error: LM file " + sD->getTargetLMs()->getPtrInLM()->getFileName() + " does not exists!" << endl;
        return 1;
    }
	if (!exists(sD->getTargetLMs()->getPtrOutLM()->getFileName())) {
        cout << "Error: LM file " + sD->getTargetLMs()->getPtrOutLM()->getFileName() + " does not exists!" << endl;
        return 1;
    }
    
    sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getSourceLMs()->getPtrInLM(), true);
    sD->getSourcePPLs()->getPtrOutPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getSourceLMs()->getPtrOutLM(), true);
    sD->getTargetPPLs()->getPtrInPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getTargetLMs()->getPtrInLM(), false);
    sD->getTargetPPLs()->getPtrOutPPL()->initialize(sD->getPTPairs()->getPtrOutPT(), sD->getTargetLMs()->getPtrOutLM(), false);

    //---- Global scores ----
    cout << "Computing global scores." << endl;
    
    if (opt->getWFile()->getFileName().compare("") != 0)
        sD->getWeightsFile()->initialize(opt->getWFile());
    
    for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
		double resS = (sD->getSourcePPLs()->getPtrInPPL()->getXE(i) - sD->getSourcePPLs()->getPtrOutPPL()->getXE(i));
		double resT = (sD->getTargetPPLs()->getPtrInPPL()->getXE(i) - sD->getTargetPPLs()->getPtrOutPPL()->getXE(i));
		double res = resS + resT;
        
        if (opt->getWFile()->getFileName().compare("") != 0)
            res = res * sD->getWeightsFile()->getScore(i);
        
		sD->getScHold()->getPtrScores()->addScore(res);
	}
    
	sD->getScHold()->getPtrScores()->calibrate();
    sD->getScHold()->getPtrScores()->inverse();
    //-----------------------
    
    if (opt->getLocal()) {
        //---- Local scores ----
        cout << "Computing local scores." << endl;
        vector<SourcePhrase> srcPh = sD->getPTPairs()->getPtrOutPT()->getSrcPhrases();
        
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
    
    cout << "NB Scores: " + toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Phrase-table: " + toString(sD->getPTPairs()->getPtrOutPT()->getSize()) << endl;
    
    if (toString(sD->getScHold()->getPtrScores()->getSize()) != toString(sD->getPTPairs()->getPtrOutPT()->getSize())) {
        cout << "Not the same number of scores, exiting." << endl;
        return 1;
    }
    
    XenIO::writeNewPT(sD->getPTPairs()->getPtrOutPT(), sD->getScHold()->getPtrScores());
    
    return 0;
}
