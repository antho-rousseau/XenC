/**
 *  @file xenio.h
 *  @brief Class handling all input/output operations of XenC
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

#ifndef XENIO_H_
#define XENIO_H_

#include "common.h"
#include "eval.h"
#include "score.h"
#include "xenoption.h"
#include "similarity.h"

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/regex.hpp>

using namespace boost;
using namespace boost::iostreams;

class Corpus;       // Forward declaration
class PhraseTable;  // Forward declaration

/**
 *  @class XenIO
 *  @brief Class handling all input/output operations of XenC
 *
 *  This class handles file reading/writing (plain text or compressed),
 *  corpus cleaning, phrases and phrase-tables writing, similarity dumping...
 */
class XenIO {
public:
    /**
     *  @fn static void cleanCorpusMono (boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore)
     *  @brief Monolingual corpus cleaning (ensures no empty lines)
     *
     *  @param ptrCorp :    the Corpus to clean
     *  @param ptrScore :   the associated Score object to clean
     */
    static void cleanCorpusMono(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore);
    
    /**
     *  @fn static void cleanCorpusBi (boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore)
     *  @brief Bilingual corpus cleaning (ensures no empty lines)
     *
     *  @param ptrCorpSource :  the source language Corpus to clean
     *  @param ptrCorpTarget :  the target language Corpus to clean
     *  @param ptrScore :       the associated Score object to clean
     */
    static void cleanCorpusBi(boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore);
    
    /**
     *  @fn static void writeMonoOutput (boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore)
     *  @brief Writes monolingual scored/sorted result files
     *
     *  @param ptrCorp :    the Corpus to write
     *  @param ptrScore :   the associated Score object to write
     */
    static void writeMonoOutput(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore);
    
    /**
     *  @fn static void writeBiOutput (boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore)
     *  @brief Writes bilingual scored/sorted result files
     *
     *  @param ptrCorpSource :  the source language Corpus to write
     *  @param ptrCorpTarget :  the target language Corpus to write
     *  @param ptrScore :       the associated Score object to write
     */
    static void writeBiOutput(boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore);
    
    /**
     *  @fn static void writeNewPT (boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<Score> ptrScore)
     *  @brief Writes a new rescored phrase-table
     *
     *  @param ptrPT :      the new phrase-table to write
     *  @param ptrScore :   the associated Score object to write
     */
    static void writeNewPT(boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<Score> ptrScore);
    
    /**
     *  @fn static std::string writeSourcePhrases (boost::shared_ptr<PhraseTable> ptrPT)
     *  @brief Writes a phrase-table's source phrases
     *
     *  @param ptrPT :  the phrase-table to write the source phrases from
     *  @return the written source phrases file name
     */
    static std::string writeSourcePhrases(boost::shared_ptr<PhraseTable> ptrPT);
    
    /**
     *  @fn static std::string writeTargetPhrases (boost::shared_ptr<PhraseTable> ptrPT)
     *  @brief Writes a phrase-table's target phrases
     *
     *  @param ptrPT : the phrase-table to write the target phrases from
     *  @return the written target phrases file name
     */
    static std::string writeTargetPhrases(boost::shared_ptr<PhraseTable> ptrPT);
    
    /**
     *  @fn static void writeEval (boost::shared_ptr<Eval> ptrEval, std::string distName)
     *  @brief Writes an evaluation/best point distribution file
     *
     *  @param ptrEvalMap :     the EvalMap containing the scores to write
     *  @param distName :       the distribution file name
     */
    static void writeEval(boost::shared_ptr<EvalMap> ptrEvalMap, std::string distName);
    
    /**
     *  @fn static void dumpSimilarity (boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Similarity> ptrSim)
     *  @brief Dumps the Similarity measures of a Corpus
     *
     *  @param ptrCorp :    the Corpus from which the Similarity measures are dumped
     *  @param ptrSim :     the Similarity measures to dump
     */
    static void dumpSimilarity(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Similarity> ptrSim);
    
    /**
     *  @fn static std::vector<std::string> read (boost::shared_ptr<XenFile> ptrFile)
     *  @brief Reads a file (plain text/gzipped)
     *
     *  @param ptrFile :    the file to read
     *  @return a vector of strings containing the read file's lines
     */
    static std::vector<std::string> read(boost::shared_ptr<XenFile> ptrFile);
    
    /**
     *  @fn static boost::shared_ptr<EvalMap> readDist(std::string distFile)
     *  @brief Reads a evaluation/best point distribution file
     *
     *  @param distFile :   file to read
     *  @return an EvalMap containing the already computed scores
     */
    static boost::shared_ptr<EvalMap> readDist(std::string distFile);
};

#endif
