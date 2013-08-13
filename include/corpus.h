/**
 *  @file corpus.h
 *  @brief Class handling corpus-related functionalities
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

#ifndef CORPUS_H_
#define CORPUS_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "utils/common.h"
#include "xenfile.h"

using namespace boost;

class XenIO;    // Forward declaration

/**
 *  @class Corpus
 *  @brief Corpus-related functionalities
 *
 *  This class handles the corpus used in XenC, providing means to get lines of text,
 *  size, language, token counts...
 */
class Corpus {
public:
    /**
     *  @fn Corpus ()
     *  @brief Default constructor
     */
    Corpus();
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenFile> ptrData, std::string lg)
     *  @brief Initialization function from an already instanciated XenFile
     *
     *  @param ptrData :    shared pointer on a XenFile representing the corpus on disk
     *  @param lg :         language of the corpus
     */
    void initialize(boost::shared_ptr<XenFile> ptrData, std::string lg);
    
    /**
     *  @fn void initialize (std::string filePath, std::string lg)
     *  @brief Initialization function from a string containing a valid path/file name
     *
     *  @param filePath :   string containing a valid path/file name
     *  @param lg :         language of the corpus
     */
    void initialize(std::string filePath, std::string lg);
    
    /**
     *  @fn ~Corpus ()
     *  @brief Default destructor
     */
    ~Corpus();
    
    /**
     *  @fn boost::shared_ptr<XenFile> getXenFile () const
     *  @brief Accessor to the XenFile associated to the Corpus
     *
     *  @return shared pointer to the XenFile
     */
    boost::shared_ptr<XenFile> getXenFile() const;
    
    /**
     *  @fn std::string getLine (int line)
     *  @brief Accessor to the lines of text from the Corpus
     *
     *  @param line : integer representing the line number
     *  @return string containing the text line
     */
    std::string getLine(int line);
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the Corpus
     *
     *  @return unsigned int representing the size
     */
    unsigned int getSize() const;
    
    /**
     *  @fn std::string getLang () const
     *  @brief Accessor to the language of the Corpus
     *
     *  @return string containing the language
     */
    std::string getLang() const;
    
    /**
     *  @fn bool getPrint (int line)
     *  @brief Accessor to the printing status of a line
     *
     *  @param line : integer representing the line number
     *  @return true if the line can be printed
     */
    bool getPrint(int line);
    
    /**
     *  @fn int getWC () const
     *  @brief Accessor to the number of tokens of the Corpus
     *
     *  @return integer representing the token count
     */
    int getWC() const;
    
    /**
     *  @fn void removeLine (int line)
     *  @brief Put the printing status of a line to false
     *
     *  @param line : integer representing the line number
     */
    void removeLine(int line);
    
private:
    std::string dir;        //!< String representing the Corpus directory
    std::string lang;       //!< String representing the Corpus language
    boost::shared_ptr<XenFile> ptrFile;     //!< Shared pointer on a XenFile wrapping the reference to the XenFile of the Corpus
    boost::shared_ptr<std::vector<std::string> > ptrText;     //!< Shared pointer on a vector of strings holding the Corpus text
    boost::shared_ptr<std::vector<int> > ptrPrint;      //!< Shared pointer on a vector of integers holding the printing status of the text
    int wc;                 //!< Integer representing the tokens count
    
    /**
     *  @fn int wordCount ()
     *  @brief Counts the tokens in the Corpus text
     *
     *  @return integer representing the token count
     */
    int wordCount();
    
    /**
     *  @fn void loadText ()
     *  @brief Loads the Corpus text into the vector
     */
    void loadText();
};

#endif
