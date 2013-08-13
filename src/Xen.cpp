/**
 *  @file Xen.cpp
 *  @brief Main file of XenC, controls execution
 *  @author Anthony Rousseau
 *  @version 1.1.0
 *  @date 13 August 2013
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

#include "Xen.h"

int main(int argc, char* argv[]) {
    po::options_description desc("XenC options", 200);
    Options opt;
    
    try {
        desc.add_options()
        ("source,s", po::value<std::string>(&opt.sLang)->required(), "source language (fr, en, ...)")
        ("target,t", po::value<std::string>(&opt.tLang)->default_value(""), "target language (if relevant)")
        ("in-stext,i", po::value<std::string>(&opt.inSData)->required(), "in-domain source text filename (plain text or gzipped file)")
        ("out-stext,o", po::value<std::string>(&opt.outSData)->required(), "out-of-domain source text filename (plain text or gzipped file)")
        ("mode,m", po::value<int>(&opt.mode)->required()->default_value(2), "filtering mode (1, 2, 3 or 4). Default is 2 (monolingual cross-entropy)")
        ("eval,e", po::value<bool>(&opt.eval)->zero_tokens()->default_value(false), "add this switch to evaluate a filtered file after computation. Eval is always done on source language")
        ("best-point,b", po::value<bool>(&opt.bp)->zero_tokens()->default_value(false), "add this switch to determinate the best point of a filtered file (eval option is implicit)")
        ("dev,d", po::value<std::string>(&opt.dev)->default_value(""), "source language dev file for eval or best point (all modes), if different from in-domain text")
        ("in-ttext", po::value<std::string>(&opt.inTData)->default_value(""), "in-domain target text filename, if target language (plain text or gzipped file)")
        ("out-ttext", po::value<std::string>(&opt.outTData)->default_value(""), "out-of-domain target text filename, if target language (plain text or gzipped file)")
        ("mono", po::value<bool>(&opt.mono)->zero_tokens()->default_value(false), "switch to force monolingual mode (if no target language)")
        ("stem", po::value<bool>(&opt.stem)->zero_tokens()->default_value(false), "switch to activate stem models computation and scoring from stem files")
        ("in-sstem", po::value<std::string>(&opt.inSStem)->default_value(""), "in-domain source stem filename (plain text or gzipped file)")
        ("in-tstem", po::value<std::string>(&opt.inTStem)->default_value(""), "in-domain target stem filename (plain text or gzipped file)")
        ("out-sstem", po::value<std::string>(&opt.outSStem)->default_value(""), "out-of-domain source stem filename (plain text or gzipped file)")
        ("out-tstem", po::value<std::string>(&opt.outTStem)->default_value(""), "out-of-domain target stem filename (plain text or gzipped file)")
        ("in-ptable", po::value<std::string>(&opt.iPTable)->default_value(""), "in-domain phrase table filename used in mode 4 scoring")
        ("out-ptable", po::value<std::string>(&opt.oPTable)->default_value(""), "out-of-domain phrase table filename used in mode 4 scoring")
        ("local", po::value<bool>(&opt.local)->zero_tokens()->default_value(false), "add a 7th score (local cross-entropy regarding the source phrase)")
        ("mean", po::value<bool>(&opt.mean)->zero_tokens()->default_value(false), "mean score from 3 OOD sample LMs instead of 1 in mode 2 & 3 (3 times slower + EXPERIMENTAL)")
        ("sim", po::value<bool>(&opt.sim)->zero_tokens()->default_value(false), "add similarity measures to score computing (EXPERIMENTAL, mode 2 only)")
        ("sim-only", po::value<bool>(&opt.simOnly)->zero_tokens()->default_value(false), "use only similarity measures (no cross-entropy)")
        ("vector-size", po::value<int>(&opt.vecSize)->default_value(150), "size of vector for similarity scores, default is 150 (WARNING: the more the slower)")
        ("step", po::value<int>(&opt.step)->default_value(10), "percentage steps for evaluation. Default is 10 (100%, 90%, ...)")
        ("s-vocab", po::value<std::string>(&opt.sVocab)->default_value(""), "source language vocab filename for LMs estimation. Default is in-domain source text vocab")
        ("t-vocab", po::value<std::string>(&opt.tVocab)->default_value(""), "target language vocab filename for LMs estimation. Default is in-domain target text vocab")
        ("full-vocab", po::value<bool>(&opt.fullVoc)->zero_tokens()->default_value(false), "use in-domain + out-of-domain vocabularies instead of in-domain only")
        ("in-slm", po::value<std::string>(&opt.inSLM)->default_value(""), "in-domain source language model (LM). Will be estimated if not present")
        ("out-slm", po::value<std::string>(&opt.outSLM)->default_value(""), "out-of-domain source language model (LM). Will be estimated if not present")
        ("in-tlm", po::value<std::string>(&opt.inTLM)->default_value(""), "in-domain target language model (LM). Will be estimated if not present")
        ("out-tlm", po::value<std::string>(&opt.outTLM)->default_value(""), "out-of-domain target language model (LM). Will be estimated if not present")
        ("order", po::value<int>(&opt.order)->default_value(4), "order for LMs. Default is 4")
        ("discount", po::value<int>(&opt.discount)->default_value(0), "discounting method for LM estimation. Default is modified KneserNey (0). 1 is GoodTuring, 2 is WittenBell.")
        ("bin-lm", po::value<int>(&opt.binLM)->default_value(1), "whether you want to estimate arpa.gz (0) or binary (1) LMs. Default is 1 (binary)")
        ("w-file", po::value<std::string>(&opt.wFile)->default_value(""), "filename for weighting the final score (one value per line)")
        ("log", po::value<bool>(&opt.log)->zero_tokens()->default_value(false), "switch to consider weights in w-file as log values")
        ("rev", po::value<bool>(&opt.rev)->zero_tokens()->default_value(false), "switch to require descending order sorted output")
        ("inv", po::value<bool>(&opt.inv)->zero_tokens()->default_value(false), "switch to require inversed calibrated scores (1 - score)")
        ("threads", po::value<int>(&opt.threads)->default_value(2), "number of threads to run for various operations (eval, sim, ...). Default is 2")
        ("sorted-only", po::value<bool>(&opt.sortOnly)->zero_tokens()->default_value(false), "switch to save space & time by only outputing the sorted scores file")
        ("help,h", "displays this help message")
        ("version,v", "displays program version");
        
        po::variables_map vm;
        
        try {
            po::store(po::parse_command_line(argc, argv, desc), vm);
            
            if (vm.count("help") || argc == 1) {
                std::cout << "XenC version 1.0 PUBLIC RELEASE. Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France." << std::endl << std::endl;
                
                std::cout << desc << std::endl;
                
                std::cout << "Filtering modes:" << std::endl << std::endl;
                std::cout << "For all modes (excepted 4), you must provide at least a source language, and in-domain and out-of-domain bitexts. Bitexts MUST NOT contain tabs." << std::endl;
                std::cout << "For every text file used, max words per line is 16384 and max chars per line is max words * 16." << std::endl << std::endl;
                std::cout << "Also, if no vocabularies and no language models are provided, they will be generated with the following parameters:" << std::endl;
                std::cout << "\t- vocabs:\tvocabularies will be created from words of in-domain bitexts." << std::endl;
                std::cout << "\t- LMs:\t\torder 4, modified kn-int smoothing, 0-0-0-0 cut-offs, sblm (binary) output format." << std::endl << std::endl;
                std::cout << "\t1:" << std::endl;
                std::cout << "\tSimple source language perplexity filtering. (Gao & al. 2002)" << std::endl;
                std::cout << "\tWill sort the out-of-domain bitext sentences (ascending order)" << std::endl;
                std::cout << "\tbased on perplexity scores given by a in-domain language model." << std::endl << std::endl;
                std::cout << "\t2:" << std::endl;
                std::cout << "\tSource language cross-entropy (Xen) difference filtering. (Moore & Lewis 2010)" << std::endl;
                std::cout << "\tWill sort the out-of-domain bitext sentences (ascending order)" << std::endl;
                std::cout << "\tbased on (in-source Xen - out-source Xen)." << std::endl << std::endl;
                std::cout << "\t3:" << std::endl;
                std::cout << "\tBilingual cross-entropy difference filtering. (Axelrod & al. 2011)" << std::endl;
                std::cout << "\tWill sort the out-of-domain bitext sentences (ascending order)" << std::endl;
                std::cout << "\tbased on (in-source Xen - out-source Xen) + (in-target Xen - out-target Xen)." << std::endl << std::endl;
                std::cout << "\t4:" << std::endl;
                std::cout << "\tPhrase-table scoring mode. (EXPERIMENTAL)" << std::endl;
                std::cout << "\tAdds the cross-entropy score of each phrase pair" << std::endl;
                std::cout << "\tin a phrase-table as a sixth feature of the table." << std::endl << std::endl;
                std::cout << "\tYou must provide:" << std::endl;
                std::cout << "\t\t- in-domain and out-of-domain phrase tables." << std::endl;
                std::cout << "\t\t- source and target vocabularies." << std::endl << std::endl;
                
                return 0;
            }
            
            if (vm.count("version")) {
                std::cout << "XenC version 1.0 PUBLIC RELEASE. Copyright 2013, Anthony Rousseau, LIUM, University of Le Mans, France." << std::endl;
                return 0;
            }
            
            po::notify(vm);
        } catch (po::error& e) {
            std::cout << desc << std::endl;
            std::cout << e.what() << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << desc << std::endl;
        std::cout << e.what() << std::endl;
    }

    opt.pc = 0;
    opt.inToks = 0;
    opt.outToks = 0;
    
    if (opt.dev.compare("") == 0) {
        if (boost::filesystem::exists(opt.inSData)) {
            opt.dev = opt.inSData;
        }
        else {
            std::cerr << "You must at least specify a source in-domain corpus." << std::endl;
            return 1;
        }
    }
    
    // -----------------------------------------------------
    // Create singletons & mode
    XenOption* xOpt = XenOption::getInstance(&opt);
    StaticData* sD = StaticData::getInstance();
    
    boost::shared_ptr<Mode> mode;
    
    switch (xOpt->getMode()) {
        case 1:
            mode = boost::make_shared<SimplePPL>();
            break;
            
        case 2:
            mode = boost::make_shared<MonoXEntropy>();
            break;
            
        case 3:
            mode = boost::make_shared<BiXEntropy>();
            break;
            
        case 4:
            mode = boost::make_shared<PTScoring>();
            break;
            
        default:
            break;
    }
    // -----------------------------------------------------
    
    opt.outName = getOutName(xOpt);
    std::string sC = sanityCheck(xOpt);          // Check if all mandatory are here

    // -----------------------------------------------------
    // LAST CHECK BEFORE OPERATIONS
	if (sC.compare("0") == 0) {
		std::cout << "Source language: " << opt.sLang << std::endl;
		if (!opt.mono) { std::cout << "Target language: " << opt.tLang << std::endl; }
        if (opt.mode != 4) {
            std::cout << "In-domain source data: " << opt.inSData << std::endl;
            std::cout << "Out-of-domain source data: " << opt.outSData << std::endl;
            if (opt.stem) {
                std::cout << "In-domain source stem file: " << opt.inSStem << std::endl;
                std::cout << "Out-of-domain source stem file: " << opt.outSStem << std::endl;
            }
            if (!opt.mono) {
                std::cout << "In-domain target data: " << opt.inTData << std::endl;
                std::cout << "Out-of-domain target data: " << opt.outTData << std::endl;
                if (opt.stem) {
                    std::cout << "In-domain target stem file: " << opt.inTStem << std::endl;
                    std::cout << "Out-of-domain target stem file: " << opt.outTStem << std::endl;
                }
            }
        }
        else {
            std::cout << "Source vocabulary: " << opt.sVocab << std::endl;
            std::cout << "Target vocabulary: " << opt.tVocab << std::endl;
            std::cout << "In-domain phrase table: " << opt.iPTable << std::endl;
            std::cout << "Out-of-domain phrase table: " << opt.oPTable << std::endl;
            std::cout << "Output phrase table: " << opt.outName << std::endl;
        }
		std::cout << "Mode: " << opt.mode << std::endl;
	}
	else {
        std::cerr << std::endl << sC << std::endl;
        sD->deleteInstance();
        xOpt->deleteInstance();
		return 1;
	}
    // -----------------------------------------------------

    try {
        // Normal mode
        if (!xOpt->getEval()&& !xOpt->getBp()) {
            int ret = mode->launch();
            
            if (ret == 0) {
                xOpt->deleteInstance();
                sD->deleteInstance();
                return 0;
            }
            else {
                std::cerr << "Something went wrong." << std::endl;
                xOpt->deleteInstance();
                sD->deleteInstance();
                return 1;
            }
        }
        // Eval or BP
        else {
            std::string sortedName = xOpt->getOutName() + ".sorted.gz";
            std::string distName = xOpt->getOutName() + ".dist";
            std::string bpName = xOpt->getOutName() + ".bp";
            
            std::cout << "Sorted output used: " + sortedName << std::endl;
            
            // -----------------------------------------------------
            // Proceed to normal mode if not done before
            if (!boost::filesystem::exists(sortedName)) {
                int ret = mode->launch();
                
                if (ret != 0) {
                    std::cerr << "Something went wrong." << std::endl;
                    xOpt->deleteInstance();
                    sD->deleteInstance();
                    return 1;
                }
            }
            // -----------------------------------------------------
            
            boost::shared_ptr<XenFile> sorted = boost::make_shared<XenFile>();
            sorted->initialize(sortedName);
            
            sD->getXenResult()->initialize(sorted);
            sD->getDevCorp()->initialize(xOpt->getDev(), xOpt->getSLang());
            
            // Eval
            if (xOpt->getEval()) {
                boost::shared_ptr<Eval> ptrEval = boost::make_shared<Eval>();
                
                ptrEval->doEval(100, 0);
                int oldStep = xOpt->getStep();
                xOpt->setStep(2);
                ptrEval->doEval(8, 0);
                xOpt->setStep(oldStep);
                
                XenIO::writeEval(ptrEval->getDist(), distName);
            }
            // BP
            else if (xOpt->getBp()) {
                boost::shared_ptr<Eval> ptrEval;
                
                if (boost::filesystem::exists(distName.c_str()))
                    ptrEval = boost::make_shared<Eval>(distName);
                else {
                    ptrEval = boost::make_shared<Eval>();
                    ptrEval->doEval(100, 0);
                    int oldStep = xOpt->getStep();
                    xOpt->setStep(2);
                    ptrEval->doEval(8, 0);
                    xOpt->setStep(oldStep);
                    XenIO::writeEval(ptrEval->getDist(), distName);
                }
                
                ptrEval->doBP();
                XenIO::writeEval(ptrEval->getDist(), bpName);
            }
            else { return 1; }
        }
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    xOpt->deleteInstance();
    sD->deleteInstance();
	return 0;
}

std::string sanityCheck(XenOption* opt) {
    std::string sortName = getOutName(opt) + ".sorted.gz";
        
	if (boost::filesystem::exists(sortName.c_str()) && (!opt->getEval() && !opt->getBp()) && opt->getMode() != 4)
		return "Final sorted file " + sortName + " already exists, exiting.";
    
    if (opt->getMode() == 4) {
        if (opt->getSLang().compare("") == 0) { return "Please specify a source language."; }
        else if (opt->getTLang().compare("") == 0) { return "Please specify a target language."; }
        else if (opt->getInPTable()->getFullPath().compare("") == 0) { return "Please specify an in-domain phrase table."; }
        else if (opt->getOutPTable()->getFullPath().compare("") == 0) { return "Please specify an out-of-domain phrase table"; }
        else if (opt->getSVocab()->getFullPath().compare("") == 0) { return "Please specify a source vocabulary."; }
        else if (opt->getTVocab()->getFullPath().compare("") == 0) { return "Please specify a target vocabulary."; }
    }
    else {
        if (opt->getSLang().compare("") == 0) { return "Please specify a source language."; }
        else if (opt->getTLang().compare("") == 0 && (!opt->getMono() || opt->getMode() == 3)) { return "Please specify a target language (or add the --mono option)."; }
        else if (opt->getInSData()->getFileName().compare("") == 0) { return "Please specify an in-domain source data."; }
        else if (opt->getOutSData()->getFileName().compare("") == 0 && opt->getMode() > 0) { return "Please specify an out-of-domain source data."; }
        else if (opt->getInTData()->getFileName().compare("") == 0 && opt->getMode() == 3) { return "Please specify an in-domain target data."; }
        else if (opt->getOutTData()->getFileName().compare("") == 0 && opt->getMode() == 3) { return "Please specify an out-of-domain target data."; }
        else if (opt->getMode() == -1) { return "Please specify a mode."; }
        else if (opt->getMode() > 4 || opt->getMode() == 0) { return "Mode should be 1, 2, 3 or 4."; }
    }
    
	return "0";
}

std::string getOutName(XenOption* opt) {
    std::string baseName = "";
    
    if (opt->getMode() != 4) {
        std::string weight = "";
        std::string loga = "";
        std::string reve = "";
        std::string inve = "";
        std::string stem = "";

        if (opt->getWFile()->getFileName().compare("") != 0) { weight = ".weight"; }
        if (opt->getLog()) { loga = ".log"; }
        if (opt->getRev()) { reve = ".rev"; }
        if (opt->getInv()) { inve = ".inv"; }
        if (opt->getStem()) { stem = ".stem"; }
        
        baseName = opt->getOutSData()->getPrefix() + "." + opt->getSLang();
        if (opt->getTLang().compare("") != 0) { baseName = baseName + "-" + opt->getTLang(); }
        baseName = baseName + ".mode" + XenCommon::toString(opt->getMode()) + weight + loga + reve + inve + stem;
    }
    else {
        baseName = opt->getOutPTable()->getPrefix() + "-new." + opt->getOutPTable()->getExt();
    }
        
	return baseName;
}
