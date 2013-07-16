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
 * $Id: eval.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "eval.h"
#include "utils/StaticData.h"

/*
 *  Threaded function to handle parallel evaluation of final sorted file
 */
void taskEval(int pc, shared_ptr<XenResult> ptrXR, shared_ptr<XenVocab> ptrVoc, shared_ptr<Corpus> ptrDevCorp, shared_ptr<EvalMap> ptrDist) {
    cout << "Starting eval " + toString(pc) + " percent." << endl;
    
    shared_ptr<XenLMsri> ptrLM(new XenLMsri);
    ptrLM->initialize(ptrXR, ptrVoc, pc);
    ptrLM->createLM();
    
    shared_ptr<PPL> ptrPPL(new PPL);
    ptrPPL->initialize(ptrDevCorp, ptrLM);
    
    ptrDist->operator[](pc) = ptrPPL->getCorpPPL();
    
    cout << "Eval " + toString(pc) + " percent done." << endl;
}

/*
 *  Constructor from a XenResult
 *  (sorted file from filtering)
 */
Eval::Eval() {
    ptrDist = shared_ptr<EvalMap>(new EvalMap);
}

/*
 *  Constructor from a XenResult & an existing distfile
 *  (usually for best point estimation)
 */
Eval::Eval(string distFile) {
    ptrDist = shared_ptr<EvalMap>(new EvalMap);
    
	if (exists(distFile.c_str())) {
		cout << "Loading dist file " + distFile << endl;
		regex e1("(.*)\\t.*");
		regex e2(".*\\t(.*)");
		ifstream in(distFile.c_str(), ios::in);
		string line = "";
        
		while (getline(in, line)) {
			string key = regex_replace(line, e1, "\\1", boost::match_default | boost::format_sed);
			string value = regex_replace(line, e2, "\\1", boost::match_default | boost::format_sed);
            ptrDist->operator[](toInt(key)) = toDouble(value);
		}
        
		in.close();
	}
	else {
        cout << "Specified eval file " << distFile << " does not exists!" << endl;
        exit (1);
	}
}

/*
 *  Destructor
 */
Eval::~Eval() {

}

//  ------
//  Public
//  ------

/*
 *  Main threaded eval loop
 *  If you find yourself out of memory,
 *  consider using only one or two threads.
 */
void Eval::doEval(int h, int l) {
    XenOption* opt = XenOption::getInstance();
    StaticData* sD = StaticData::getInstance();
    
    if (opt->getSVocab()->getFileName().compare("") == 0) { sD->getVocabs()->getPtrSourceVoc()->initialize(sD->getSourceCorps()->getPtrInCorp()); }
	else { sD->getVocabs()->getPtrSourceVoc()->initialize(opt->getSVocab()); }
    
    pool threadPool(opt->getThreads());
    
	int pc = h;
    if (pc == 0) { pc = opt->getStep(); }
    
	while (pc >= l && pc >= opt->getStep()) {
        EvalMap::iterator found = ptrDist->find(pc);
        if (found == ptrDist->end())
            threadPool.schedule(boost::bind(taskEval, pc, sD->getXenResult(), sD->getVocabs()->getPtrSourceVoc(), sD->getDevCorp(), ptrDist));
            
        pc -= opt->getStep();
	}
    
    threadPool.wait();
    
	cout << "Evaluation done." << endl;
}

/*
 *  Best point estimation routine
 */
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
	cout << "Requiring eval from " + toString(hBound) + " to " + toString(lBound) + " with step " + toString(opt->getStep()) << endl;
	doEval(hBound, lBound);
    
	if (opt->getStep() > 1) {
		doBP();
	}
}

/*
 *  Returns the evaluation distribution
 */
EvalMap Eval::getDist() const {
	return *ptrDist;
}
