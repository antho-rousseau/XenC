/*
 * This file is part of the cross-entropy tool for data selection (XenC)
 * aimed at speech recognition and statistical machine translation.
 *
 * Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France
 *
 * The XenC tool is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: xenio.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle all input/output from & to XenC
 *  read/write files, read/write gzip files & all...
 *  All public & static
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
#include <boost/algorithm/string.hpp>

using namespace boost;
using namespace boost::filesystem;
using namespace boost::iostreams;

class Corpus;
class PhraseTable;

class XenIO {
    public:
        static void cleanCorpusMono(shared_ptr<Corpus>, shared_ptr<Score>);
        static void cleanCorpusBi(shared_ptr<Corpus>, shared_ptr<Corpus>, shared_ptr<Score>);
        static void writeMonoOutput(shared_ptr<Corpus>, shared_ptr<Score>);
        static void writeBiOutput(shared_ptr<Corpus>, shared_ptr<Corpus>, shared_ptr<Score>);
        static void writeNewPT(shared_ptr<PhraseTable>, shared_ptr<Score>);
        static string writeSourcePhrases(shared_ptr<PhraseTable>);
        static string writeTargetPhrases(shared_ptr<PhraseTable>);
        static void writeEval(shared_ptr<Eval>, string);
        static void dumpSimilarity(shared_ptr<Corpus>, shared_ptr<Similarity>);
        static vector<string> read(shared_ptr<XenFile>);
};

#endif
