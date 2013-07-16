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
 * $Id: ppl.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "ppl.h"

/*
 *  Threaded function to handle parallel computation of perplexity
 */
void taskCalcPPL(int n, string l, shared_ptr<vector<double> > p, shared_ptr<XenLMsri> lm) {
    TextStats tstats = lm->getSentenceStats(l);
    
    double denom = tstats.numWords - tstats.numOOVs - tstats.zeroProbs + tstats.numSentences;
    Prob prob = LogPtoPPL(tstats.prob / denom);
    
    p->operator[](n) = prob;
}

/*
 *  Default constructor
 */
PPL::PPL() {
    
}

/*
 *  Initialization from a Corpus & a LM
 */
void PPL::initialize(shared_ptr<Corpus> c, shared_ptr<XenLMsri> l) {
    ptrCorp = c;
    ptrPT = shared_ptr<PhraseTable>(new PhraseTable);
    ptrLM = l;
    source = false;
    ptrPPL = shared_ptr<vector<double> >(new vector<double>(ptrCorp->getSize(), 0.0));
}

/*
 *  Initialization from a PhraseTable & a LM
 *  Bool is here to specify if we're working
 *  on source or target phrases (true = source)
 */
void PPL::initialize(shared_ptr<PhraseTable> pt, shared_ptr<XenLMsri> l, bool s) {
    ptrCorp = shared_ptr<Corpus>(new Corpus);
    ptrPT = pt;
    ptrLM = l;
    source = s;
    ptrPPL = shared_ptr<vector<double> >(new vector<double>(ptrPT->getSize(), 0.0));
}

/*
 *  Destructor
 */
PPL::~PPL() {

}

//  ------
//  Public
//  ------

/*
 *  Returns the size of the computed perplexities
 */
unsigned int PPL::getSize() const {
	return (unsigned int)ptrPPL->size();
}

/*
 *  Returns the perplexity of the nth sentence/phrase
 */
double PPL::getPPL(int n) {
	return ptrPPL->operator[](n);
}

/*
 *  Returns the cross-entropy of the nth sentence/phrase
 */
double PPL::getXE(int n) {
	return PPL::crossEntropy(ptrPPL->operator[](n));
}

/*
 *  Returns the global perplexity of a Corpus
 */
double PPL::getCorpPPL() {
    cout << "Computing document perplexity score with LM " << ptrLM->getFileName() << "..." << endl;
    
    TextStats tstats = ptrLM->getDocumentStats(ptrCorp);
    
    double denom = tstats.numWords - tstats.numOOVs - tstats.zeroProbs + tstats.numSentences;
    
    cout << "Finished computing document perplexity score with LM " << ptrLM->getFileName() << "." << endl;
    
    return LogPtoPPL(tstats.prob / denom);
}

/*
 *  Computes the perplexity of sentences in a corpus
 */
void PPL::calcPPLCorpus() {
    XenOption* opt = XenOption::getInstance();
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
    
    ptrLM->loadLM();
    
    pool threadPool(opt->getThreads());
    
    cout << "Computing sentences perplexity scores with LM " << ptrLM->getFileName() << "..." << endl;
    
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++)
        threadPool.schedule(boost::bind(taskCalcPPL, i, ptrCorp->getLine(i), ptrPPL, ptrLM));
    
    threadPool.wait();
    
    cout << "Finished computing sentences perplexity scores with LM " << ptrLM->getFileName() << "." << endl;
}

/*
 *  Computes the perplexity of phrases from a PhraseTable
 */
void PPL::calcPPLPhraseTable() {
    XenOption* opt = XenOption::getInstance();
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");

    ptrLM->loadLM();
    
    pool threadPool(opt->getThreads());
    
    cout << "Computing phrases perplexity scores with LM " << ptrLM->getFileName() << "..." << endl;

    for (unsigned int i = 0; i < ptrPT->getSize(); i++)
        if (source)
            threadPool.schedule(boost::bind(taskCalcPPL, i, ptrPT->getSource(i), ptrPPL, ptrLM));
        else
            threadPool.schedule(boost::bind(taskCalcPPL, i, ptrPT->getTarget(i), ptrPPL, ptrLM));
    
    threadPool.wait();
    
    cout << "Finished computing phrases perplexity scores with LM " << ptrLM->getFileName() << "." << endl;
}

//  -------
//  Private
//  -------

/*
 *  Computes the cross-entropy based
 *  on the perplexity score
 */
double PPL::crossEntropy(double f) {
	double res = (log(f)/log(2));
	return res;
}
