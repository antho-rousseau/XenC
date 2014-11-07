/**
 *  @file ppl.h
 *  @brief Class handling the perplexity/cross-entropy computations
 *  @author Anthony Rousseau
 *  @version 1.2.0
 *  @date 19 August 2013
 */

/*  This file is part of the cross-entropy tool for data selection (XenC)
 *  aimed at speech recognition and statistical machine translation.
 *
 *  Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
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

#ifndef PPL_H_
#define PPL_H_

#include "utils/threadpool.hpp"
#include "corpus.h"
#include "phrasetable.h"
#include "XenLMsri.h"

using namespace boost::threadpool;

/**
 *  @fn void taskCalcPPL (int numLine, std::string line, boost::shared_ptr<std::vector<double> > ptrPPL, boost::shared_ptr<XenLMsri> ptrLM)
 *  @brief Thread-safe perplexity computation function
 *
 *  @param numLine :        integer to the line number to compute perplexity for
 *  @param line :           string to the text line to compute perplexity for
 *  @param ptrPPL :         shared pointer on the vector of doubles containing the perplexity scores
 *  @param ptrLM :          shared pointer on the language model to compute perplexity and cross-entropy from
 */
void taskCalcPPL(int numLine, std::string line, boost::shared_ptr<std::vector<double> > ptrPPL, boost::shared_ptr<XenLMsri> ptrLM);

/**
 *  @class PPL
 *  @brief Perplexity/Cross-entropy computations
 *
 *  This class handles the perplexity/cross-entropy computations in XenC.
 *  It uses threads extensively to compute scores simultaneously.
 */
class PPL {
public:
    /**
     *  @fn PPL ()
     *  @brief Default constructor
     */
    PPL();
    
    /**
     *  @fn void initialize (boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenLMsri> ptrLM)
     *  @brief Initialization function from a Corpus and a Language Model object
     *
     *  @param ptrCorp :    shared pointer on a Corpus to compute perplexity for
     *  @param ptrLM :      shared pointer on a XenLMsri object to compute perplexity from
     */
    void initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenLMsri> ptrLM);
    
    /**
     *  @fn void initialize (boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<XenLMsri> ptrLM, bool source)
     *  @brief Initialization function from a PhraseTable and a Language Model object
     *
     *  @param ptrPT :      shared pointer on a PhraseTable to compute perplexity for
     *  @param ptrLM :      shared pointer on a XenLMsri object to compute perplexity from
     *  @param source :     boolean indicating if we are on source (true) or target (false) side of the PhraseTable
     */
    void initialize(boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<XenLMsri> ptrLM, bool source);
    
    /**
     *  @fn ~PPL ()
     *  @brief Default destructor
     */
    ~PPL();
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the perplexity/cross-entropy vector
     *
     *  @return unsigned int representing the size
     */
    unsigned int getSize() const;
    
    /**
     *  @fn double getPPL (int n)
     *  @brief Accessor to the nth perplexity score
     *
     *  @param n :      integer indicating the position of the score to return
     *  @return double representing the nth perplexity score
     */
    double getPPL(int n);
    
    /**
     *  @fn double getXE (int n)
     *  @brief Accessor to the nth cross-entropy score
     *
     *  @param n :      integer indicating the position of the score to return
     *  @return double representing the nth cross-entropy score
     */
    double getXE(int n);
    
    /**
     *  @fn double getCorpPPL ()
     *  @brief Accessor to the document-level perplexity score
     *  @return double representing the document perplexity score
     */
    double getCorpPPL();
    
    /**
     *  @fn void calcPPLCorpus ()
     *  @brief Computes the perplexity of a Corpus sentence by sentence
     */
    void calcPPLCorpus();
    
    /**
     *  @fn void calcPPLPhraseTable ()
     *  @brief Computes the perplexity of a PhraseTable phrase by phrase
     */
    void calcPPLPhraseTable();

private:
    boost::shared_ptr<XenLMsri> ptrLM;                  //!< Shared pointer on the XenLMsri object figuring the language model
    boost::shared_ptr<Corpus> ptrCorp;                  //!< Shared pointer on the Corpus object to compute perplexity for
    boost::shared_ptr<PhraseTable> ptrPT;               //!< Shared pointer on the PhraseTable object to compute perplexity for
    bool source;                                        //!< Boolean indicating if we are working on source or target side (for the PhraseTable)
    boost::shared_ptr<std::vector<double> > ptrPPL;     //!< Shared pointer on a vector of doubles holding the computed perplexity scores

    /**
     *  @fn static double crossEntropy (double ppl)
     *  @brief Compute the cross-entropy score from a perplexity score: log(ppl)/log(2)
     *
     *  @param ppl :    perplexity to compute cross-entropy from
     *  @return the computed cross-entropy
     */
    static double crossEntropy(double ppl);
};

#endif
