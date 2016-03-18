/**
 *  @file ppl.cpp
 *  @brief Class handling the perplexity/cross-entropy computations
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

#include "../include/ppl.h"

boost::mutex randy;

void taskCalcPPL(int numLine, std::string line, boost::shared_ptr<std::vector<double> > ptrPPL, boost::shared_ptr<XenLMken> ptrLM) {
    XenOption* opt = XenOption::getInstance();

    TxtStats tstats = ptrLM->getSentenceStats(line);

    double prob = 0.0;

    if (!opt->getExclOOVs())
        prob = pow(10.0, -(tstats.prob / static_cast<double>(tstats.numwords)));
    else
        prob = pow(10.0, -((tstats.prob - tstats.zeroprobs) / static_cast<double>(tstats.numwords - tstats.numoov + tstats.numsentences)));

    randy.lock();
    ptrPPL->operator[](numLine) = prob;
    randy.unlock();
}

PPL::PPL() {
    source = false;
}

void PPL::initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenLMken> ptrLM) {
    this->ptrCorp = ptrCorp;
    ptrPT = boost::make_shared<PhraseTable>();
    this->ptrLM = ptrLM;
    ptrPPL = boost::make_shared<vector<double> >(ptrCorp->getSize(), 0.0);
}

void PPL::initialize(boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<XenLMken> ptrLM, bool source) {
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
    XenOption* opt = XenOption::getInstance();
    std::cout << "Computing document perplexity score with LM " << ptrLM->getFileName() << "..." << std::endl;

    TxtStats tstats = ptrLM->getDocumentStats(ptrCorp);

    double prob = 0.0;

    if (!opt->getExclOOVs())
        prob = pow(10.0, -(tstats.prob / static_cast<double>(tstats.numwords)));
    else
        prob = pow(10.0, -((tstats.prob - tstats.zeroprobs) / static_cast<double>(tstats.numwords - tstats.numoov + tstats.numsentences)));

    std::cout << "Finished computing document perplexity score with LM " << ptrLM->getFileName() << "." << std::endl;
    
    return prob;
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
