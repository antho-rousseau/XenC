/**
 *  @file xenoption.cpp
 *  @brief Singleton class handling XenC options accessors/mutators
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

#include "xenoption.h"

XenOption* XenOption::_instance = NULL;

XenOption* XenOption::getInstance() {
    if (_instance == NULL)
        _instance =  new XenOption();
    
    return _instance;
}

XenOption* XenOption::getInstance(LPOptions o) {
    if (_instance == NULL)
        _instance =  new XenOption(o);
    
    return _instance;
}

void XenOption::deleteInstance() {
    delete _instance;
    _instance = NULL;
}

XenOption::XenOption() {
    
}

XenOption::XenOption(LPOptions o) {
    opt = o;
    
    inSD = boost::make_shared<XenFile>();
    inSD->initialize(opt->inSData);
    
    outSD = boost::make_shared<XenFile>();
	outSD->initialize(opt->outSData);
    
    inTD = boost::make_shared<XenFile>();
    inTD->initialize(opt->inTData);
    
    outTD = boost::make_shared<XenFile>();
    outTD->initialize(opt->outTData);
    
    inSS = boost::make_shared<XenFile>();
    inSS->initialize(opt->inSStem);
    
    outSS = boost::make_shared<XenFile>();
	outSS->initialize(opt->outSStem);
    
    inTS = boost::make_shared<XenFile>();
    inTS->initialize(opt->inTStem);
    
    outTS = boost::make_shared<XenFile>();
    outTS->initialize(opt->outTStem);
    
    inPT = boost::make_shared<XenFile>();
    inPT->initialize(opt->iPTable);
    
    outPT = boost::make_shared<XenFile>();
    outPT->initialize(opt->oPTable);
    
    sVoc = boost::make_shared<XenFile>();
    sVoc->initialize(opt->sVocab);
    
    tVoc = boost::make_shared<XenFile>();
	tVoc->initialize(opt->tVocab);
    
    inSLM = boost::make_shared<XenFile>();
	inSLM->initialize(opt->inSLM);
    
    outSLM = boost::make_shared<XenFile>();
	outSLM->initialize(opt->outSLM);
    
    inTLM = boost::make_shared<XenFile>();
	inTLM->initialize(opt->inTLM);
    
    outTLM = boost::make_shared<XenFile>();
	outTLM->initialize(opt->outTLM);
    
    wFile = boost::make_shared<XenFile>();
	wFile->initialize(opt->wFile);
    
    dev = boost::make_shared<XenFile>();
	dev->initialize(opt->dev);
}

XenOption::~XenOption() {

}

std::string XenOption::getSLang() const {
    return opt->sLang;
}

std::string XenOption::getTLang() const {
    return opt->tLang;
}

boost::shared_ptr<XenFile> XenOption::getInSData() const {
    return inSD;
}

boost::shared_ptr<XenFile> XenOption::getOutSData() const {
    return outSD;
}

boost::shared_ptr<XenFile> XenOption::getInTData() const {
    return inTD;
}

boost::shared_ptr<XenFile> XenOption::getOutTData() const {
    return outTD;
}

boost::shared_ptr<XenFile> XenOption::getInSStem() const {
    return inSS;
}

boost::shared_ptr<XenFile> XenOption::getOutSStem() const {
    return outSS;
}

boost::shared_ptr<XenFile> XenOption::getInTStem() const {
    return inTS;
}

boost::shared_ptr<XenFile> XenOption::getOutTStem() const {
    return outTS;
}

boost::shared_ptr<XenFile> XenOption::getInPTable() const {
    return inPT;
}

boost::shared_ptr<XenFile> XenOption::getOutPTable() const {
    return outPT;
}

bool XenOption::getMono() const {
    return opt->mono;
}

int XenOption::getMode() const {
    return opt->mode;
}

bool XenOption::getMean() const {
    return opt->mean;
}

bool XenOption::getSim() const {
    return opt->sim;
}

bool XenOption::getSimOnly() const {
    return opt->simOnly;
}

int XenOption::getVecSize() const {
    return opt->vecSize;
}

boost::shared_ptr<XenFile> XenOption::getSVocab() const {
    return sVoc;
}

boost::shared_ptr<XenFile> XenOption::getTVocab() const {
    return tVoc;
}

bool XenOption::getFullVocab() const {
    return opt->fullVoc;
}

boost::shared_ptr<XenFile> XenOption::getInSLM() const {
    return inSLM;
}

boost::shared_ptr<XenFile> XenOption::getOutSLM() const {
    return outSLM;
}

boost::shared_ptr<XenFile> XenOption::getInTLM() const {
    return inTLM;
}

boost::shared_ptr<XenFile> XenOption::getOutTLM() const {
    return outTLM;
}

boost::shared_ptr<XenFile> XenOption::getWFile() const {
    return wFile;
}

boost::shared_ptr<XenFile> XenOption::getDev() const {
    return dev;
}

int XenOption::getOrder() const {
    return opt->order;
}

int XenOption::getDiscount() const {
    return opt->discount;
}

bool XenOption::getToLower() const {
    return opt->toLower;
}

bool XenOption::noUnkIsWord() const {
    return opt->noUnkIsWord;
}

int XenOption::getBinLM() const {
    return opt->binLM;
}

int XenOption::getSampleSize() const {
    return opt->sampleSize;
}

bool XenOption::getLog() const {
    return opt->log;
}

bool XenOption::getRev() const {
    return opt->rev;
}

bool XenOption::getInv() const {
    return opt->inv;
}

bool XenOption::getStem() const {
    return opt->stem;
}

bool XenOption::getLocal() const {
    return opt->local;
}

bool XenOption::getEval() const {
    return opt->eval;
}

bool XenOption::getBp() const {
    return opt->bp;
}

int XenOption::getStep() const {
    return opt->step;
}

std::string XenOption::getOutName() const {
    return opt->outName;
}

std::string XenOption::getName() const {
    return opt->name;
}

int XenOption::getThreads() const {
    return opt->threads;
}

bool XenOption::getSortOnly() const {
    return opt->sortOnly;
}

void XenOption::setSampleSize(int size) {
    opt->sampleSize = size;
}

void XenOption::setStep(int step) {
    opt->step = step;
}
