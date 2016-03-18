/**
 *  @file xenvocab.h
 *  @brief Class handling a XenC vocabulary
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

#ifndef XENVOCAB_H_
#define XENVOCAB_H_

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <vector>

#include "utils/common.h"
#include "corpus.h"
#include "xenresult.h"
#include "xenoption.h"

using namespace boost;
using namespace boost::filesystem;

/**
 *  @class XenVocab
 *  @brief Class handling a XenC vocabulary
 */
class XenVocab {
public:
    /**
     *  @fn XenVocab ()
     *  @brief Default constructor
     */
    XenVocab();
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenFile> ptrFile)
     *  @brief Initialization function from an already instantiated XenFile
     *
     *  @param ptrFile :    the vocabulary file
     */
    void initialize(boost::shared_ptr<XenFile> ptrFile);
    
    /**
     *  @fn void initialize (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Initialization function from a Corpus
     *
     *  @param ptrCorp :    the Corpus to extract the vocabulary from
     */
    void initialize(boost::shared_ptr<Corpus> ptrCorp);
    
    /**
     *  @fn void initialize (boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp)
     *  @brief Initialization function from two Corpus
     *
     *  @param ptrInCorp  :     the in-domain Corpus to extract the vocabulary from
     *  @param ptrOutCorp :     the out-of-domain Corpus to extract the vocabulary from
     */
    void initialize(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp);
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenResult> ptrXenRes)
     *  @brief Initialization function from a sorted result file
     *
     *  @param ptrXenRes :  the sorted result file to extract the vocabulary from
     */
    void initialize(boost::shared_ptr<XenResult> ptrXenRes);
    
    /**
     *  @fn ~XenVocab ()
     *  @brief Default destructor
     */
    ~XenVocab();

    /**
     *  @fn std::map<std::string, int> getXenVocab()
     *  @brief Accessor to the XenC vocabulary object
     *
     *  @return the XenC vocabulary object
     */
    std::map<std::string, int> getXenVocab() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getXenFile ()
     *  @brief Accessor to the vocabulary file
     *
     *  @return the vocabulary file
     */
    boost::shared_ptr<XenFile> getXenFile() const;
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the vocabulary text
     *
     *  @return the size of the vocabulary text
     */
    unsigned int getSize() const;
    
private:
    boost::shared_ptr<XenFile> ptrFile;     //!< Shared pointer on the vocabulary file
    std::map<std::string, int> voc;         //!< Map holding the XenC vocabulary

    /**
     *  @fn void writeVocab ()
     *  @brief Writes a vocabulary on disk
     */
    void writeVocab();

    /**
     *  @fn void makeVocab (boost::shared_ptr<XenFile> ptrFile)
     *  @brief Generates a vocabulary from a file

     *  @param ptrFile :    the file to generate the vocabulary from
     */
    void makeVocab(boost::shared_ptr<XenFile> ptrFile);

    /**
     *  @fn void makeVocab (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Generates a vocabulary from a Corpus
     *
     *  @param ptrCorp :    the Corpus to generate the vocabulary from
     */
    void makeVocab(boost::shared_ptr<Corpus> ptrCorp);
    
    /**
     *  @fn void makeVocab (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Generates a vocabulary from a Corpus
     *
     *  @param ptrCorp :    the Corpus to generate the vocabulary from
     */
    void makeVocab(boost::shared_ptr<Corpus> ptrInCorp, boost::shared_ptr<Corpus> ptrOutCorp);
    
    /**
     *  @fn void makeVocab (boost::shared_ptr<Corpus> ptrCorp)
     *  @brief Generates a vocabulary from a sorted result file
     *
     *  @param ptrXenRes :    the sorted result file to generate the vocabulary from
     */
    void makeVocab(boost::shared_ptr<XenResult> ptrXenRes);
};

#endif
