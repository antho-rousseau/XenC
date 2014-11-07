/**
 *  @file xenoption.h
 *  @brief Singleton class handling XenC options accessors/mutators
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

#ifndef XENOPTION_H_
#define XENOPTION_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "utils/common.h"
#include "xenfile.h"

using namespace boost;

/**
 *  @class XenOption
 *  @brief Singleton class handling XenC options accessors/mutators
 */
class XenOption {
public:
    /**
     *  @fn static XenOption* getInstance ()
     *  @brief Accessor to the instance of the singleton XenOption object
     *
     *  @return the XenOption unique instance
     */
    static XenOption* getInstance();
    
    /**
     *  @fn static XenOption* getInstance (LPOptions opt)
     *  @brief Accessor to the instance of the singleton XenOption object
     *
     *  @param opt :    the LPOptions struct to build the XenOption object from
     *  @return the XenOption unique instance
     */
    static XenOption* getInstance(LPOptions opt);
    
    /**
     *  @fn static void deleteInstance ()
     *  @brief Deletes the unique instance of the XenOption singleton
     */
    static void deleteInstance();

    /**
     *  @fn std::string getSLang () const
     *  @brief Accessor to the source language
     *
     *  @return the source language
     */
    std::string getSLang() const;
    
    /**
     *  @fn std::string getTLang () const
     *  @brief Accessor to the target language
     *
     *  @return the target language
     */
	std::string getTLang() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInSData ()
     *  @brief Accessor to the source language in-domain data file
     *
     *  @return the source language in-domain data file
     */
	boost::shared_ptr<XenFile> getInSData() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutSData ()
     *  @brief Accessor to the source language out-of-domain data file
     *
     *  @return the source language out-of-domain data file
     */
	boost::shared_ptr<XenFile> getOutSData() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInTData ()
     *  @brief Accessor to the target language in-domain data file
     *
     *  @return the target language in-domain data file
     */
    boost::shared_ptr<XenFile> getInTData() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutTData ()
     *  @brief Accessor to the target language out-of-domain data file
     *
     *  @return the target language out-of-domain data file
     */
    boost::shared_ptr<XenFile> getOutTData() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInSStem ()
     *  @brief Accessor to the source language in-domain stem data file
     *
     *  @return the source language in-domain stem data file
     */
    boost::shared_ptr<XenFile> getInSStem() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutSStem ()
     *  @brief Accessor to the source language out-of-domain stem data file
     *
     *  @return the source language out-of-domain stem data file
     */
	boost::shared_ptr<XenFile> getOutSStem() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInTStem ()
     *  @brief Accessor to the target language in-domain stem data file
     *
     *  @return the target language in-domain stem data file
     */
    boost::shared_ptr<XenFile> getInTStem() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutTStem ()
     *  @brief Accessor to the target language out-of-domain stem data file
     *
     *  @return the target language out-of-domain stem data file
     */
    boost::shared_ptr<XenFile> getOutTStem() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInPTable ()
     *  @brief Accessor to the in-domain phrase-table file
     *
     *  @return the in-domain phrase-table file
     */
    boost::shared_ptr<XenFile> getInPTable() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutPTable ()
     *  @brief Accessor to the out-of-domain phrase-table file
     *
     *  @return the out-of-domain phrase-table file
     */
    boost::shared_ptr<XenFile> getOutPTable() const;
    
    /**
     *  @fn bool getMono () const
     *  @brief Accessor to the monolingual or bilingual execution state
     *
     *  @return true if we work on monolingual data
     */
    bool getMono() const;
    
    /**
     *  @fn int getMode () const
     *  @brief Accessor to the filtering mode
     *
     *  @return the filtering mode
     */
	int getMode() const;
    
    /**
     *  @fn bool getMean () const
     *  @brief Accessor to the mean execution state
     *
     *  @return true if we compute out-of-domain scores with mean of 3 LMs
     */
    bool getMean() const;
    
    /**
     *  @fn bool getSim () const
     *  @brief Accessor to the similarity measures execution state
     *
     *  @return true if we compute similarity measures
     */
    bool getSim() const;
    
