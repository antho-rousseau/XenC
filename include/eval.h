/**
 *  @file eval.h
 *  @brief Class handling evaluation system
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

#ifndef EVAL_H_
#define EVAL_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "utils/common.h"
#include "utils/threadpool.hpp"
#include "corpus.h"
#include "xenoption.h"
#include "XenLMken.h"
#include "ppl.h"
#include "xenresult.h"
#include "xenvocab.h"

class StaticData;   /// Forward declaration

using namespace boost::threadpool;
using namespace boost;

typedef std::map<int, double, std::greater<int> > EvalMap;  //!< descending ordered map on integers as keys and doubles as values

/**
 *  @fn void taskEval(int pc, boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc, boost::shared_ptr<Corpus> ptrDevCorp, boost::shared_ptr<EvalMap> ptrDist)
 *  @brief Thread-safe evaluation function
 *
 *  @param pc :         integer representing the percentage of the scored out-of-domain corpus to take
 *  @param ptrCorp :      shared pointer on the Corpus object representing the part of selection result file
 *  @param ptrVoc :     shared pointer on the XenVocab object representing the vocabulary to use for eval
 *  @param ptrDevCorp : shared pointer on the Corpus object representing the development set
 *  @param ptrDist :    shared pointer on the EvalMap type containing the evaluation scores
 */
void taskEval(int pc, boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc, boost::shared_ptr<Corpus> ptrDevCorp, boost::shared_ptr<EvalMap> ptrDist);

/**
 *  @class Eval
 *  @brief Evaluation system
 *
 *  This class handles the evaluation procedure in XenC, providing mean to perform eval, best point,
 *  and getting the results.
 *  It uses threads extensively, so please watch your memory usage since there is some memory leaks
 *  in SRILM.
 */
class Eval {
public:
    /**
     *  @fn Eval ()
     *  @brief Default constructor
     */
    Eval();
    
    /**
     *  @fn Eval (std::string distFile)
     *  @brief Constructor from a string
     *
     *  @param distFile :   string containing a valid path to the evaluation (*.dist) file, usually used when doing BP
     */
    Eval(std::string distFile);
    
    /**
     *  @fn ~Eval ()
     *  @brief Default destructor
     */
    ~Eval();

    /**
     *  @fn void doEval (int high, int low)
     *  @brief Computes an evaluation bount by the high and low integers (in percentage)
     *
     *  @param high :   integer representing the upper bound for evaluation
     *  @param low :    integer representing the lower bound for evaluation
     */
    void doEval(int high, int low);
    
    /**
     *  @fn void doBP ()
     *  @brief Computes the best theoretical point based on current evaluation
     */
    void doBP();
    
    /**
     *  @fn EvalMap getDist () const
     *  @brief Accessor to the evaluation distribution map
     *
     *  @return shared pointer on EvalMap containing all the evaluation results
     */
    boost::shared_ptr<EvalMap> getDist() const;

    /**
     *  @fn int getBP ()
     *  @brief Accessor to the determined best point
     *
     *  @return integer representing the percentage for the best point
     */
    int getBP();
    
private:
    boost::shared_ptr<EvalMap> ptrDist;     //!< Shared pointer on a EvalMap wrapping the reference to the map containing the evaluation results */
};

#endif
