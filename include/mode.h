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
 * $Id: mode.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Abstract class to define filtering modes architecture
 */

#ifndef MODE_H_
#define MODE_H_

#include "utils/common.h"
#include "utils/xenio.h"
#include "utils/StaticData.h"

#include "xenoption.h"

class Mode {
    public:
        virtual int launch() = 0;
        virtual ~Mode() = 0;
    
    protected:
        static void findSampleSize(shared_ptr<Corpus>, shared_ptr<Corpus>);
        static Corpus extractSample(shared_ptr<Corpus>, int, bool);
};

#endif
