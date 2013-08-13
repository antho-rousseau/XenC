/**
 *  @file eval.cpp
 *  @brief Class handling evaluation system
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

#include "eval.h"
#include "utils/StaticData.h"

void taskEval(int pc, boost::shared_ptr<XenResult> ptrXR, boost::shared_ptr<XenVocab> ptrVoc, boost::shared_ptr<Corpus> ptrDevCorp, boost::shared_ptr<EvalMap> ptrDist) {
    std::cout << "Starting eval " + XenCommon::toString(pc) + " percent." << std::endl;
    
    boost::shared_ptr<XenLMsri> ptrLM = boost::make_shared<XenLMsri>();
    ptrLM->initialize(ptrXR, ptrVoc, pc);
    ptrLM->createLM();
    
    boost::shared_ptr<PPL> ptrPPL = boost::make_shared<PPL>();
    ptrPPL->initialize(ptrDevCorp, ptrLM);
    
    ptrDist->operator[](pc) = ptrPPL->getCorpPPL();
    
    std::cout << "Eval " + XenCommon::toString(pc) + " percent done, PPL = " + XenCommon::toString(ptrPPL->getCorpPPL()) + "." << std::endl;
}

Eval::Eval() {
    ptrDist = boost::make_shared<EvalMap>();
}

Eval::Eval(std::string distFile) {
    ptrDist = XenIO::readDist(distFile);
}

Eval::~Eval() {
    
}

void Eval::doEval(int high, int low) {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    if (opt->getSVocab()->getFileName().compare("") == 0) { sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp()); }
	else { sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab()); }
    
    pool threadPool(opt->getThreads());
    
	int pc = high;
    if (pc == 0) { pc = opt->getStep(); }
    
	while (pc >= low && pc >= opt->getStep()) {
        EvalMap::iterator found = ptrDist->find(pc);
        if (found == ptrDist->end())
            threadPool.schedule(boost::bind(taskEval, pc, sD->getXenResult(), sD->getVocabs()->getPtrSourceVoc(), sD->getDevCorp(), ptrDist));
        
        pc -= opt->getStep();
	}
    
    threadPool.wait();
    
    std::cout << "Evaluation done." << std::endl;
}

void Eval::doBP() {
    XenOption* opt = XenOption::getInstance();
    
	double lower = 999999999999;
	int lBound = 0;
	int hBound = 0;
    
	for(EvalMap::iterator it = ptrDist->begin() ; it != ptrDist->end() ; ++it)
        if (it->second < lower) {
            lower = it->second;
            EvalMap::iterator itH = it; --itH;
            EvalMap::iterator itL = it; ++itL;
            lBound = itL->first;
            hBound = itH->first;
            if (lBound > it->first) { lBound = 0; }
            if (hBound < it->first) { hBound = 100; }
        }
    
	opt->setStep(opt->getStep() / 2);
    std::cout << "Requiring eval from " + XenCommon::toString(hBound) + " to " + XenCommon::toString(lBound) + " with step " + XenCommon::toString(opt->getStep()) << std::endl;
	doEval(hBound, lBound);
    
	if (opt->getStep() > 1) {
		doBP();
	}
}

boost::shared_ptr<EvalMap> Eval::getDist() const {
	return ptrDist;
}
