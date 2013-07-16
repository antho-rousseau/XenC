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
 * $Id: xenio.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "xenio.h"
#include "corpus.h"
#include "phrasetable.h"

//  ------
//  Public
//  ------

/*
 *  Checks for empty lines in a monolingual corpus
 *  and marks them as "removed" if needed
 */
void XenIO::cleanCorpusMono(shared_ptr<Corpus> c, shared_ptr<Score> sc) {
    cout << "Cleaning monolingual output..." << endl;
    regex e("^\\s*$");
    
    for (unsigned int i = 0; i < c->getSize(); i++) {
		if (regex_search(c->getLine(i), e, boost::match_default | boost::format_sed)) {
            c->removeLine(i);
            sc->removeScore(i);
        }
	}
    cout << "Monolingual output cleaned." << endl;
}

/*
 *  Checks for empty lines in a bilingual corpus
 *  and marks them as "removed" if needed
 */
void XenIO::cleanCorpusBi(shared_ptr<Corpus> c1, shared_ptr<Corpus> c2, shared_ptr<Score> sc) {
    cout << "Cleaning bilingual output..." << endl;
    regex e("^\\s*$");
    
    for (unsigned int i = 0; i < c1->getSize(); i++) {
        if (regex_search(c1->getLine(i), e, boost::match_default | boost::format_sed) || regex_search(c2->getLine(i), e, boost::match_default | boost::format_sed)) {
            c1->removeLine(i);
            c2->removeLine(i);
            sc->removeScore(i);
        }
    }
    cout << "Bilingual output cleaned." << endl;
}

/*
 *  Writes a monolingual scored/sorted file (gzipped)
 */
void XenIO::writeMonoOutput(shared_ptr<Corpus> c, shared_ptr<Score> sc) {
    XenOption* opt = XenOption::getInstance();
    
	string scoredName = opt->getOutName() + ".scored.gz";
	string sortedName = opt->getOutName() + ".sorted.gz";
    
    multimap<double, string> sortMap;
    
    for (unsigned int i = 0; i < c->getSize(); i++)
        if (c->getPrint(i) && sc->getPrint(i)) {
            pair<double, string> p(sc->getScore(i), c->getLine(i));
            sortMap.insert(p);
        }
    
    if (!opt->getSortOnly()) {
        cout << "Writing scored output to " + scoredName << endl;

        filtering_ostream out;
        out.push(gzip_compressor());
        out.push(file_sink(scoredName.c_str(), ios_base::out | ios_base::binary));
        out.setf(ios::fixed | ios::showpoint);
        out.precision(15);

        for (unsigned int i = 0; i < c->getSize(); i++)
            if (c->getPrint(i) && sc->getPrint(i))
                out << toString(sc->getScore(i)) << '\t' << c->getLine(i) << endl;

        out.flush();
        out.reset();
    }

	cout << "Writing sorted output to " + sortedName << endl;
    
    filtering_ostream out;
    out.push(gzip_compressor());
    out.push(file_sink(sortedName.c_str(), ios_base::out | ios_base::binary));
    out.setf(ios::fixed | ios::showpoint);
    out.precision(15);
    
    for (multimap<double, string>::iterator it = sortMap.begin(); it != sortMap.end(); ++it)
        out << toString(it->first) << '\t' << it->second << endl;
    
    out.flush();
    out.reset();
}

/*
 *  Writes a bilingual scored/sorted file (gzipped)
 */
void XenIO::writeBiOutput(shared_ptr<Corpus> sCorp, shared_ptr<Corpus> tCorp, shared_ptr<Score> sc) {
    XenOption* opt = XenOption::getInstance();
    
	string scoredName = opt->getOutName() + ".scored.gz";
	string sortedName = opt->getOutName() + ".sorted.gz";
    
    multimap<double, string> sortMap;
    
    for (unsigned int i = 0; i < sCorp->getSize(); i++)
        if (sCorp->getPrint(i) && tCorp->getPrint(i) && sc->getPrint(i)) {
            pair<double, string> p(sc->getScore(i), sCorp->getLine(i) + '\t' + tCorp->getLine(i));
            sortMap.insert(p);
        }
    
    if (!opt->getSortOnly()) {
        cout << "Writing scored output to " + scoredName << endl;

        filtering_ostream out;
        out.push(gzip_compressor());
        out.push(file_sink(scoredName.c_str(), ios_base::out | ios_base::binary));
        out.setf(ios::fixed | ios::showpoint);
        out.precision(15);

        for (unsigned int i = 0; i < sCorp->getSize(); i++)
            if (sCorp->getPrint(i) && tCorp->getPrint(i) && sc->getPrint(i))
                out << toString(sc->getScore(i)) << '\t' << sCorp->getLine(i) << '\t' << tCorp->getLine(i) << endl;

        out.flush();
        out.reset();
    }
    
	cout << "Writing sorted output to " + sortedName << endl;
    
    filtering_ostream out;
    out.push(gzip_compressor());
    out.push(file_sink(sortedName.c_str(), ios_base::out | ios_base::binary));
    out.setf(ios::fixed | ios::showpoint);
    out.precision(15);
    
    for (multimap<double, string>::iterator it = sortMap.begin(); it != sortMap.end(); ++it)
        out << toString(it->first) << '\t' << it->second << endl;
    
    out.flush();
    out.reset();
}

/*
 *  Writes a new modified/scored phrase table
 */
