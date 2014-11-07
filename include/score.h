/**
 *  @file score.h
 *  @brief Class holding the XenC scores representation
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

#ifndef SCORE_H_
#define SCORE_H_

#include "utils/common.h"       //!< Inherits std::vector from here

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost;

/**
 *  @class Score
 *  @brief Class holding the XenC scores representation
 *
 *  This class holds the representation of XenC scores.
 *  Can add/remove scores and provides access to them.
 */
class Score {
public:
    /**
     *  @fn Score ()
     *  @brief Default constructor
     */
    Score();
    
    /**
     *  @fn ~Score ()
     *  @brief Default destructor
     */
    ~Score();
    
    /**
     *  @fn void addScore (double sc)
     *  @brief Adds a score to the vector of doubles
     *
     *  @param sc :     score to add to the Score holder
     */
    void addScore(double sc);
    
    /**
     *  @fn void removeScore (int n)
     *  @brief Removes the nth score from the vector of doubles
     *
     *  @param n :      position of the score to remove in the vector
     */
    void removeScore(int n);
    
    /**
     *  @fn double getScore (int n) const
     *  @brief Accessor to the nth score
     *
     *  @param n :      position of the score to return
     *  @return double representing the requested score
     */
    double getScore(int n) const;
    
    /**
     *  @fn bool getPrint (int n) const
     *  @brief Accessor to the output status of the nth score
     *
     *  @param n :      position of the printing status to get
     *  @return true if the score should be outputted
     */
    bool getPrint(int n) const;
    
    /**
     *  @fn unsigned int getSize () const
     *  @brief Accessor to the size of the scores vector
     *
     *  @return unsigned int representing the size
     */
    unsigned int getSize() const;

    /**
     *  @fn void calibrate ()
     *  @brief Calibrates the scores distribution between 0 and 1
     */
    void calibrate();
    
    /**
     *  @fn void inverse ()
     *  @brief Inverts the calibrated score distribution (1 - score)
     */
    void inverse();
    
private:
    boost::shared_ptr<std::vector<double> > ptrScores;      //!< Shared pointer on a vector of doubles holding the scores
    boost::shared_ptr<std::vector<int> > ptrPrint;          //!< Shared pointer on a vector of integers holding the printing status
};

#endif
