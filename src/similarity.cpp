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
 * $Id: similarity.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "similarity.h"

/*
 *  Default constructor
 */
Similarity::Similarity() {
    
}

/*
 *  Initialization from two Corpus and a Vocabulary
 */
void Similarity::initialize(shared_ptr<Corpus> ptrInCorp, shared_ptr<Corpus> ptrOutCorp, shared_ptr<XenVocab> ptrVocab) {
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

/*
 *  Destructor
 */
Similarity::~Similarity() {
    
}

//  ------
//  Public
//  ------

/*
 *  Returns the similarity score of the
 *  nth sentence in the out-of-domain corpus
 */
float Similarity::getSim(int n) {
    return ptrOodSimilarity->operator[](n);
}

/*
 *  Returns the size of the similarity scores list
 */
int Similarity::getSize() const {
    return (int)ptrOodSimilarity->size();
}

//  -------
//  Private
//  -------

/*
 *  Loads the words from the vocabulary
 */
void Similarity::loadWords() {
    cout << "Loading vocab words for similarity vector building." << endl;
    
    map<string, int> tmp = ptrVoc->getXenVocab();
    
    for (map<string,int>::iterator it = tmp.begin(); it != tmp.end(); ++it)
        ptrWords->push_back(it->first);
    
    cout << "Done loading vocab words for similarity vector building." << endl;
}

/*
 *  Computes the in-domain TF-IDF scores (both TF and IDF)
 *  In-domain is one doc, out-of-domain is one doc per sentence
 */
void Similarity::computeInDomainTFIDF() {
    cout << "Computing in-domain TF-IDF." << endl;
    
    map<string, int> tmpIdIDF;
    map<string, int> tmpOodIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        ptrIdTf->operator[](ptrWords->operator[](i)) = 0;
        tmpIdIDF[ptrWords->operator[](i)] = 0;
        tmpOodIDF[ptrWords->operator[](i)] = 0;
        ptrIdTfIdf->operator[](ptrWords->operator[](i)) = 0.0;
    }
    
    for (unsigned int i = 0; i < ptrID->getSize(); i++) {
        vector<string> lWords;
        string line = ptrID->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            int found = (int)tmpIdIDF.count(lWords[j]);
            
            if (found == 1) {
                tmpIdIDF[lWords[j]] = 1;
                ptrIdTf->operator[](lWords[j]) = ptrIdTf->operator[](lWords[j]) + 1;
            }
        }
    }

    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        map<string, int> lineIDF;
        vector<string> lWords;
        string line = ptrOOD->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (map<string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
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
    
    //DEBUG
    string outName = ptrID->getXenFile()->getPrefix() + ".tfidf";
    
    ofstream out(outName.c_str(), ios::out | ios::trunc);
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        out << ptrWords->operator[](i) << '\t' << ptrIdTf->operator[](ptrWords->operator[](i)) << " --- " << tmpIdIDF[ptrWords->operator[](i)] << " --- " << tmpOodIDF[ptrWords->operator[](i)] << " --> " << ptrIdTfIdf->operator[](ptrWords->operator[](i)) << endl;
    }
    
    out.close();

    cout << "Done computing in-domain TF-IDF." << endl;
}

/*
 *  Computes the out-of-domain TF-IDF scores (both TF and IDF)
 *  Out-of-domain is one doc, in-domain is one doc per sentence
 */
