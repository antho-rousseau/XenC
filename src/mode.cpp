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
 * $Id: mode.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "mode.h"

/*
 *  Pure virtual destructor
 */
Mode::~Mode() {
    
}

//  ---------
//  Protected
//  ---------

/*
 *  Computes the size of the sample which will be used for
 *  out-of-domain LM estimation
 */
void Mode::findSampleSize(shared_ptr<Corpus> iC, shared_ptr<Corpus> oC) {
    XenOption* opt = XenOption::getInstance();
    
	int iW = iC->getWC();
	int oW = oC->getWC();
    
	double res = (double)iW / (double)oW * 100;
	int i = (int)(res + 0.5f);
    
	if (i == 0) { i = 1; }
	if (i > 100) { i = 100; }
    
	opt->setSampleSize(i);
}

/*
 *  Extracts the previously computed sample size from
 *  the OOD corpus
 */
Corpus Mode::extractSample(shared_ptr<Corpus> c, int sS, bool mean) {
	double res = (double)c->getWC() * ((double)sS / 100);
	int max = (int)(res + 0.5f);
    
    string rnd = "";
    
    if (mean) {
        srand ((unsigned int)time(NULL));
        rnd = toString0(rand() % 10000);
    }
    
	string sampleName = c->getXenFile()->getPrefix() + rnd + "-sample" + toString(sS);
	string outFile = sampleName + "." + c->getLang();
    
    if (!exists(outFile.c_str())) {
        srand((unsigned int)time(NULL));
        
        ofstream out(outFile.c_str(), ios::out | ios::trunc);
        
        int count = 0;
        
        while (count < max) {
            string line = c->getLine(rand() % c->getSize());
            out << line << endl;
            int n = XenCommon::wordCount(line);
            count += (n + 1);
        }
        
        out.close();
    }
    else {
        cout << "Sample file " << outFile << " already exists, reusing." << endl;
    }
    
	Corpus r;
    r.initialize(outFile, c->getLang());

	return r;
}
