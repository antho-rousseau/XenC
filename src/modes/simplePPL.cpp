/**
 *  @file simplePPL.cpp
 *  @brief Derived class to handle filtering mode 1: simple perplexity
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

#include "simplePPL.h"

SimplePPL::SimplePPL() {

}

SimplePPL::~SimplePPL() {
    
}

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
    
    if (!boost::filesystem::exists(sD->getSourceLMs()->getPtrInLM()->getFileName())) {
        std::cout << "Error: LM file " + sD->getSourceLMs()->getPtrInLM()->getFileName() + " does not exists!" << std::endl;
        return 1;
    }
    
    sD->getSourcePPLs()->getPtrInPPL()->initialize(sD->getSourceCorps()->getPtrOutCorp(), sD->getSourceLMs()->getPtrInLM());
    sD->getSourcePPLs()->getPtrInPPL()->calcPPLCorpus();
    
    if (opt->getWFile()->getFileName().compare("") != 0)
        sD->getWeightsFile()->initialize(opt->getWFile());
    
    for (unsigned int i = 0; i < sD->getSourcePPLs()->getPtrInPPL()->getSize(); i++) {
        double res = sD->getSourcePPLs()->getPtrInPPL()->getPPL(i);
        
        if (opt->getWFile()->getFileName().compare("") != 0)
            res = res * sD->getWeightsFile()->getWeight(i);
        
        sD->getScHold()->getPtrScores()->addScore(res);
    }
    
    sD->getScHold()->getPtrScores()->calibrate();
    if (opt->getInv()) { sD->getScHold()->getPtrScores()->inverse(); }
    
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
