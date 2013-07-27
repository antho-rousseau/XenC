/**
 *  @file ppl.cpp
 *  @brief Class handling the perplexity/cross-entropy computations
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

#include "ppl.h"

void taskCalcPPL(int numLine, std::string line, boost::shared_ptr<std::vector<double> > ptrPPL, boost::shared_ptr<XenLMsri> ptrLM) {
    TextStats tstats = ptrLM->getSentenceStats(line);
    
    double denom = tstats.numWords - tstats.numOOVs - tstats.zeroProbs + tstats.numSentences;
    Prob prob = LogPtoPPL(tstats.prob / denom);
    
    ptrPPL->operator[](numLine) = prob;
}

PPL::PPL() {
    source = false;
}

void PPL::initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenLMsri> ptrLM) {
    this->ptrCorp = ptrCorp;
    ptrPT = boost::make_shared<PhraseTable>();
    this->ptrLM = ptrLM;
    ptrPPL = boost::make_shared<vector<double> >(ptrCorp->getSize(), 0.0);
}

void PPL::initialize(boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<XenLMsri> ptrLM, bool source) {
    ptrCorp = boost::make_shared<Corpus>();
    this->ptrPT = ptrPT;
    this->ptrLM = ptrLM;
    this->source = source;
    ptrPPL = boost::make_shared<vector<double> >(ptrPT->getSize(), 0.0);
}

PPL::~PPL() {

}

unsigned int PPL::getSize() const {
	return (unsigned int)ptrPPL->size();
}

double PPL::getPPL(int n) {
	return ptrPPL->operator[](n);
}

double PPL::getXE(int n) {
	return crossEntropy(ptrPPL->operator[](n));
}

double PPL::getCorpPPL() {
    std::cout << "Computing document perplexity score with LM " << ptrLM->getFileName() << "..." << std::endl;
    
    TextStats tstats = ptrLM->getDocumentStats(ptrCorp);
    
    double denom = tstats.numWords - tstats.numOOVs - tstats.zeroProbs + tstats.numSentences;
    
    std::cout << "Finished computing document perplexity score with LM " << ptrLM->getFileName() << "." << std::endl;
    
    return LogPtoPPL(tstats.prob / denom);
}

void PPL::calcPPLCorpus() {
    XenOption* opt = XenOption::getInstance();

    ptrLM->loadLM();
    
    pool threadPool(opt->getThreads());
    
    std::cout << "Computing sentences perplexity scores with LM " << ptrLM->getFileName() << "..." << std::endl;
    
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++)
        threadPool.schedule(boost::bind(taskCalcPPL, i, ptrCorp->getLine(i), ptrPPL, ptrLM));
    
    threadPool.wait();
    
    std::cout << "Finished computing sentences perplexity scores with LM " << ptrLM->getFileName() << "." << std::endl;
}

void PPL::calcPPLPhraseTable() {
    XenOption* opt = XenOption::getInstance();

    ptrLM->loadLM();
    
    pool threadPool(opt->getThreads());
    
    std::cout << "Computing phrases perplexity scores with LM " << ptrLM->getFileName() << "..." << std::endl;

    for (unsigned int i = 0; i < ptrPT->getSize(); i++)
        if (source)
            threadPool.schedule(boost::bind(taskCalcPPL, i, ptrPT->getSource(i), ptrPPL, ptrLM));
        else
            threadPool.schedule(boost::bind(taskCalcPPL, i, ptrPT->getTarget(i), ptrPPL, ptrLM));
    
    threadPool.wait();
    
    std::cout << "Finished computing phrases perplexity scores with LM " << ptrLM->getFileName() << "." << std::endl;
}

double PPL::crossEntropy(double ppl) {
	return std::log(ppl)/std::log(2);
}
