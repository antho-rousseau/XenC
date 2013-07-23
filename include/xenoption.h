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
 * $Id: xenoption.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle options accessors/mutators
 *  This is a singleton
 */

#ifndef XENOPTION_H_
#define XENOPTION_H_

#include "utils/common.h"

#include "xenfile.h"

class XenOption {
public:
    static XenOption* getInstance();
    static XenOption* getInstance(LPOptions);
    static void deleteInstance();
    // ---------------------
    string getSLang() const;
	string getTLang() const;
	shared_ptr<XenFile> getInSData();
	shared_ptr<XenFile> getOutSData();
    shared_ptr<XenFile> getInTData();
    shared_ptr<XenFile> getOutTData();
    shared_ptr<XenFile> getInSStem();
	shared_ptr<XenFile> getOutSStem();
    shared_ptr<XenFile> getInTStem();
    shared_ptr<XenFile> getOutTStem();
    shared_ptr<XenFile> getInPTable();
    shared_ptr<XenFile> getOutPTable();
    bool getMono() const;
	int getMode() const;
    bool getMean() const;
    bool getSim() const;
    bool getSimOnly() const;
    int getVecSize() const;
	shared_ptr<XenFile> getSVocab();
	shared_ptr<XenFile> getTVocab();
	shared_ptr<XenFile> getInSLM();
	shared_ptr<XenFile> getOutSLM();
	shared_ptr<XenFile> getInTLM();
	shared_ptr<XenFile> getOutTLM();
	shared_ptr<XenFile> getWFile();
	shared_ptr<XenFile> getDev();
	int getOrder() const;
    int getDiscount() const;
	string getCutoff() const;
	string getSmooth() const;
    int getBinLM() const;
	int getSampleSize() const;
    bool getLog() const;
	bool getRev() const;
    bool getInv() const;
    bool getStem() const;
    bool getLocal() const;
	bool getEval() const;
	bool getBp() const;
	int getStep() const;
    string getOutName() const;
    string getName() const;
    int getThreads() const;
    bool getSortOnly() const;
    void setSampleSize(int);
    void setEval(bool);
    void setBp(bool);
    void setStep(int);
    
private:
    XenOption();
    XenOption(LPOptions);
    ~XenOption();
    //------------
    LPOptions opt;
    static XenOption* _instance;
    shared_ptr<XenFile> inSD;
	shared_ptr<XenFile> outSD;
    shared_ptr<XenFile> inTD;
    shared_ptr<XenFile> outTD;
    shared_ptr<XenFile> inSS;
	shared_ptr<XenFile> outSS;
    shared_ptr<XenFile> inTS;
    shared_ptr<XenFile> outTS;
    shared_ptr<XenFile> inPT;
    shared_ptr<XenFile> outPT;
    shared_ptr<XenFile> sVoc;
	shared_ptr<XenFile> tVoc;
	shared_ptr<XenFile> inSLM;
	shared_ptr<XenFile> outSLM;
	shared_ptr<XenFile> inTLM;
	shared_ptr<XenFile> outTLM;
	shared_ptr<XenFile> wFile;
	shared_ptr<XenFile> dev;
};

#endif
