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
 * $Id: corpus.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle corpus-related functionnalities
 */

#ifndef CORPUS_H_
#define CORPUS_H_

#include "utils/common.h"

#include "xenfile.h"

class XenIO;

class Corpus {
    public:
        Corpus();
        void initialize(shared_ptr<XenFile>, string);
        void initialize(string, string);
        ~Corpus();
        //--------------------
        XenFile* getXenFile();
        string getLine(int);
        unsigned int getSize();
        string getLang() const;
        bool getPrint(int);
        int getWC() const;
        void removeLine(int);
    
    private:
        string dir;
        string lang;
		shared_ptr<XenFile> ptrFile;
        shared_ptr<vector<string> > ptrText;
        shared_ptr<vector<int> > ptrPrint;
        int wc;
        //--------------
        int wordCount();
        void loadText();
};

#endif
