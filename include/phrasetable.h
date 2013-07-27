/**
 *  @file phrasetable.h
 *  @brief Class handling phrase-table related functionalities
 *  @author Anthony Rousseau
 *  @version 1.0.0
 *  @date 27 July 2013
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

#ifndef PHRASETABLE_H_
#define PHRASETABLE_H_

#include "utils/common.h"

#include "xenoption.h"
#include "sourcephrase.h"

class XenIO;    // Forward declaration

/**
 *  @class PhraseTable
 *  @brief Class handling phrase-table related functionalities
 *
 *  This class handles all phrase-table related functionalities
 *  and is used in the fourth filtering mode
 */
class PhraseTable {
public:
    /**
     *  @fn PhraseTable ()
     *  @brief Default constructor
     */
    PhraseTable();
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenFile> ptrData)
     *  @brief Initialization function from an already instanciated XenFile
     *
     *  @param ptrData :    shared pointer on a XenFile representing the PhraseTable on disk
     */
    void initialize(boost::shared_ptr<XenFile> ptrData);
    
    /**
     *  @fn ~PhraseTable ()
     *  @brief Default destructor
     */
    ~PhraseTable();

    /**
     *  @fn boost::shared_ptr<XenFile> getXenFile ()
     *  @brief Accessor to the XenFile associated to the PhraseTable
     *
     *  @return shared pointer to the XenFile
     */
    boost::shared_ptr<XenFile> getXenFile() const;
    
    /**
     *  @fn std::string getSource (int n)
     *  @brief Accessor to the nth source phrase
     *
     *  @param n : integer representing the phrase number
     *  @return string containing the source phrase
     */
    std::string getSource(int n);
    
    /**
     *  @fn std::string getTarget (int n)
     *  @brief Accessor to the nth target phrase
     *
     *  @param n : integer representing the phrase number
     *  @return string containing the target phrase
     */
    std::string getTarget(int n);
    
    /**
     *  @fn std::string getScores (int n)
     *  @brief Accessor to the nth scores for the source/target phrase pair
     *
     *  @param n : integer representing the phrase number
     *  @return string containing the scores
     */
    std::string getScores(int n);
    
    /**
     *  @fn std::string getAlignment (int n)
     *  @brief Accessor to the nth alignments for the source/target phrase pair
     *
     *  @param n : integer representing the phrase number
     *  @return string containing the alignment
     */
    std::string getAlignment(int n);
    
    /**
     *  @fn std::string getCounts (int n)
     *  @brief Accessor to the nth counts for the source/target phrase pair
     *
     *  @param n : integer representing the phrase number
     *  @return string containing the counts
     */
    std::string getCounts(int ph);
    
    /**
     *  @fn std::vector<SourcePhrase> getSrcPhrases ()
     *  @brief Accessor to the vector of merged source phrases
     *
     *  @return vector of merged SourcePhrase
     */
    std::vector<SourcePhrase> getSrcPhrases();
    
    /**
     *  @fn void setSrcPhrases (std::vector<SourcePhrase> vSP)
     *  @brief Mutator to the vector of merged source phrases
     *
     *  @param vSP :    vector of SourcePhrase
     */
    void setSrcPhrases(std::vector<SourcePhrase> vSP);
    
    /**
     *  @fn unsigned int getSize ()
     *  @brief Accessor to the size of the PhraseTable
     *
     *  @return unsigned int representing the size
     */
    unsigned int getSize() const;
    
private:
    boost::shared_ptr<XenFile> ptrFile;     //!< Shared pointer on the XenFile figuring the phrase-table
    boost::shared_ptr<std::vector<std::string> > ptrPhPairs;    //!< Shared pointer on a vector of strings containing all the phrase pairs
    std::string dlm;    //!< Delimitor for the phrase table (usually "|||")
    XenCommon::Splitter lineSpl;    //!< Multi-character splitter object
    unsigned int size;  //!< Size of the PhraseTable
    boost::shared_ptr<std::vector<SourcePhrase> > ptrSrcPhrases;    //!< Shared pointer on a vector of SourcePhrase containing all the merged source phrases
    
    /**
     *  @fn void loadTable ()
     *  @brief Loads the phrase-table lines into the vector
     */
    void loadTable();
    
    /**
     *  @fn void mergePhrasesBySource ()
     *  @brief Merges all the phrase pairs by source phrase
     */
    void mergePhrasesBySource();
};

#endif
