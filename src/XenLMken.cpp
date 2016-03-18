/**
 *  @file XenLMken.cpp
 *  @brief Class handling KenLM estimation, loading, querying...
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

#include "../include/XenLMken.h"

XenLMken::XenLMken() {
    XenOption* opt = XenOption::getInstance();

    order = (unsigned int) opt->getOrder();
    mempc = opt->getMemPc();
    temp = opt->getTemp();

    textFile = NULL;
    lmFile = NULL;
    
    pc = 0;
}

void XenLMken::initialize(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<XenVocab> ptrVoc) {
    XenOption* opt = XenOption::getInstance();
    
    this->ptrCorp = ptrCorp;
    this->ptrVoc = ptrVoc;

    ptrXR = boost::make_shared<XenResult>();    //!< No use for a XenResult here
    pc = 0;
    
    textFile = new char[ptrCorp->getXenFile()->getFullPath().length() + 1];
    std::strcpy(textFile, ptrCorp->getXenFile()->getFullPath().c_str());

    std::string name = makeLMname(opt);
    lmFile = new char[name.length() + 1];
    std::strcpy(lmFile, name.c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

void XenLMken::initialize(boost::shared_ptr<XenFile> ptrFile, boost::shared_ptr<XenVocab> ptrVoc) {
    XenOption* opt = XenOption::getInstance();
    
    ptrCorp = boost::make_shared<Corpus>();     //!< No use for a corpus here
    this->ptrVoc = ptrVoc;
    
    ptrXR = boost::make_shared<XenResult>();    //!< No use for a XenResult here
    pc = 0;

    textFile = new char[1];
    std::strcpy(textFile, "");
    
    lmFile = new char[ptrFile->getFullPath().length() + 1];
    std::strcpy(lmFile, ptrFile->getFullPath().c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

void XenLMken::initialize(boost::shared_ptr<XenResult> ptrXenRes, boost::shared_ptr<XenVocab> ptrVoc, int pc, std::string name) {
    XenOption* opt = XenOption::getInstance();
    
    ptrCorp = boost::make_shared<Corpus>();     //!< No use for a corpus here
    this->ptrVoc = ptrVoc;
    
    ptrXR = ptrXenRes;
    this->pc = pc;

    textFile = new char[1];
    std::strcpy(textFile, "");
    
    lmFile = new char[name.length() + 1];
    std::strcpy(lmFile, name.c_str());
    
    setlocale(LC_CTYPE, "");
    setlocale(LC_COLLATE, "");
}

XenLMken::~XenLMken() {
    delete[] lmFile;
    delete[] textFile;
}

int XenLMken::createLM() {
    XenOption* opt = XenOption::getInstance();

    if (boost::filesystem::exists(lmFile))
        std::cout << "LM file already here, reusing..." << std::endl;
    else {
        lm::builder::PipelineConfig pipeline;

        std::string text, intermediate, arpa;
        std::vector<std::string> discount_fallback;
        discount_fallback.push_back("0.5");
        discount_fallback.push_back("1");
        discount_fallback.push_back("1.5");
        bool verbose_header = true;

        pipeline.order = (size_t) opt->getOrder();
        pipeline.initial_probs.interpolate_unigrams = true;
        pipeline.sort.temp_prefix = temp;
        pipeline.sort.total_memory = opt->getMemPc();
        pipeline.minimum_block = opt->getMinBlk();
        pipeline.sort.buffer_size = opt->getSortBlk();
        pipeline.block_count = 2;
        pipeline.vocab_estimate = 1000000;
        pipeline.prune_vocab_file = ptrVoc->getXenFile()->getFullPath();
        pipeline.prune_vocab = true;
        pipeline.vocab_size_for_unk = 0;
        pipeline.disallowed_symbol_action = lm::THROW_UP;

        lm::builder::Discount dis;
        dis.amount[0] = 0.0;
        for (unsigned i = 0; i < 3; ++i) {
            float discount = boost::lexical_cast<float>(discount_fallback[i < discount_fallback.size() ? i : (discount_fallback.size() - 1)]);
            UTIL_THROW_IF(discount < 0.0 || discount > static_cast<float>(i+1), util::Exception, "The discount for count " << (i+1) << " was parsed as " << discount << " which is not in the range [0, " << (i+1) << "].");
            dis.amount[i + 1] = discount;
        }

        pipeline.discount.fallback = dis;
        pipeline.discount.bad_action = lm::COMPLAIN;
        pipeline.prune_thresholds.resize(order, 0);

        util::NormalizeTempPrefix(pipeline.sort.temp_prefix);

        lm::builder::InitialProbabilitiesConfig &initial = pipeline.initial_probs;
        initial.adder_in.total_memory = 32768;
        initial.adder_in.block_count = 2;
        initial.adder_out.total_memory = 32768;
        initial.adder_out.block_count = 2;
        pipeline.read_backoffs = initial.adder_out;

        pipeline.renumber_vocabulary = false;
        pipeline.output_q = false;

        text = textFile;
        arpa = lmFile;

        util::scoped_fd in(util::OpenReadOrThrow(text.c_str()));
        util::scoped_fd out(util::CreateOrThrow(arpa.c_str()));

        try {
            lm::builder::Output output(pipeline.sort.temp_prefix, false, pipeline.output_q);
            output.Add(new lm::builder::PrintHook(out.release(), verbose_header));
            lm::builder::Pipeline(pipeline, in.release(), output);
        } catch (const util::MallocException &e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Try rerunning with a more conservative -S setting than " << XenCommon::toString(mempc) << std::endl;
        }

        std::cout << "LM estimation done." << std::endl;
    }
    
    return 0;
}

int XenLMken::loadLM() {
    config.positive_log_probability = SILENT;

    ptrMdl = boost::make_shared<lm::ngram::ProbingModel>(lmFile, config);

    return 0;
}

std::string XenLMken::getFileName() const {
    return lmFile;
}

TxtStats XenLMken::getSentenceStats(std::string sent) {
    TxtStats r;
    typename lm::ngram::Model::State state, out;
    lm::FullScoreReturn ret;

    std::vector<std::string> words;
    boost::split(words, sent, boost::is_any_of(" "));

    state = ptrMdl->BeginSentenceState();

    float total = 0.0;
    uint64_t oov = 0;
    uint64_t numwords = 0;
    float zeroprob = 0.0;

    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
        lm::WordIndex vocab = ptrMdl->GetVocabulary().Index(*it);
        ret = ptrMdl->FullScore(state, vocab, out);
        if (vocab == ptrMdl->GetVocabulary().NotFound()) {
            ++oov;
            zeroprob += ret.prob;
        }
        total += ret.prob;
        ++numwords;
        state = out;
    }

    ret = ptrMdl->FullScore(state, ptrMdl->GetVocabulary().EndSentence(), out);
    total += ret.prob;

    r.prob = total;
    r.zeroprobs = zeroprob;
    r.numwords = numwords;
    r.numoov = oov;
    r.numsentences = 1;

    return r;
}

TxtStats XenLMken::getDocumentStats(boost::shared_ptr<Corpus> c) {
    TxtStats r, tmp;
    r.prob = 0.0;
    r.zeroprobs = 0.0;
    r.numoov = 0;
    r.numwords = 0;
    r.numsentences = 0;

    for (unsigned int i = 0; i < c->getSize(); i++) {
        tmp = getSentenceStats(c->getLine(i));

        r.prob += tmp.prob;
        r.zeroprobs += tmp.zeroprobs;
        r.numwords += tmp.numwords;
        r.numoov += tmp.numoov;
        r.numsentences += tmp.numsentences;
    }
    
    return r;
}

std::string XenLMken::makeLMname(XenOption* opt) {
    //!< Common name
    std::string ret = opt->getInSData()->getDirName() + "/" + ptrCorp->getXenFile()->getPrefix() + "." + ptrVoc->getXenFile()->getPrefix() + "." + XenCommon::toString(opt->getOrder()) + "g.";

    //!< Cut-offs
    for (unsigned int i = 1; i < order; i++)
        ret = ret + "0-";
    ret = ret + "0.";
    
    //!< Output format (extension)
    ret = ret + "arpa";
    
    return ret;
}
