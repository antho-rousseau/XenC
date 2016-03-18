/**
 *  @file XenLMken.h
 *  @brief Class handling KenLM estimation, loading, querying...
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

#ifndef XENLMKEN_H_
#define XENLMKEN_H_

#define KENLM_MAX_ORDER 6

#include <boost/algorithm/string.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

#include "kenlm/lm/config.hh"
#include "kenlm/lm/model.hh"
#include "kenlm/lm/builder/output.hh"
#include "kenlm/lm/builder/pipeline.hh"
#include "kenlm/lm/lm_exception.hh"
#include "kenlm/lm/common/print.hh"
#include "kenlm/util/file.hh"
#include "kenlm/util/file_piece.hh"
#include "kenlm/util/usage.hh"

#include "corpus.h"
#include "xenvocab.h"
#include "xenoption.h"
#include "xenresult.h"


using namespace boost;
using namespace boost::algorithm;
using namespace boost::filesystem;
using namespace lm;
using namespace lm::builder;

struct TxtStats {
    float prob;
    float zeroprobs;
    uint64_t numwords;
    uint64_t numoov;
    uint64_t numsentences;
};

/**
 *  @class XenLMken
 *  @brief Class handling KenLM estimation, loading, querying...
 *
 *  This class is in charge of handling all KenLM-related operations.
 */
class XenLMken {
public:
    /**
     *  @fn XenLMken ()
     *  @brief Default constructor
     */
    XenLMken();
    
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
     *  @fn ~XenLMken ()
     *  @brief Default destructor
     */
    ~XenLMken();
    
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
     *  @fn std::string getFileName () const
     *  @brief Accessor to the language model file name
     *
     *  @return the language model file name
     */
    std::string getFileName() const;
    
    /**
     *  @fn TextStats getSentenceStats (std::string sent)
     *  @brief Computes the KenLM stats of a given sentence
     *
     *  @param sent :   the sentence to compute the stats from
     *  @return the computed KenLM stats
     */
    TxtStats getSentenceStats(std::string sent);
    
    /**
     *  @fn TextStats getDocumentStats (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Computes the KenLM stats of a Corpus at a document level
     *
     *  @param ptrCorp :    the Corpus to compute the stats from
     *  @return the computed document-level KenLM stats
     */
    TxtStats getDocumentStats(boost::shared_ptr<Corpus> c);
    
private:
    boost::shared_ptr<Corpus> ptrCorp;              //!< Shared pointer on the LM Corpus
    boost::shared_ptr<XenVocab> ptrVoc;             //!< Shared pointer on the LM Vocabulary (XenVocab)
    boost::shared_ptr<XenResult> ptrXR;             //!< Shared pointer on the LM XenResult file
    lm::ngram::Config config;
    boost::shared_ptr<lm::ngram::ProbingModel> ptrMdl;
    char* textFile;                                 //!< The Corpus file name on disk
    char* lmFile;                                   //!< The LM file name on disk
    unsigned order;                                 //!< The LM order
    std::string mempc;
    std::string temp;
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
