/**
 *  @file xenresult.h
 *  @brief Class handling a XenC sorted result file for evaluation/best point
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

#ifndef XENRESULT_H_
#define XENRESULT_H_

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "utils/common.h"
#include "xenfile.h"

using namespace boost;

class XenIO;    // Forward declaration

/**
 *  @class XenResult
 *  @brief Class handling a XenC sorted result file for evaluation/best point
 */
class XenResult {
public:
    /**
     *  @fn XenResult ()
     *  @brief Default constructor
     */
    XenResult();
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenFile> ptrFile)
     *  @brief Initialization function from an already instantiated XenFile
     *
     *  @param ptrFile :    the sorted result file
     */
    void initialize(boost::shared_ptr<XenFile> ptrFile);
    
    /**
     *  @fn ~XenResult ()
     *  @brief Default destructor
     */
    ~XenResult();
    
    /**
     *  @fn std::vector<std::string> getSortedText () const
     *  @brief Accessor to the sorted corpus text
     *
     *  @return the sorted corpus text
     */
    std::vector<std::string> getSortedText() const;
    
    /**
     *  @fn std::string getTextLine (int n) const;
     *  @brief Accessor to the nth line of the sorted corpus text
     *
     *  @param n :  the number of the text line to get
     *  @return the requested nth line of text
     */
    std::string getTextLine(int n);
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the sorted corpus text
     *
     *  @return the size of the sorted corpus text
     */
    unsigned int getSize() const;
    
    /**
     *  @fn boost::shared_ptr<XenFile> getXenFile ()
     *  @brief Accessor to the sorted result file
     *
     *  @return the sorted result file
     */
    boost::shared_ptr<XenFile> getXenFile() const;
    
private:
    boost::shared_ptr<XenFile> ptrResult;   //!< Shared pointer on the sorted result file
    std::vector<std::string> txt;           //!< Vector of strings holding the sorted corpus text
};

#endif