    /**
     *  @fn bool getSimOnly () const
     *  @brief Accessor to the similarity measures ONLY execution state
     *
     *  @return true if we compute similarity measures ONLY
     */
    bool getSimOnly() const;
    
    /**
     *  @fn int getVecSize () const
     *  @brief Accessor to the similarity measures vector size
     *
     *  @return the similarity measures vector size
     */
    int getVecSize() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getSVocab ()
     *  @brief Accessor to the source language vocabulary file
     *
     *  @return the source language vocabulary file
     */
	boost::shared_ptr<XenFile> getSVocab() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getTVocab ()
     *  @brief Accessor to the target language vocabulary file
     *
     *  @return the target language vocabulary file
     */
	boost::shared_ptr<XenFile> getTVocab() const;
    
    /**
     *  @fn bool getFullVocab () const
     *  @brief Accessor to the global vocabulary execution state
     *
     *  @return true if we use a global vocabulary instead of only in-domain
     */
    bool getFullVocab() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInSLM ()
     *  @brief Accessor to the source language in-domain language model file
     *
     *  @return the source language in-domain language model file
     */
	boost::shared_ptr<XenFile> getInSLM() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutSLM ()
     *  @brief Accessor to the source language out-of-domain language model file
     *
     *  @return the source language out-of-domain language model file
     */
	boost::shared_ptr<XenFile> getOutSLM() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getInTLM ()
     *  @brief Accessor to the target language in-domain language model file
     *
     *  @return the target language in-domain language model file
     */
	boost::shared_ptr<XenFile> getInTLM() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getOutTLM ()
     *  @brief Accessor to the target language out-of-domain language model file
     *
     *  @return the target language out-of-domain language model file
     */
	boost::shared_ptr<XenFile> getOutTLM() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getWFile ()
     *  @brief Accessor to the weights file
     *
     *  @return the weights file
     */
	boost::shared_ptr<XenFile> getWFile() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getDev ()
     *  @brief Accessor to the development corpus file
     *
     *  @return the development corpus file
     */
	boost::shared_ptr<XenFile> getDev() const;
    
    /**
     *  @fn int getOrder () const
     *  @brief Accessor to the order for language models estimation
     *
     *  @return the order for language models estimation
     */
	int getOrder() const;
    
    /**
     *  @fn int getDiscount () const
     *  @brief Accessor to the discounting method for language models estimation
     *
     *  @return the discounting method for language models estimation
     */
    int getDiscount() const;
    
    /**
     *  @fn bool getToLower () const
     *  @brief Accessor to the flag for lower case vocabulary mapping in LM estimation
     *
     *  @return true if lower case vocabulary mapping is requested
     */
    bool getToLower() const;
    
    /**
     *  @fn bool noUnkIsWord () const
     *  @brief Accessor to the flag for NOT considering <unk> as a word in LM estimation
     *
     *  @return true if <unk> is NOT considered as a word
     */
    bool noUnkIsWord() const;
    
    /**
     *  @fn int getBinLM () const
     *  @brief Accessor to the estimated LMs output format
     *
     *  @return the estimated LMs output format
     */
    int getBinLM() const;
    
    /**
     *  @fn int getSampleSize () const
     *  @brief Accessor to the out-of-domain Corpus current sample size
     *
     *  @return the out-of-domain Corpus current sample size
     */
	int getSampleSize() const;
    
    /**
     *  @fn bool getLog () const
     *  @brief Accessor to the log-domain state of values in the weights file
     *
     *  @return true if values in the weights file are in the log-domain 
     */
    bool getLog() const;
    
    /**
     *  @fn bool getRev () const
     *  @brief Accessor to the reversed filtered output state
     *
     *  @return true if we output a descending order filtered file
     */
	bool getRev() const;
    
    /**
     *  @fn bool getInv () const
     *  @brief Accessor to the inverted filtered output state
     *
     *  @return true if we output inverted scores (1 - score)
     */
    bool getInv() const;
    
    /**
     *  @fn bool getStem () const
     *  @brief Accessor to the stem mode execution state
     *
     *  @return true if we work with stem Corpus too
     */
    bool getStem() const;
    
    /**
     *  @fn bool getLocal () const
     *  @brief Accessor to the local score for phrase-table scoring execution state
     *
     *  @return true if we also compute local scores in phrase-table scoring mode
     */
    bool getLocal() const;
    
