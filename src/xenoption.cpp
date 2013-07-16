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
 * $Id: xenoption.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
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
    
    inSD = shared_ptr<XenFile>(new XenFile);
    inSD->initialize(opt->inSData);
    
    outSD = shared_ptr<XenFile>(new XenFile);
	outSD->initialize(opt->outSData);
    
    inTD = shared_ptr<XenFile>(new XenFile);
    inTD->initialize(opt->inTData);
    
    outTD = shared_ptr<XenFile>(new XenFile);
    outTD->initialize(opt->outTData);
    
    inSS = shared_ptr<XenFile>(new XenFile);
    inSS->initialize(opt->inSStem);
    
    outSS = shared_ptr<XenFile>(new XenFile);
	outSS->initialize(opt->outSStem);
    
    inTS = shared_ptr<XenFile>(new XenFile);
    inTS->initialize(opt->inTStem);
    
    outTS = shared_ptr<XenFile>(new XenFile);
    outTS->initialize(opt->outTStem);
    
    inPT = shared_ptr<XenFile>(new XenFile);
    inPT->initialize(opt->iPTable);
    
    outPT = shared_ptr<XenFile>(new XenFile);
    outPT->initialize(opt->oPTable);
    
    sVoc = shared_ptr<XenFile>(new XenFile);
    sVoc->initialize(opt->sVocab);
    
    tVoc = shared_ptr<XenFile>(new XenFile);
	tVoc->initialize(opt->tVocab);
    
    inSLM = shared_ptr<XenFile>(new XenFile);
	inSLM->initialize(opt->inSLM);
    
    outSLM = shared_ptr<XenFile>(new XenFile);
	outSLM->initialize(opt->outSLM);
    
    inTLM = shared_ptr<XenFile>(new XenFile);
	inTLM->initialize(opt->inTLM);
    
    outTLM = shared_ptr<XenFile>(new XenFile);
	outTLM->initialize(opt->outTLM);
    
    wFile = shared_ptr<XenFile>(new XenFile);
	wFile->initialize(opt->wFile);
    
    dev = shared_ptr<XenFile>(new XenFile);
	dev->initialize(opt->dev);
}

XenOption::~XenOption() {

}

string XenOption::getSLang() const {
    return opt->sLang;
}

string XenOption::getTLang() const {
    return opt->tLang;
}

shared_ptr<XenFile> XenOption::getInSData() {
    return inSD;
}

shared_ptr<XenFile> XenOption::getOutSData() {
    return outSD;
}

shared_ptr<XenFile> XenOption::getInTData() {
    return inTD;
}

shared_ptr<XenFile> XenOption::getOutTData() {
    return outTD;
}

shared_ptr<XenFile> XenOption::getInSStem() {
    return inSS;
}

shared_ptr<XenFile> XenOption::getOutSStem() {
    return outSS;
}

shared_ptr<XenFile> XenOption::getInTStem() {
    return inTS;
}

shared_ptr<XenFile> XenOption::getOutTStem() {
    return outTS;
}

shared_ptr<XenFile> XenOption::getInPTable() {
    return inPT;
}

shared_ptr<XenFile> XenOption::getOutPTable() {
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

shared_ptr<XenFile> XenOption::getSVocab() {
    return sVoc;
}

shared_ptr<XenFile> XenOption::getTVocab() {
    return tVoc;
}

shared_ptr<XenFile> XenOption::getInSLM() {
    return inSLM;
}

shared_ptr<XenFile> XenOption::getOutSLM() {
    return outSLM;
}

shared_ptr<XenFile> XenOption::getInTLM() {
    return inTLM;
}

shared_ptr<XenFile> XenOption::getOutTLM() {
    return outTLM;
}

shared_ptr<XenFile> XenOption::getWFile() {
    return wFile;
}

shared_ptr<XenFile> XenOption::getDev() {
    return dev;
}

int XenOption::getOrder() const {
    return opt->order;
}

string XenOption::getCutoff() const {
    return opt->cutoff;
}

string XenOption::getSmooth() const {
    return opt->smooth;
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

string XenOption::getOutName() const {
    return opt->outName;
}

string XenOption::getName() const {
    return opt->name;
}

int XenOption::getThreads() const {
    return opt->threads;
}

bool XenOption::getSortOnly() const {
    return opt->sortOnly;
}

void XenOption::setSampleSize(int sS) {
    opt->sampleSize = sS;
}

void XenOption::setEval(bool ev) {
    opt->eval = ev;
}

void XenOption::setBp(bool bp) {
    opt->bp = bp;
}

void XenOption::setStep(int st) {
    opt->step = st;
}
