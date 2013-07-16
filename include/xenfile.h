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
 * $Id: xenfile.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle the files used in XenC
 *  and provide basic functions around them
 */

#ifndef XENFILE_H_
#define XENFILE_H_

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include "common.h"

using namespace boost;
using namespace boost::filesystem;

class XenFile {
    public:
        XenFile();
        void initialize(string);
        ~XenFile();
        // ----------------
        string getFileName() const;
        string getPrefix();
        string getExt();
        string getDirName() const;
        string getFullPath() const;
        bool isGZ() const;
    
    private:
        string fileName;
        string dirName;
};

#endif