void Similarity::computeOutOfDomainTFIDF() {
    cout << "Computing out-of-domain TF-IDF." << endl;
    
    map<string, int> tmpIdIDF;
    map<string, int> tmpOodIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        ptrOodTf->operator[](ptrWords->operator[](i)) = 0;
        tmpIdIDF[ptrWords->operator[](i)] = 0;
        tmpOodIDF[ptrWords->operator[](i)] = 0;
        ptrOodTfIdf->operator[](ptrWords->operator[](i)) = 0.0;
    }
    
    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        vector<string> lWords;
        string line = ptrOOD->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            int found = (int)tmpOodIDF.count(lWords[j]);
            
            if (found == 1) {
                tmpOodIDF[lWords[j]] = 1;
                ptrOodTf->operator[](lWords[j]) = ptrOodTf->operator[](lWords[j]) + 1;
            }
        }
    }
    
    for (unsigned int i = 0; i < ptrID->getSize(); i++) {
        map<string, int> lineIDF;
        vector<string> lWords;
        string line = ptrID->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (map<string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
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
    
    //DEBUG
    string outName = ptrOOD->getXenFile()->getPrefix() + ".tfidf";
    
    ofstream out(outName.c_str(), ios::out | ios::trunc);
    
    for (unsigned int i = 0; i < ptrWords->size(); i++) {
        out << ptrWords->operator[](i) << '\t' << ptrOodTf->operator[](ptrWords->operator[](i)) << " --- " << tmpOodIDF[ptrWords->operator[](i)] << " --- " << tmpIdIDF[ptrWords->operator[](i)] << " --> " << ptrOodTfIdf->operator[](ptrWords->operator[](i)) << endl;
    }
    
    out.close();
    
    cout << "Done computing out-of-domain TF-IDF." << endl;
}

/*
 *  Builds the in-domain vector from TF-IDF scores
 *  Keeps the best matching words given the desired vector size
 */
void Similarity::buildIDVector() {
    cout << "Building in-domain vector." << endl;
    
    XenOption* opt = XenOption::getInstance();
    
    map<string, float> meanTFIDF;
    
    for (unsigned int i = 0; i < ptrWords->size(); i++)
        meanTFIDF[ptrWords->operator[](i)] = (float)(ptrIdTfIdf->operator[](ptrWords->operator[](i)) + ptrOodTfIdf->operator[](ptrWords->operator[](i))) / (float)2;
    
    multimap<float, string, greater<float> > meanTFIDFByScore = flip_map(meanTFIDF);
    
    int count = 0;
    float idFreqThres = (float)ptrID->getSize() / 300; // Threshold 0,33% of words is the same
    float oodFreqThres = (float)ptrOOD->getSize() / 300; // Threshold 0,33% of words is the same
    
    for (multimap<float, string, greater<float> >::iterator it = meanTFIDFByScore.begin(); count < opt->getVecSize(); ++it) {
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
    
    //DEBUG
    string outName = ptrID->getXenFile()->getPrefix() + ".vec";
    
    ofstream out(outName.c_str(), ios::out | ios::trunc);
    
    for (int i = 0; i < opt->getVecSize(); i++) {
        out << ptrVecWords->operator[](i) << "\t\t" << ptrIdVecTfIdf->operator[](i) << endl;
    }
    
    out.close();
    //
    
    cout << "Done building in-domain vector." << endl;
}

/*
 *  Computes the out-of-domain IDF only for the built vector
 */
void Similarity::computeOutOfDomainIDF() {
    cout << "Computing out-of-domain IDF." << endl;
    
    int nbDocs = ptrOOD->getSize();
    map<string, int> tmpIDF;
    
    for (unsigned int i = 0; i < ptrVecWords->size(); i++) {
        tmpIDF[ptrVecWords->operator[](i)] = 0;
    }
    
    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        map<string, int> lineIDF;
        vector<string> lWords;
        string line = ptrOOD->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            lineIDF[lWords[j]] = 0;
        }
        
        for (map<string, int>::iterator it = lineIDF.begin(); it != lineIDF.end(); ++it) {
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
    
    //DEBUG
    string outName = ptrOOD->getXenFile()->getPrefix() + ".vec";
    
    ofstream out(outName.c_str(), ios::out | ios::trunc);
    
    for (unsigned int i = 0; i < ptrVecWords->size(); i++) {
        out << ptrVecWords->operator[](i) << "\t\t" << ptrOodVecIdf->operator[](i) << endl;
    }
    
    out.close();

    cout << "Done computing out-of-domain IDF." << endl;
}

/*
 *  Computes the final similarity scores from the built vector
 */
void Similarity::computeSimilarity() {
    cout << "Computing similarity scores." << endl;

    for (unsigned int i = 0; i < ptrOOD->getSize(); i++) {
        map<string, int> tmpTF;
        
        for (unsigned int j = 0; j < ptrVecWords->size(); j++) {
            tmpTF[ptrVecWords->operator[](j)] = 0;
        }
        
        vector<string> lWords;
        string line = ptrOOD->getLine(i);
        
        split(lWords, line, is_any_of(" "));
        
        for (unsigned int j = 0; j < lWords.size(); j++) {
            map<string, int>::const_iterator mit(tmpTF.find(lWords[j])), mend(tmpTF.end());
            
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
        
        // Protect from NaN
        if (sim != sim)
            sim = 0.0;
        
        ptrOodSimilarity->operator[](i) = sim;
    }
    
    cout << "Done computing similarity scores." << endl;
}
