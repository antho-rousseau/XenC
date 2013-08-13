/**
 *  @file mode.cpp
 *  @brief Abstract class defining the filtering modes architecture
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

#include "mode.h"

Mode::~Mode() {
    
}

int Mode::findSampleSize(boost::shared_ptr<Corpus> idCorp, boost::shared_ptr<Corpus> oodCorp) {
    int iW = idCorp->getWC();
	int oW = oodCorp->getWC();
    
	double res = (double)iW / (double)oW * 100;
	int i = (int)(res + 0.5f);
    
	if (i == 0) { i = 1; }
	if (i > 100) { i = 100; }
    
	return i;
}

Corpus Mode::extractSample(boost::shared_ptr<Corpus> ptrCorp, int sSize, bool mean) {
	double res = (double)ptrCorp->getWC() * ((double)sSize / 100);
	int max = (int)(res + 0.5f);
    
    std::string rnd = "";
    
    if (mean) {
        std::srand ((unsigned int)std::time(NULL));
        rnd = XenCommon::toString0(std::rand() % 10000);
    }
    
    std::string sampleName = ptrCorp->getXenFile()->getPrefix() + rnd + "-sample" + XenCommon::toString(sSize);
    std::string outFile = sampleName + "." + ptrCorp->getLang();
    
    if (!boost::filesystem::exists(outFile.c_str())) {
        std::srand((unsigned int)std::time(NULL));
        
        std::ofstream out(outFile.c_str(), std::ios::out | std::ios::trunc);
        
        int count = 0;
        
        while (count < max) {
            std::string line = ptrCorp->getLine(std::rand() % ptrCorp->getSize());
            out << line << std::endl;
            int n = XenCommon::wordCount(line);
            count += (n + 1);
        }
        
        out.close();
    }
    else {
        std::cout << "Sample file " << outFile << " already exists, reusing." << std::endl;
    }
    
	Corpus r;
    r.initialize(outFile, ptrCorp->getLang());
    
	return r;
}
