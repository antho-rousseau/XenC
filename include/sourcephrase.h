/**
 *  @file sourcephrase.h
 *  @brief Class holding a merged source phrase and all associated data
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

#ifndef SOURCEPHRASE_H_
#define SOURCEPHRASE_H_

#include "score.h"

/**
 *  @class SourcePhrase
 *  @brief Class holding a merged source phrase and all associated data
 *
 *  This class holds a merged source phrase from a PhraseTable, along with
 *  target phrases, scores, alignments and counts.
 */
class SourcePhrase {
public:
    /**
     *  @fn SourcePhrase (std::string src)
     *  @brief Constructor from a string
     *
     *  @param src :    string representing the source phrase
     */
    SourcePhrase(std::string src);
    
    /**
     *  @fn ~SourcePhrase ()
     *  @brief Default destructor
     */
    ~SourcePhrase();
    
    /**
     *  @fn std::string getSource () const
     *  @brief Accessor to the source phrase
     *
     *  @return the source phrase
     */
    std::string getSource() const;
    
    /**
     *  @fn unsigned int getTargetSize () const
     *  @brief Accessor to the size of the target phrases associated to the source phrase
     *
     *  @return the size of the target phrases vector
     */
    unsigned int getTargetSize() const;
    
    /**
     *  @fn boost::shared_ptr<Score> getScoresXE ()
     *  @brief Accessor to the vector of cross-entropy scores for the target phrases
     *
     *  @return a shared pointer on a Score object containing the scores
     */
    boost::shared_ptr<Score> getScoresXE() const;
    
    /**
     *  @fn void addTarget (std::string s)
     *  @brief Associates a target phrase to the source phrase
     *
     *  @param s :      the target phrase to add to the source phrase
     */
    void addTarget(std::string s);
    
    /**
     *  @fn void addScores (std::string s)
     *  @brief Associates a phrase table scores sequence to the source phrase
     *
     *  @param s :      the scores sequence to add to the source phrase
     */
    void addScores(std::string s);
    
    /**
     *  @fn void addAlignments (std::string s)
     *  @brief Associates alignments to the source phrase
     *
     *  @param s :      the alignments to add to the source phrase
     */
    void addAlignments(std::string s);
    
    /**
     *  @fn void addCounts (std::string s)
     *  @brief Associates counts to the source phrase
     *
     *  @param s :      the counts to add to the source phrase
     */
    void addCounts(std::string s);
    
private:
    std::string source;     //!< String representing the source phrase
    boost::shared_ptr<std::vector<std::string> > ptrTargets;        //!< Shared pointer on a vector of string representing the target phrases
    boost::shared_ptr<std::vector<std::string> > ptrScores;         //!< Shared pointer on a vector of string representing the scores
    boost::shared_ptr<std::vector<std::string> > ptrAlignments;     //!< Shared pointer on a vector of string representing the alignments
    boost::shared_ptr<std::vector<std::string> > ptrCounts;         //!< Shared pointer on a vector of string representing the counts
    boost::shared_ptr<Score> ptrScoresXE;       //!< Shared pointer on a Score object representing the cross-entropy scores for the target phrases
};

#endif
