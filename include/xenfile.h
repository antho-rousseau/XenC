/**
 *  @file xenfile.h
 *  @brief Class providing some basic functions around files
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

#ifndef XENFILE_H_
#define XENFILE_H_

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include "common.h"

using namespace boost;
using namespace boost::filesystem;

/**
 *  @class XenFile
 *  @brief Class providing some basic functions around files
 *
 *  This class handles the files used in XenC and provides some
 *  basic functionalities around them used widely in XenC.
 */
class XenFile {
public:
    /**
     *  @fn XenFile ()
     *  @brief Default constructor
     */
    XenFile();
    
    /**
     *  @fn void initialize (std::string name)
     *  @brief Initialization function from a string
     *
     *  @param name : the file to handle
     */
    void initialize(std::string name);
    
    /**
     *  @fn ~XenFile ()
     *  @brief Default destructor
     */
    ~XenFile();

    /**
     *  @fn std::string getFileName () const
     *  @brief Accessor to the file name
     *
     *  @return the file name (and only the file name)
     */
    std::string getFileName() const;
    
    /**
     *  @fn std::string getPrefix ()
     *  @brief Accessor to the prefix of the file name (before the dot)
     *
     *  @return the file name's prefix
     */
    std::string getPrefix();
    
    /**
     *  @fn std::string getExt ()
     *  @brief Accessor to the extension of the file name (after the dot)
     *
     *  @return the file name's extension
     */
    std::string getExt();
    
    /**
     *  @fn std::string getDirName () const
     *  @brief Accessor to the file's directory name
     *
     *  @return the file's directory name
     */
    std::string getDirName() const;
    
    /**
     *  @fn std::string getFullPath () const
     *  @brief Accessor to the file's full path
     *
     *  @return the file's full path
     */
    std::string getFullPath() const;
    
    /**
     *  @fn bool isGZ () const
     *  @brief Tries to guess if the file is a gzip or plain text
     *
     *  @return true if the file is gzipped
     */
    bool isGZ() const;
    
private:
    std::string fileName;   //!< The file's name
    std::string dirName;    //!< The file's directory
};

#endif
