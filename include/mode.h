/**
 *  @file mode.h
 *  @brief Abstract class defining the filtering modes architecture
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

#ifndef MODE_H_
#define MODE_H_

#include "corpus.h"
#include "utils/common.h"

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

/**
 *  @class Mode
 *  @brief Filtering modes interface
 *
 *  This class takes the role of an interface to the various XenC filtering modes.
 */
class Mode {
public:
    /**
     *  @fn virtual int launch ()
     *  @brief Virtual function in charge of launching the implemented mode
     */
    virtual int launch() = 0;
    
    /**
     *  @fn virtual ~Mode ()
     *  @brief Pure virtual destructor
     */
    virtual ~Mode() = 0;
    
protected:
    /**
     *  @fn static void findSampleSize (boost::shared_ptr<Corpus> idCorp, boost::shared_ptr<Corpus> oodCorp)
     *  @brief Finds the optimal sample size for the OOD Corpus 
     *
     *  @param idCorp :     in-domain Corpus
     *  @param oodCorp :    out-of-domain Corpus
     *  @return size of the required Corpus sample in percentage of the whole one
     */
    static int findSampleSize(boost::shared_ptr<Corpus> idCorp, boost::shared_ptr<Corpus> oodCorp);
    
    /**
     *  @fn static Corpus extractSample (boost::shared_ptr<Corpus> ptrCorp, int sSize, bool mean)
     *  @brief Extracts a random sample from a give Corpus
     *
     *  @param ptrCorp :    Corpus from which the sample should be extracted
     *  @param sSize :      size of the sample to extract
     *  @param mean :       true if we are in "mean" mode (not the same Corpus filename)
     *  @return extracted Corpus sample
     */
    static Corpus extractSample(boost::shared_ptr<Corpus> ptrCorp, int sSize, bool mean);
};

#endif
