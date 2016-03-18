/**
 *  @file similarity.cpp
 *  @brief Class taking care of all the similarity measure computations
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

#include "../include/similarity.h"

Similarity::Similarity() {
    
}

void Similarity::initialize(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp, boost::shared_ptr<XenVocab> ptrVocab) {
    ptrVoc = ptrVocab;
    ptrID = ptrInCorp;
    ptrOOD = ptrOutCorp;
    
    loadWords();
    computeInDomainTFIDF();
    computeOutOfDomainTFIDF();
    buildIDVector();
    computeOutOfDomainIDF();
    computeSimilarity();
}

Similarity::~Similarity() {
    
}

float Similarity::getSim(int n) {
    return ptrOodSimilarity->operator[](n);
}

unsigned int Similarity::getSize() const {
    return (unsigned int)ptrOodSimilarity->size();
}

void Similarity::loadWords() {
    std::cout << "Loading vocab words for similarity vector building." << std::endl;
    
    std::map<std::string, int> tmp = ptrVoc->getXenVocab();
    
    for (std::map<std::string,int>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        ptrWords->push_back(it->first);
    
    std::cout << "Done loading vocab words for similarity vector building." << std::endl;
}

void Similarity::computeInDomainTFIDF() {
    std::cout << "Computing in-domain TF-IDF." << std::endl;
    
    std::map<std::string, int> tmpIdIDF;
    std::map<std::string, int> tmpOodIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        ptrIdTf->operator[](ptrWords->operator[](i)) = 0;
        tmpIdIDF[ptrWords->operator[](i)] = 0;
        tmpOodIDF[ptrWords->operator[](i)] = 0;
        ptrIdTfIdf->operator[](ptrWords->operator[](i)) = 0.0;
    }
    
    for (unsigned int i = 0; i < ptrID->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrID->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            int found = (int)tmpIdIDF.count(lWords[j]);
            
            if (found == 1) {
                tmpIdIDF[lWords[j]] = 1;
                ptrIdTf->operator[](lWords[j]) = ptrIdTf->operator[](lWords[j]) + 1;
            }
        }
    }

    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        std::map<std::string, int> lineIDF;
        std::vector<std::string> lWords;
        std::string line = ptrOOD->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (std::map<std::string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
            int found = (int)tmpOodIDF.count(it->first);
            
            if (found == 1) {
                tmpOodIDF[it->first] = tmpOodIDF[it->first] + 1;
            }
        }
    }
    
    int nbDocs = ptrOOD->getSize() + 1;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        float IDF = 0.0;
        if (tmpIdIDF[ptrWords->operator[](i)] + tmpOodIDF[ptrWords->operator[](i)] != 0)
            IDF = log((float)nbDocs / (float)(tmpIdIDF[ptrWords->operator[](i)] + tmpOodIDF[ptrWords->operator[](i)]));
        ptrIdTfIdf->operator[](ptrWords->operator[](i)) = (float)ptrIdTf->operator[](ptrWords->operator[](i)) * (float)IDF;
    }
    
    /** @todo DEBUG, needs more testing */
    std::string outName = ptrID->getXenFile()->getPrefix() + ".tfidf";
    
    std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        out << ptrWords->operator[](i) << '\t' << ptrIdTf->operator[](ptrWords->operator[](i)) << " --- " << tmpIdIDF[ptrWords->operator[](i)] << " --- " << tmpOodIDF[ptrWords->operator[](i)] << " --> " << ptrIdTfIdf->operator[](ptrWords->operator[](i)) << std::endl;
    }
    
    out.close();
    
    std::cout << "Done computing in-domain TF-IDF." << std::endl;
}

void Similarity::computeOutOfDomainTFIDF() {
    std::cout << "Computing out-of-domain TF-IDF." << std::endl;
    
    std::map<std::string, int> tmpIdIDF;
    std::map<std::string, int> tmpOodIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        ptrOodTf->operator[](ptrWords->operator[](i)) = 0;
        tmpIdIDF[ptrWords->operator[](i)] = 0;
        tmpOodIDF[ptrWords->operator[](i)] = 0;
        ptrOodTfIdf->operator[](ptrWords->operator[](i)) = 0.0;
    }
    
    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        std::vector<std::string> lWords;
        std::string line = ptrOOD->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            int found = (int)tmpOodIDF.count(lWords[j]);
            
            if (found == 1) {
                tmpOodIDF[lWords[j]] = 1;
                ptrOodTf->operator[](lWords[j]) = ptrOodTf->operator[](lWords[j]) + 1;
            }
        }
    }
    
    for (unsigned int i = 0; i < ptrID->getSize(); i++) {
        std::map<std::string, int> lineIDF;
        std::vector<std::string> lWords;
        std::string line = ptrID->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (std::map<std::string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
            int found = (int)tmpIdIDF.count(it->first);
            
            if (found == 1) {
                tmpIdIDF[it->first] = tmpIdIDF[it->first] + 1;
            }
        }
    }
    
    int nbDocs = ptrID->getSize() + 1;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        float IDF = 0.0;
        if (tmpIdIDF[ptrWords->operator[](i)] + tmpOodIDF[ptrWords->operator[](i)] != 0)
            IDF = log((float)nbDocs / (float)(tmpIdIDF[ptrWords->operator[](i)] + tmpOodIDF[ptrWords->operator[](i)]));
        ptrOodTfIdf->operator[](ptrWords->operator[](i)) = (float)ptrOodTf->operator[](ptrWords->operator[](i)) * (float)IDF;
    }
    
    /** @todo DEBUG, needs more testing */
    std::string outName = ptrOOD->getXenFile()->getPrefix() + ".tfidf";
    
    std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        out << ptrWords->operator[](i) << '\t' << ptrOodTf->operator[](ptrWords->operator[](i)) << " --- " << tmpOodIDF[ptrWords->operator[](i)] << " --- " << tmpIdIDF[ptrWords->operator[](i)] << " --> " << ptrOodTfIdf->operator[](ptrWords->operator[](i)) << std::endl;
    }
    
    out.close();
    
    std::cout << "Done computing out-of-domain TF-IDF." << std::endl;
}

