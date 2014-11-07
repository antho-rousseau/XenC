/**
 *  @file XenLMsri.h
 *  @brief Class handling SRI LM estimation, loading, querying...
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

#ifndef XENLMSRI_H_
#define XENLMSRI_H_

#include <boost/algorithm/string.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "Ngram.h"  //!< Include SRILM Ngram.h

#include "corpus.h"
#include "xenvocab.h"
#include "xenoption.h"
#include "xenresult.h"

/** @def MAX_ORDER 9 */
#define MAX_ORDER 9                 //!< Maximum LM order

/** @def MAX_WORDS 16384 */
#define MAX_WORDS 16384             //!< Maximum tokens per line of text

/** @def MAX_CHARS MAX_WORDS * 16 */
#define MAX_CHARS MAX_WORDS * 16    //!< Maximum characters per line of text

using namespace boost;
using namespace boost::algorithm;
using namespace boost::filesystem;

/**
 *  @class XenLMsri
 *  @brief Class handling SRI LM estimation, loading, querying...
 *
 *  This class is in charge of handling all SRILM-related operations.
 *  Be aware that due to some memory leaks in SRILM, memory usage in XenC
 *  can grow up very fast.
 */
class XenLMsri {
public:
    /**
     *  @fn XenLMsri ()
     *  @brief Default constructor
     */
    XenLMsri();
    
    /**
     *  @fn void initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc)
     *  @brief Initialization function from a Corpus and a vocabulary (XenVocab)
     *
     *  @param ptrCorp :    the corpus to estimate the LM from
     *  @param ptrVoc :     the vocabulary used to estimate the LM
     */
    void initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc);
    
    /**
     *  @fn void initialize(boost::shared_ptr<XenFile> ptrFile, boost::shared_ptr<XenVocab> ptrVoc)
     *  @brief Initialization function from an already existing LM file and a vocabulary (XenVocab)
     *
     *  @param ptrFile :    the LM file to load
     *  @param ptrVoc :     the vocabulary used to estimate the LM
     */
    void initialize(boost::shared_ptr<XenFile> ptrFile, boost::shared_ptr<XenVocab> ptrVoc);
    
    /**
     *  @fn void initialize(boost::shared_ptr<XenResult> ptrXenRes, boost::shared_ptr<XenVocab> ptrVoc, int pc, std::string name = "")
     *  @brief Initialization function from a XenC filtering result file and a vocabulary
     *
     *  @param ptrXenRes :  the XenC filtering result file
     *  @param ptrVoc :     the vocabulary used to estimate the LM
     *  @param pc :         the percentage of the corpus in ptrXenRes to use
     *  @param name :       optionnal file name of the LM
     */
    void initialize(boost::shared_ptr<XenResult> ptrXenRes, boost::shared_ptr<XenVocab> ptrVoc, int pc, std::string name = "");
    
    /**
     *  @fn ~XenLMsri ()
     *  @brief Default destructor
     */
    ~XenLMsri();
    
    /**
     *  @fn int createLM ()
     *  @brief Estimates a language model based on the provided data
     *
     *  @return 0 if all goes well
     */
    int createLM();
    
    /**
     *  @fn int loadLM ()
     *  @brief Loads a language model from a provided file name
     *
     *  @return 0 if all goes well
     */
    int loadLM();
    
    /**
     *  @fn int writeLM ()
     *  @brief Writes an (arpa or binary) estimated language model on disk
     *
     *  @return 0 if all goes well
     */
    int writeLM();
    
    /**
     *  @fn std::string getFileName () const
     *  @brief Accessor to the language model file name
     *
     *  @return the language model file name
     */
    std::string getFileName() const;
    
    /**
     *  @fn TextStats getSentenceStats (std::string sent)
     *  @brief Computes the SRILM stats of a given sentence
     *
     *  @param sent :   the sentence to compute the stats from
     *  @return the computed SRILM stats
     */
    TextStats getSentenceStats(std::string sent);
    
    /**
     *  @fn TextStats getDocumentStats (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Computes the SRILM stats of a Corpus at a document level
     *
     *  @param ptrCorp :    the Corpus to compute the stats from
     *  @return the computed document-level SRILM stats
     */
    TextStats getDocumentStats(boost::shared_ptr<Corpus> ptrCorp);
    
private:
    boost::shared_ptr<Corpus> ptrCorp;              //!< Shared pointer on the LM Corpus
    boost::shared_ptr<XenVocab> ptrVoc;             //!< Shared pointer on the LM Vocabulary (XenVocab)
    boost::shared_ptr<Vocab> ptrVocab;              //!< Shared pointer on the LM Vocabulary (SRILM Vocab)
    boost::shared_ptr<XenResult> ptrXR;             //!< Shared pointer on the LM XenResult file
    boost::shared_ptr<NgramStats> ptrNStats;        //!< Shared pointer on the LM NgramStats
    boost::shared_array<Discount*> ptrDiscounts;    //!< Shared array on the LM Discounts per order
    boost::shared_ptr<Ngram> ptrLM;                 //!< Shared pointer on the SRI LM Ngram structure
    char* textFile;                                 //!< The Corpus file name on disk
    char* lmFile;                                   //!< The LM file name on disk
    unsigned order;                                 //!< The LM order
    int discount;                                   //!< The LM discounting method
    unsigned gtmin[MAX_ORDER + 1];                  //!< The LM cut-off min array (for SRILM computation)
    unsigned gtmax[MAX_ORDER + 1];                  //!< The LM cut-off max array (for SRILM computation)
    int writeBinaryLM;                              //!< The LM output format (arpa or binary)
    int pc;                                         //!< The XenResult percentage to take

    /**
     *  @fn std::string makeLMname (XenOption* opt)
     *  @brief Determines the name of the output LM given the options parameters
     *
     *  @param opt :    the options passed to the program
     *  @return the output name of the language model
     */
    std::string makeLMname(XenOption* opt);
};

#endif