void XenIO::writeNewPT(shared_ptr<PhraseTable> pT, shared_ptr<Score> sc) {
    XenOption* opt = XenOption::getInstance();
    
    Score sc2;
    if (opt->getLocal()) {
        vector<SourcePhrase> srcPh = pT->getSrcPhrases();
    
        for (unsigned int i = 0; i < srcPh.size(); i++) {
            SourcePhrase sP = srcPh[i];
        
            for (unsigned int j = 0; j < sP.getScoresXE()->getSize(); j++) {
                sc2.addScore(sP.getScoresXE()->getScore(j));
            }
        }
    }
    
    cout << "Writing new phrase-table to " + opt->getOutName() << endl;
    string oF = opt->getOutName();
    filtering_ostream out;
    out.push(gzip_compressor());
    out.push(file_sink(oF.c_str(), ios_base::out | ios_base::binary));
    
    if (!out) {
        cout << "Can't write to " << opt->getOutName() + ".gz" << endl;
		exit (1);
    }
    else {
        for (unsigned int i = 0; i < sc->getSize(); i++) {
            out << pT->getSource(i) << " ||| " << pT->getTarget(i) << " ||| " << pT->getScores(i) << " " << toString(sc->getScore(i));
            if (opt->getLocal())
                out << " " << toString(sc2.getScore(i));
            out << " ||| " << pT->getAlignment(i) << " ||| " << pT->getCounts(i) << endl;
        }
    }
    
    out.flush();
    out.reset();
}

/*
 *  Writes the sources phrases from a phrase table
 */
string XenIO::writeSourcePhrases(shared_ptr<PhraseTable> pT) {
    string outName = pT->getXenFile()->getPrefix() + "-source";
    
    if (exists(outName.c_str())) {
        cout << "Source phrases of table " << pT->getXenFile()->getFileName() << " already dumped, we continue." << endl;
    }
    else {
        ofstream out(outName.c_str(), ios::out | ios::trunc);
    
        cout << "Writing source phrases of table " << pT->getXenFile()->getFileName() << "." << endl;
    
        for(unsigned int i = 0; i < pT->getSize(); i++)
            out << pT->getSource(i) << endl;
        
        out.close();
    
        cout << "Done writing source phrases." << endl;
    }
    
    return outName;
}

/*
 *  Writes the target phrases from a phrase table
 */
string XenIO::writeTargetPhrases(shared_ptr<PhraseTable> pT) {
    string outName = pT->getXenFile()->getPrefix() + "-target";
    
    if (exists(outName.c_str())) {
        cout << "Target phrases of table " << pT->getXenFile()->getFileName() << " already dumped, we continue." << endl;
    }
    else {
        ofstream out(outName.c_str(), ios::out | ios::trunc);
    
        cout << "Writing target phrases of table " << pT->getXenFile()->getFileName() << "." << endl;
    
        for(unsigned int i = 0; i < pT->getSize(); i++)
            out << pT->getTarget(i) << endl;
        
        out.close();
    
        cout << "Done writing target phrases." << endl;
    }
    
    return outName;
}

/*
 *  Writes an eval/best point file
 */
void XenIO::writeEval(shared_ptr<Eval> ptrEval, string distName) {
    ofstream out(distName.c_str(), ios::out | ios::trunc);
    
    EvalMap em = ptrEval->getDist();
    
    for(EvalMap::iterator it = em.begin() ; it != em.end() ; ++it)
        out << toString(it->first) << '\t' << toString0(it->second) << endl;
    
	out.close();
}

/*
 *  Dumps the similarity scores of a corpus
 *  FOR DEBUG/TESTING
 */
void XenIO::dumpSimilarity(shared_ptr<Corpus> c, shared_ptr<Similarity> sim) {
    string outName = c->getXenFile()->getPrefix() + ".sim";
    
    ofstream out(outName.c_str(), ios::out | ios::trunc);
    
    for (unsigned int i = 0; i < c->getSize(); i++) {
        out << sim->getSim(i) << '\t' << c->getLine(i) << endl;
    }
    
    out.close();
}

/*
 *  Reads a file/gzipped file into memory
 */
vector<string> XenIO::read(shared_ptr<XenFile> file) {
    string line;
    vector<string> ret;
    
    cout << "Reading file " + file->getFullPath() << endl;
    
    if (!file->isGZ()) {
        ifstream f (file->getFullPath().c_str());
    
        if (!f.is_open()) {
            cout << "Error while opening file " + file->getFullPath() << endl;
            exit(1);
        }

        while(getline(f, line))
            ret.push_back(line);

        if (f.bad()) {
            cout << "Error while reading file " + file->getFullPath() << endl;
            exit(1);
        }
    
        f.close();
    }
    else {
        ifstream f(file->getFullPath().c_str(), std::ios_base::in | std::ios_base::binary);
        
        if (!f.is_open()) {
            cout << "Error while opening file " + file->getFullPath() << endl;
            exit(1);
        }
        
        try {
            filtering_istream in;
            in.push(gzip_decompressor());
            in.push(f);
            
            while(getline(in, line))
                ret.push_back(line);
            
            if (f.bad()) {
                cout << "Error while reading file " + file->getFullPath() << endl;
                exit(1);
            }
            
            f.close();
        }
        catch(const gzip_error& e) {
            cout << e.what() << endl;
            exit(1);
        }
    }
    
    cout << "Done reading file " + file->getFullPath() << endl;
    
    return ret;
}