void Similarity::buildIDVector() {
    std::cout << "Building in-domain vector." << std::endl;
    
    XenOption* opt = XenOption::getInstance();
    
    std::map<std::string, float> meanTFIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++)
        meanTFIDF[ptrWords->operator[](i)] = (float)(ptrIdTfIdf->operator[](ptrWords->operator[](i)) + ptrOodTfIdf->operator[](ptrWords->operator[](i))) / (float)2;
    
    std::multimap<float, std::string, std::greater<float> > meanTFIDFByScore = XenCommon::flip_map(meanTFIDF);
    
    int count = 0;
    float idFreqThres = (float)ptrID->getSize() / 300; // Threshold 0,33% of words is the same
    float oodFreqThres = (float)ptrOOD->getSize() / 300; // Threshold 0,33% of words is the same
    
    for (std::multimap<float, std::string, std::greater<float> >::iterator it = meanTFIDFByScore.begin(); count < opt->getVecSize(); ++it) {
        count++;
        
        int idTf = ptrIdTf->operator[](it->second);
        int oodTf = ptrOodTf->operator[](it->second);
        
        if (idTf > idFreqThres || oodTf > oodFreqThres) {
            count--;
        }
        else {
            ptrIdVecTfIdf->operator[](count - 1) = it->first;
            ptrVecWords->operator[](count - 1) = it->second;
        }
    }
    
    /** @todo DEBUG, needs more testing */
    std::string outName = ptrID->getXenFile()->getPrefix() + ".vec";
    
    std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
    
    for (int i = 0; i < opt->getVecSize(); i++) {
        out << ptrVecWords->operator[](i) << "\t\t" << ptrIdVecTfIdf->operator[](i) << std::endl;
    }
    
    out.close();
    
    std::cout << "Done building in-domain vector." << std::endl;
}

void Similarity::computeOutOfDomainIDF() {
    std::cout << "Computing out-of-domain IDF." << std::endl;
    
    int nbDocs = ptrOOD->getSize();
    std::map<std::string, int> tmpIDF;
    
    for (unsigned int i = 0; i < ptrVecWords->size(); i++) {
        tmpIDF[ptrVecWords->operator[](i)] = 0;
    }
    
    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        std::map<std::string, int> lineIDF;
        std::vector<std::string> lWords;
        std::string line = ptrOOD->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (std::map<std::string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
            int found = (int)tmpIDF.count(it->first);
            
            if (found == 1) {
                tmpIDF[it->first] = tmpIDF[it->first] + 1;
            }
        }
    }
    
    for (unsigned int i = 0; i < ptrVecWords->size(); i++) {
        float IDF = 0.0;
        if (tmpIDF[ptrVecWords->operator[](i)] != 0)
            IDF = log((float)nbDocs / (float)tmpIDF[ptrVecWords->operator[](i)]);
        ptrOodVecIdf->operator[](i) = IDF;
    }
    
    /** @todo DEBUG, needs more testing */
    std::string outName = ptrOOD->getXenFile()->getPrefix() + ".vec";
    
    std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
    
    for (unsigned int i = 0; i < ptrVecWords->size(); i++) {
        out << ptrVecWords->operator[](i) << "\t\t" << ptrOodVecIdf->operator[](i) << std::endl;
    }
    
    out.close();

    std::cout << "Done computing out-of-domain IDF." << std::endl;
}

void Similarity::computeSimilarity() {
    std::cout << "Computing similarity scores." << std::endl;

    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        std::map<std::string, int> tmpTF;
        
        for (unsigned int j = 0; j < ptrVecWords->size(); j++) {
            tmpTF[ptrVecWords->operator[](j)] = 0;
        }
        
        std::vector<std::string> lWords;
        std::string line = ptrOOD->getLine(i);
        
        boost::split(lWords, line, boost::is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            std::map<std::string, int>::const_iterator mit(tmpTF.find(lWords[j])), mend(tmpTF.end());
            
            if(mit != mend)
                tmpTF[lWords[j]] = tmpTF[lWords[j]] + 1;
        }
        
        float sumAiBi = 0.0;
        float sumAiSQ = 0.0;
        float sumBiSQ = 0.0;
        
        for (unsigned int j = 0; j < ptrVecWords->size(); j++) {
            float oodTfIdf = (float)tmpTF[ptrVecWords->operator[](j)] * (float)ptrOodVecIdf->operator[](j);
            float idTfIdf = ptrIdVecTfIdf->operator[](j);
            sumAiBi = sumAiBi + (idTfIdf * oodTfIdf);
            sumAiSQ = sumAiSQ + (idTfIdf * idTfIdf);
            sumBiSQ = sumBiSQ + (oodTfIdf * oodTfIdf);
        }
        
        float sim = sumAiBi / (sqrt(sumAiSQ) * sqrt(sumBiSQ));
        
        /** @bug Sometimes sim = NaN, so we need to protect against that */
        if (sim != sim)
            sim = 0.0;
        
        ptrOodSimilarity->operator[](i) = sim;
    }
    
    std::cout << "Done computing similarity scores." << std::endl;
}
