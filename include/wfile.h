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
 * $Id: wfile.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle a weight file in order to ponderate scores
 *  (confidence scores from unsup training for instance)
 */

#ifndef WFILE_H_
#define WFILE_H_

#include "utils/common.h"
#include "utils/xenio.h"

#include "xenfile.h"

class Wfile {
    public:
        Wfile();
        void initialize(shared_ptr<XenFile>);
        ~Wfile();
        // -----------------
        double getScore(int);
        unsigned int getSize() const;
    
    private:
        shared_ptr<XenFile> ptrFile;
        shared_ptr<vector<double> > ptrWeights;
        // --------------------
        void loadWeights();
        void calibrate();
};

#endif
