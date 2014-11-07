/**
 *  @file xenio.cpp
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

#include "xenio.h"
#include "corpus.h"
#include "phrasetable.h"

void XenIO::cleanCorpusMono(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore) {
    std::cout << "Cleaning monolingual output..." << std::endl;
    boost::regex e("^\\s*$");
    
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
		if (boost::regex_search(ptrCorp->getLine(i), e, boost::match_default | boost::format_sed)) {
            ptrCorp->removeLine(i);
            ptrScore->removeScore(i);
        }
	}
    
    std::cout << "Monolingual output cleaned." << std::endl;
}

void XenIO::cleanCorpusBi(boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore) {
    std::cout << "Cleaning bilingual output..." << std::endl;
    boost::regex e("^\\s*$");
    
    for (unsigned int i = 0; i < ptrCorpSource->getSize(); i++) {
        if (boost::regex_search(ptrCorpSource->getLine(i), e, boost::match_default | boost::format_sed) || boost::regex_search(ptrCorpTarget->getLine(i), e, boost::match_default | boost::format_sed)) {
            ptrCorpSource->removeLine(i);
            ptrCorpTarget->removeLine(i);
            ptrScore->removeScore(i);
        }
    }
    
    std::cout << "Bilingual output cleaned." << std::endl;
}

void XenIO::writeMonoOutput(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Score> ptrScore) {
    XenOption* opt = XenOption::getInstance();
    
    std::string scoredName = opt->getOutName() + ".scored.gz";
    std::string sortedName = opt->getOutName() + ".sorted.gz";
    
    std::multimap<double, std::string> sortMap;
    
    for (unsigned int i = 0; i < ptrCorp->getSize(); i++)
        if (ptrCorp->getPrint(i) && ptrScore->getPrint(i)) {
            std::pair<double, std::string> p(ptrScore->getScore(i), ptrCorp->getLine(i));
            sortMap.insert(p);
        }
    
    try {
        if (!opt->getSortOnly()) {
            std::cout << "Writing scored output to " + scoredName << std::endl;
            
            
            boost::iostreams::filtering_ostream out;
            out.push(boost::iostreams::gzip_compressor());
            out.push(boost::iostreams::file_sink(scoredName.c_str(), std::ios_base::out | std::ios_base::binary));
            out.setf(std::ios::fixed | std::ios::showpoint);
            out.precision(15);
            
            if (!out.good())
                throw XenCommon::XenCEption("Something went wrong in output stream...");
            
            for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
                if (ptrCorp->getPrint(i) && ptrScore->getPrint(i))
                    out << XenCommon::toString(ptrScore->getScore(i)) << '\t' << ptrCorp->getLine(i) << std::endl;
            
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
            
            out.flush();
            out.reset();
        }
        
        std::cout << "Writing sorted output to " + sortedName << std::endl;
        
        boost::iostreams::filtering_ostream out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(boost::iostreams::file_sink(sortedName.c_str(), std::ios_base::out | std::ios_base::binary));
        out.setf(std::ios::fixed | std::ios::showpoint);
        out.precision(15);
        
        if (!out.good())
            throw XenCommon::XenCEption("Something went wrong in output stream...");
        
        if (opt->getRev()) {
            for (std::multimap<double, std::string>::reverse_iterator it = sortMap.rbegin(); it != sortMap.rend(); ++it) {
                out << XenCommon::toString(it->first) << '\t' << it->second << std::endl;
                
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
        }
        else {
            for (std::multimap<double, std::string>::iterator it = sortMap.begin(); it != sortMap.end(); ++it) {
                out << XenCommon::toString(it->first) << '\t' << it->second << std::endl;
                
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
        }

        out.flush();
        out.reset();
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

void XenIO::writeBiOutput(boost::shared_ptr<Corpus> ptrCorpSource, boost::shared_ptr<Corpus> ptrCorpTarget, boost::shared_ptr<Score> ptrScore) {
    XenOption* opt = XenOption::getInstance();
    
    std::string scoredName = opt->getOutName() + ".scored.gz";
    std::string sortedName = opt->getOutName() + ".sorted.gz";
    
    std::multimap<double, std::string> sortMap;
    
    for (unsigned int i = 0; i < ptrCorpSource->getSize(); i++)
        if (ptrCorpSource->getPrint(i) && ptrCorpTarget->getPrint(i) && ptrScore->getPrint(i)) {
            std::pair<double, std::string> p(ptrScore->getScore(i), ptrCorpSource->getLine(i) + '\t' + ptrCorpTarget->getLine(i));
            sortMap.insert(p);
        }
    
    try {
        if (!opt->getSortOnly()) {
            std::cout << "Writing scored output to " + scoredName << std::endl;
            
            boost::iostreams::filtering_ostream out;
            out.push(boost::iostreams::gzip_compressor());
            out.push(boost::iostreams::file_sink(scoredName.c_str(), std::ios_base::out | std::ios_base::binary));
            out.setf(std::ios::fixed | std::ios::showpoint);
            out.precision(15);
            
            if (!out.good())
                throw XenCommon::XenCEption("Something went wrong in output stream...");
            
            for (unsigned int i = 0; i < ptrCorpSource->getSize(); i++) {
                if (ptrCorpSource->getPrint(i) && ptrCorpTarget->getPrint(i) && ptrScore->getPrint(i))
                    out << XenCommon::toString(ptrScore->getScore(i)) << '\t' << ptrCorpSource->getLine(i) << '\t' << ptrCorpTarget->getLine(i) << std::endl;
                
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
            
            out.flush();
            out.reset();
        }
        
        std::cout << "Writing sorted output to " + sortedName << std::endl;
        
        boost::iostreams::filtering_ostream out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(boost::iostreams::file_sink(sortedName.c_str(), std::ios_base::out | std::ios_base::binary));
        out.setf(std::ios::fixed | std::ios::showpoint);
        out.precision(15);
        
        if (!out.good())
            throw XenCommon::XenCEption("Something went wrong in output stream...");
        
        if (opt->getRev()) {
            for (std::multimap<double, std::string>::reverse_iterator it = sortMap.rbegin(); it != sortMap.rend(); ++it) {
                out << XenCommon::toString(it->first) << '\t' << it->second << std::endl;
                
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
        }
        else {
            for (std::multimap<double, std::string>::iterator it = sortMap.begin(); it != sortMap.end(); ++it) {
                out << XenCommon::toString(it->first) << '\t' << it->second << std::endl;
                
                if (out.bad())
                    throw XenCommon::XenCEption("Something went wrong in output stream...");
            }
        }
        
        out.flush();
        out.reset();
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

void XenIO::writeNewPT(boost::shared_ptr<PhraseTable> ptrPT, boost::shared_ptr<Score> ptrScore) {
    XenOption* opt = XenOption::getInstance();
    Score sc2;
    
    if (opt->getLocal()) {
        std::vector<SourcePhrase> srcPh = ptrPT->getSrcPhrases();
    
        for (unsigned int i = 0; i < srcPh.size(); i++) {
            SourcePhrase sP = srcPh[i];
        
            for (unsigned int j = 0; j < sP.getScoresXE()->getSize(); j++) {
                sc2.addScore(sP.getScoresXE()->getScore(j));
            }
        }
    }
    
    try {
        std::cout << "Writing new phrase-table to " + opt->getOutName() << std::endl;
        
        std::string oF = opt->getOutName();
        
        boost::iostreams::filtering_ostream out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(boost::iostreams::file_sink(oF.c_str(), std::ios_base::out | std::ios_base::binary));
        
        if (!out.good())
            throw XenCommon::XenCEption("Can't write to " + opt->getOutName() + ".gz");

        for (unsigned int i = 0; i < ptrScore->getSize(); i++) {
            out << ptrPT->getSource(i) << " ||| " << ptrPT->getTarget(i) << " ||| " << ptrPT->getScores(i) << " " << XenCommon::toString(ptrScore->getScore(i));
            if (opt->getLocal())
                out << " " << XenCommon::toString(sc2.getScore(i));
            out << " ||| " << ptrPT->getAlignment(i) << " ||| " << ptrPT->getCounts(i) << std::endl;
            
            if (out.bad())
                throw XenCommon::XenCEption("Something went wrong in output stream...");
        }
        
        out.flush();
        out.reset();
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

std::string XenIO::writeSourcePhrases(boost::shared_ptr<PhraseTable> ptrPT) {
    std::string outName = ptrPT->getXenFile()->getPrefix() + "-source";
    
    if (boost::filesystem::exists(outName.c_str())) {
        std::cout << "Source phrases of table " << ptrPT->getXenFile()->getFileName() << " already dumped, we continue." << std::endl;
    }
    else {
        try {
            std::ofstream out(outName.c_str(), ios::out | ios::trunc);
            
            if (!out.is_open())
                throw XenCommon::XenCEption("Can't open " + outName + " for writing.");
            
            std::cout << "Writing source phrases of table " << ptrPT->getXenFile()->getFileName() << "." << std::endl;
            
            for(unsigned int i = 0; i < ptrPT->getSize(); i++) {
                out << ptrPT->getSource(i) << std::endl;
            
                if (out.bad())
                    throw XenCommon::XenCEption("Error while writing file " + outName);
            }
            
            out.close();
            
            std::cout << "Done writing source phrases." << std::endl;
        } catch (XenCommon::XenCEption &e) {
            throw;
        }
    }
    
    return outName;
}

std::string XenIO::writeTargetPhrases(boost::shared_ptr<PhraseTable> ptrPT) {
    std::string outName = ptrPT->getXenFile()->getPrefix() + "-target";
    
    if (boost::filesystem::exists(outName.c_str())) {
        std::cout << "Target phrases of table " << ptrPT->getXenFile()->getFileName() << " already dumped, we continue." << std::endl;
    }
    else {
        try {
            std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
            
            if (!out.is_open())
                throw XenCommon::XenCEption("Can't open " + outName + " for writing.");

            std::cout << "Writing target phrases of table " << ptrPT->getXenFile()->getFileName() << "." << std::endl;
            
            for(unsigned int i = 0; i < ptrPT->getSize(); i++) {
                out << ptrPT->getTarget(i) << std::endl;
            
                if (out.bad())
                    throw XenCommon::XenCEption("Error while writing file " + outName);
            }
            
            out.close();
            
            std::cout << "Done writing target phrases." << std::endl;
        } catch (XenCommon::XenCEption &e) {
            throw;
        }
        
    }
    
    return outName;
}

void XenIO::writeEval(boost::shared_ptr<EvalMap> ptrEvalMap, std::string distName) {
    try {
        std::ofstream out(distName.c_str(), std::ios::out | std::ios::trunc);
        
        if (!out.is_open())
            throw XenCommon::XenCEption("Can't open " + distName + " for writing.");
        
        for(EvalMap::iterator it = ptrEvalMap->begin() ; it != ptrEvalMap->end() ; ++it) {
            out << XenCommon::toString(it->first) << '\t' << XenCommon::toString0(it->second) << std::endl;
            
            if (out.bad())
                throw XenCommon::XenCEption("Error while writing file " + distName);
        }
        
        out.close();
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

void XenIO::dumpSimilarity(boost::shared_ptr<Corpus> ptrCorp, boost::shared_ptr<Similarity> ptrSim) {
    try {
        std::string outName = ptrCorp->getXenFile()->getPrefix() + ".sim";
        
        std::ofstream out(outName.c_str(), std::ios::out | std::ios::trunc);
        
        if (!out.is_open())
            throw XenCommon::XenCEption("Can't open " + outName + " for writing.");
        
        for (unsigned int i = 0; i < ptrCorp->getSize(); i++) {
            out << ptrSim->getSim(i) << '\t' << ptrCorp->getLine(i) << std::endl;
            
            if (out.bad())
                throw XenCommon::XenCEption("Error while writing file " + outName);
        }
        
        out.close();
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}

std::vector<std::string> XenIO::read(boost::shared_ptr<XenFile> ptrFile) {
    std::string line;
    std::vector<std::string> ret;
    
    std::cout << "Reading file " + ptrFile->getFullPath() << std::endl;
    
    try {
        if (!ptrFile->isGZ()) {
            std::ifstream f (ptrFile->getFullPath().c_str());
            
            if (!f.is_open())
                throw XenCommon::XenCEption("Error while opening file " + ptrFile->getFullPath());
            
            while(std::getline(f, line))
                ret.push_back(line);
            
            if (f.bad())
                throw XenCommon::XenCEption("Error while reading file " + ptrFile->getFullPath());
            
            f.close();
        }
        else {
            std::ifstream f(ptrFile->getFullPath().c_str(), std::ios_base::in | std::ios_base::binary);
            
            if (!f.is_open())
                throw XenCommon::XenCEption("Error while opening file " + ptrFile->getFullPath());
            
            try {
                boost::iostreams::filtering_istream in;
                in.push(boost::iostreams::gzip_decompressor());
                in.push(f);
                
                while(std::getline(in, line))
                    ret.push_back(line);
                
                if (f.bad())
                    throw XenCommon::XenCEption("Error while reading file " + ptrFile->getFullPath());
                
                f.close();
            }
            catch(boost::iostreams::gzip_error &e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    catch (XenCommon::XenCEption &e) {
        throw;
    }
    
    std::cout << "Done reading file " + ptrFile->getFullPath() << std::endl;
    
    return ret;
}

boost::shared_ptr<EvalMap> XenIO::readDist(std::string distFile) {
    boost::shared_ptr<EvalMap> ret = boost::make_shared<EvalMap>();
    
    try {
        if (boost::filesystem::exists(distFile.c_str())) {
            std::cout << "Loading dist file " + distFile << std::endl;
            boost::regex e1("(.*)\\t.*");
            boost::regex e2(".*\\t(.*)");
            std::ifstream in(distFile.c_str(), std::ios::in);
            std::string line = "";
            
            if (!in.is_open())
                throw XenCommon::XenCEption("Error while opening file " + distFile);
            
            while (std::getline(in, line)) {
                std::string key = boost::regex_replace(line, e1, "\\1", boost::match_default | boost::format_sed);
                std::string value = boost::regex_replace(line, e2, "\\1", boost::match_default | boost::format_sed);
                ret->operator[](XenCommon::toInt(key)) = XenCommon::toDouble(value);
            }
            
            if (in.bad())
                throw XenCommon::XenCEption("Error while reading file " + distFile);
            
            in.close();
            
            return ret;
        }
        else {
            throw XenCommon::XenCEption("Specified eval file " + distFile + " does not exists!");
        }
    } catch (XenCommon::XenCEption &e) {
        throw;
    }
}
