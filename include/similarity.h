/**
 *  @file similarity.h
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

#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <boost/algorithm/string.hpp>   //!< For boost::split

#include "utils/common.h"

#include "corpus.h"
#include "xenvocab.h"
#include "xenoption.h"

using namespace boost;

typedef std::map<int, float> SimMap;    //!< Map of integers as keys and floats as values to represent the similarity measures by sentence number

/**
 *  @class Similarity
 *  @brief Class taking care of all the similarity measure computations
 *
 *  This class computes similarity scores between two Corpus given a vocabulary.
 *  It determines the optimal vector for both Corpus, and uses it for similarity computation.
 *  WARNING: this feature is still experimental
 */
class Similarity {
public:
    /**
     *  @fn Similarity ()
     *  @brief Default constructor
     */
    Similarity();
    
    /**
     *  @fn void initialize (boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp, boost::shared_ptr<XenVocab> ptrVocab)
     *  @brief Initialization function from two Corpus (in and out-of-domain) and a vocabulary (XenVocab)
     *
     *  @param ptrInCorp :      shared pointer on the in-domain Corpus
     *  @param ptrOutCorp :     shared pointer on the out-of-domain Corpus
     *  @param ptrVocab :       shared pointer on the common XenVocab (usually the in-domain one)
     */
    void initialize(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp, boost::shared_ptr<XenVocab> ptrVocab);
    
    /**
     *  @fn ~Similarity ()
     *  @brief Default destructor
     */
    ~Similarity();
    
    /**
     *  @fn float getSim (int n)
     *  @brief Accessor to the nth sentence similarity measure
     *
     *  @param n :      integer representing the number of the sentence
     *  @return float representing the similarity measure of the nth sentence
     */
    float getSim(int n);
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the similarity map
     *
     *  @return unsigned int representing the size
     */
    unsigned int getSize() const;
    
private:
    boost::shared_ptr<XenVocab> ptrVoc;                                //!< Shared pointer on the XenVocab
    boost::shared_ptr<Corpus> ptrID;                                   //!< Shared pointer on the in-domain Corpus
    boost::shared_ptr<Corpus> ptrOOD;                                  //!< Shared pointer on the out-of-domain Corpus
    boost::shared_ptr<std::map<std::string, float> > ptrIdTfIdf;       //!< Shared pointer on a map of strings/floats representing the in-domain TF-IDF
    boost::shared_ptr<std::map<std::string, float> > ptrOodTfIdf;      //!< Shared pointer on a map of strings/floats representing the out-of-domain TF-IDF
    boost::shared_ptr<std::vector<std::string> > ptrWords;             //!< Shared pointer on a vector of strings holding the vocabulary words
    boost::shared_ptr<std::map<std::string, int> > ptrIdTf;            //!< Shared pointer on a map of strings/integers representing the in-domain TF
    boost::shared_ptr<std::map<std::string, int> > ptrOodTf;           //!< Shared pointer on a map of strings/integers representing the out-of-domain TF
    boost::shared_ptr<std::vector<std::string> > ptrVecWords;          //!< Shared pointer on a vector of strings holding the similarity vector words
    boost::shared_ptr<std::vector<float> > ptrIdVecTfIdf;              //!< Shared pointer on a vector of floats holding the in-domain TF-IDF for the sim vector
    boost::shared_ptr<std::vector<float> > ptrOodVecIdf;               //!< Shared pointer on a vector of floats holding the out-of-domain IDF for the sim vector
    boost::shared_ptr<SimMap> ptrOodSimilarity;                        //!< Shared pointer on a SimMap containing the out-of-domain similarity measures
    
    /**
     *  @fn void loadWords ()
     *  @brief Loads the XenVocab vocabulary words
     */
    void loadWords();
    
    /**
     *  @fn void computeInDomainTFIDF ()
     *  @brief Computes the in-domain term frequency/inverse document frequency
     */
    void computeInDomainTFIDF();
    
    /**
     *  @fn void computeOutOfDomainTFIDF ()
     *  @brief Computes the out-of-domain term frequency/inverse document frequency
     */
    void computeOutOfDomainTFIDF();
    
    /**
     *  @fn void buildIDVector ()
     *  @brief Builds the vector of words for the similarity computation
     */
    void buildIDVector();
    
    /**
     *  @fn void computeOutOfDomainIDF ()
     *  @brief Computes the out-of-domain inverse document frequency based on the word vector
     */
    void computeOutOfDomainIDF();
    
    /**
     *  @fn void computeSimilarity ()
     *  @brief Computes the final out-of-domain similarity measures
     */
    void computeSimilarity();
};

#endif
