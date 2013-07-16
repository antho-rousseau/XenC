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
 * $Id: xenfile.cpp, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

#include "xenfile.h"

/*
 *  Default constructor
 */
XenFile::XenFile() : fileName(""), dirName("") {

}

/*
 *  Initialization from a string containing the file name
 */
void XenFile::initialize(string f) {
    path p(f);
    path fn = p.filename();
    fileName = fn.string();
    dirName = p.parent_path().string();
    
    if (dirName.compare("") == 0) { dirName = initial_path().string(); }
}

/*
 *  Destructor
 */
XenFile::~XenFile() {
    
}

//  ------
//  Public
//  ------

/*
 *  Returns the name of the file (only the file)
 */
string XenFile::getFileName() const {
    return fileName;
}

/*
 *  Returns the prefix of the file name
 *  (before the trailing dot)
 */
string XenFile::getPrefix() {
    regex e("(.*?)\\..*");
	string pre = regex_replace(fileName, e, "\\1", boost::match_default | boost::format_sed);

    return pre;
}

/*
 *  Returns the extension of the file name
 *  (after the trailing dot)
 */
string XenFile::getExt() {
    regex e(".*?\\.(.*)");
	string ext = regex_replace(fileName, e, "\\1", boost::match_default | boost::format_sed);
    
    return ext;
}

/*
 *  Returns the directory where the file is supposedly located
 */
string XenFile::getDirName() const {
    return dirName;
}

/*
 *  Returns the full path of the file
 */
string XenFile::getFullPath() const {
    return dirName + "/" + fileName;
}

/*
 *  Returns whether the file is gzipped or not,
 *  based on some magic number -- big & little endian compatible
 */
bool XenFile::isGZ() const {
    FILE* file;
    file = fopen(getFullPath().c_str(), "rb");
    
    unsigned char magicNumber[2];
    fread(&magicNumber, 2, 1, file);
    fclose(file);
    
    if((magicNumber[0] == 0x1f && magicNumber[1] == 0x8b) || (magicNumber[1] == 0x1f && magicNumber[0] == 0x8b))
        return true;
    else
        return false;
}