    /**
     *  @fn bool getEval () const
     *  @brief Accessor to the evaluation execution state
     *
     *  @return true if we have to perform an evaluation
     */
	bool getEval() const;
    
    /**
     *  @fn bool getBp () const
     *  @brief Accessor to the best point execution state
     *
     *  @return true if we have to perform a best point search
     */
	bool getBp() const;
    
    /**
     *  @fn int getStep () const
     *  @brief Accessor to the step size for evaluation/best point
     *
     *  @return the step size for evaluation/best point
     */
	int getStep() const;
    
    /**
     *  @fn std::string getOutName () const
     *  @brief Accessor to the output name for the filtered files
     *
     *  @return the output name for the filtered files
     */
    std::string getOutName() const;
    
    /**
     *  @fn std::string getName () const
     *  @brief Accessor to the program name
     *
     *  @return the program name
     */
    std::string getName() const;
    
    /**
     *  @fn int getThreads () const
     *  @brief Accessor to the requested number of threads
     *
     *  @return the requested number of threads
     */
    int getThreads() const;
    
    /**
     *  @fn bool getSortOnly () const
     *  @brief Accessor to whether we output the scored file
     *
     *  @return true if we only need to output the sorted file
     */
    bool getSortOnly() const;
    
    /**
     *  @fn void setSampleSize (int size)
     *  @brief Mutator to the out-of-domain sample size
     *
     *  @param size :   the out-of-domain sample size
     */
    void setSampleSize(int size);
    
    /**
     *  @fn void setStep (int step)
     *  @brief Mutator to the evaluation/best point step size
     *
     *  @param step :   the evaluation/best point step size
     */
    void setStep(int step);
    
private:
    /**
     *  @fn XenOption ()
     *  @brief Singleton private default constructor
     */
    XenOption();
    
    /**
     *  @fn XenOption (LPOptions opt)
     *  @brief Singleton private constructor from an options struct
     */
    XenOption(LPOptions opt);
    
    /**
     *  @fn ~XenOption ()
     *  @brief Singleton private default destructor
     */
    ~XenOption();

    LPOptions opt;                      //!< The options structure
    static XenOption* _instance;        //!< The Singleton unique instance
    boost::shared_ptr<XenFile> inSD;    //!< Shared pointer on the source language in-domain data file
	boost::shared_ptr<XenFile> outSD;   //!< Shared pointer on the source language out-of-domain data file
    boost::shared_ptr<XenFile> inTD;    //!< Shared pointer on the target language in-domain data file
    boost::shared_ptr<XenFile> outTD;   //!< Shared pointer on the target language out-of-domain data file
    boost::shared_ptr<XenFile> inSS;    //!< Shared pointer on the source language in-domain stem data file
	boost::shared_ptr<XenFile> outSS;   //!< Shared pointer on the source language out-of-domain stem data file
    boost::shared_ptr<XenFile> inTS;    //!< Shared pointer on the target language in-domain stem data file
    boost::shared_ptr<XenFile> outTS;   //!< Shared pointer on the target language out-of-domain stem data file
    boost::shared_ptr<XenFile> inPT;    //!< Shared pointer on the in-domain phrase-table file
    boost::shared_ptr<XenFile> outPT;   //!< Shared pointer on the out-of-domain phrase-table file
    boost::shared_ptr<XenFile> sVoc;    //!< Shared pointer on the source language vocabulary file
	boost::shared_ptr<XenFile> tVoc;    //!< Shared pointer on the target language vocabulary file
	boost::shared_ptr<XenFile> inSLM;   //!< Shared pointer on the source language in-domain language model file
	boost::shared_ptr<XenFile> outSLM;  //!< Shared pointer on the source language out-of-domain language model file
	boost::shared_ptr<XenFile> inTLM;   //!< Shared pointer on the target language in-domain language model file
	boost::shared_ptr<XenFile> outTLM;  //!< Shared pointer on the target language out-of-domain language model file
	boost::shared_ptr<XenFile> wFile;   //!< Shared pointer on the weights file
	boost::shared_ptr<XenFile> dev;     //!< Shared pointer on the development corpus file
};

#endif
