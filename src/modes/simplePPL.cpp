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
 * $Id: simplePPL.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "simplePPL.h"

SimplePPL::SimplePPL() {

}

SimplePPL::~SimplePPL() {
    
}

//  ------
//  Public
//  ------

/*
 *  Launches operations for this mode
 */
int SimplePPL::launch() {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    sD->getSourceCorps()->getPtrInCorp()->initialize(opt->getInSData(), opt->getSLang());
    sD->getSourceCorps()->getPtrOutCorp()->initialize(opt->getOutSData(), opt->getSLang());
    
    if (opt->getSVocab()->getFileName().compare("") == 0) { sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp()); }
	else { sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab()); }
    
    if (opt->getInSLM()->getFileName().compare("") == 0) {
        sD->getSourceLMs()->getPtrInLM()->initialize(sD->getSourceCorps()->getPtrInCorp(), sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrInLM()->createLM();
        sD->getSourceLMs()->getPtrInLM()->writeLM();
    }
    else {
        sD->getSourceLMs()->getPtrInLM()->initialize(opt->getInSLM(), sD->getVocabs()->getPtrSourceVoc());
        sD->getSourceLMs()->getPtrInLM()->loadLM();
    }
    
    if (!exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) {
        cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << endl;
        return 1;
    }
    
    sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrInLM());
    sD->getSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
    
    if (opt->getWFile()->getFileName().compare("") != 0)
        sD->getWeightsFile()->initialize(opt->getWFile());
    
    for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
        double res = sD->getSourcePPLs()->getPtrInPPL()->getPPL(i);
        
        if (opt->getWFile()->getFileName().compare("") != 0)
            res = res * sD->getWeightsFile()->getScore(i);
        
        sD->getScHold()->getPtrScores()->addScore(res);
    }
    
    sD->getScHold()->getPtrScores()->calibrate();
    if (opt->getInv()) { sD->getScHold()->getPtrScores()->inverse(); }
    
    if (opt->getTLang().compare("") == 0) {
        cout << "NB Scores: " + toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (unclean): " + toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) << endl;
        XenIO::cleanCorpusMono(sD->getSourceCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
        cout << "NB Scores: " + toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (clean): " + toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) << endl;
        XenIO::writeMonoOutput(sD->getSourceCorps()->getPtrOutCorp(), sD->getScHold()->getPtrScores());
    }
    else {
        shared_ptr<Corpus> ptrOTCorp(new Corpus);
        ptrOTCorp->initialize(opt->getOutTData(), opt->getTLang());
        cout << "NB Scores: " + toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (unclean): " + toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (unclean): " + toString(ptrOTCorp->getSize()) << endl;
        XenIO::cleanCorpusBi(sD->getSourceCorps()->getPtrOutCorp(), ptrOTCorp, sD->getScHold()->getPtrScores());
        cout << "NB Scores: " + toString(sD->getScHold()->getPtrScores()->getSize()) + " NB Source corp (clean): " + toString(sD->getSourceCorps()->getPtrOutCorp()->getSize()) + " NB Target corp (clean): " + toString(ptrOTCorp->getSize()) << endl;
        XenIO::writeBiOutput(sD->getSourceCorps()->getPtrOutCorp(), ptrOTCorp, sD->getScHold()->getPtrScores());
    }
    
    return 0;
}
