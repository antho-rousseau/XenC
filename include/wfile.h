/**
 *  @file wfile.h
 *  @brief Class handling a file with values intended at weighting XenC scores
 *  @author Anthony Rousseau
 *  @version 1.2.0
 *  @date 19 August 2013
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

#ifndef WFILE_H_
#define WFILE_H_

#include "utils/common.h"
#include "utils/xenio.h"
#include "xenfile.h"

/**
 *  @class Wfile
 *  @brief Class handling a file with values intended at weighting XenC scores
 *
 *  The values file should contain one value per line, these values can also
 *  be in the log domain.
 */
class Wfile {
public:
    /**
     *  @fn Wfile ()
     *  @brief Default constructor
     */
    Wfile();
    
    /**
     *  @fn void initialize (boost::shared_ptr<XenFile> ptrFile)
     *  @brief Initialization function from an already instanciated XenFile
     *
     *  @param ptrFile : the weights file
     */
    void initialize(boost::shared_ptr<XenFile> ptrFile);
    
    /**
     *  @fn ~Wfile ()
     *  @brief Default destructor
     */
    ~Wfile();

    /**
     *  @fn double getWeight (int n)
     *  @brief Accessor to the nth weight of the file
     *
     *  @param n :  the number of the weight line in the file
     *  @return the requested weight
     */
    double getWeight(int n);
    
    /**
     *  @fn unsigned int getSize() const
     *  @brief Accessor to the size of the weights file
     *
     *  @return the size of the weights file
     */
    unsigned int getSize() const;
    
private:
    boost::shared_ptr<XenFile> ptrFile;                     //!< Shared pointer to the XenFile containing the weights
    boost::shared_ptr<std::vector<double> > ptrWeights;     //!< Shared pointer to the vector of doubles holding each weight

    /**
     *  @fn void loadWeights ()
     *  @brief Loads the weights file
     */
    void loadWeights();
    
    /**
     *  @fn void calibrate ()
     *  @brief Calibrates the loaded weights
     */
    void calibrate();
};

#endif
