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
 * $Id: common.h, v 1.0 PUBLIC RELEASE 2013/07/16 rousseau Exp $
 */

/*
 *  Class to handle some basic things in XenC
 */

#ifndef COMMON_H_
#define COMMON_H_

// Common includes
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

// XenC options structure
typedef struct _Options {
	string sLang;
	string tLang;
	string inSData;
	string outSData;
    string inTData;
    string outTData;
    string inSStem;
	string outSStem;
    string inTStem;
    string outTStem;
    string iPTable;
    string oPTable;
	int mode;
    bool mean;
    bool sim;
    bool simOnly;
    int vecSize;
	string sVocab;
	string tVocab;
	string inSLM;
	string outSLM;
	string inTLM;
	string outTLM;
	string wFile;
	string dev;
	int order;
	string cutoff;
	string smooth;
    int binLM;
	int sampleSize;
	bool log;
	bool rev;
    bool inv;
	bool mono;
    bool stem;
    bool local;
	bool eval;
	bool bp;
	int step;
	int pc;
	int inToks;
	int outToks;
    string outName;
    string name;
    bool version;
    int threads;
    bool sortOnly;
} Options, *LPOptions;

/*
 *  Converts any basic type to string
 */
template<typename T> string toString(const T &Value) {
	ostringstream oss;
	oss.precision(20);
	oss << Value;
	return oss.str();
}

/*
 *  Converts any basic type to string with no precision for numbers
 */
template<typename T> string toString0(const T &Value) {
	ostringstream oss;
	oss << Value;
	return oss.str();
}

/*
 *  Converts any type to integer (usually for strings)
 */
template<typename T> int toInt(const T &Value) {
	int ret;
	istringstream iss;
	iss.str(Value);
	iss >> ret;
	return ret;
}

/*
 *  Converts any type to double (usually for strings)
 */
template<typename T> double toDouble(const T &Value) {
	double ret;
	istringstream iss;
	iss.str(Value);
	iss >> ret;
	return ret;
}

/*
 *  Flips a pair of objets in a multimap
 */
template<typename A, typename B> pair<B, A> flip_pair(const pair<A, B> &p) {
    return pair<B,A>(p.second, p.first);
}

/*
 *  Flips a multimap
 */
template<typename A, typename B> multimap<B, A, greater<B> > flip_map(const map<A, B> &src) {
    multimap<B, A, greater<B> > dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()), flip_pair<A, B>);
    return dst;
}

/*
 *  Common functions
 */
namespace XenCommon {
    /*
     *  Exception handling
     */
    struct XenCEption : public std::exception
    {
        string s;
        XenCEption(string ss) : s(ss) {}
        virtual ~XenCEption() throw() {}
        const char* what() const throw() { return s.c_str(); }
    };
    
    /*
     *  Returns the word count of a string
     */
    inline int wordCount(const string& str) {
        int c = 0;
        string::const_iterator pos = str.begin();
        string::const_iterator end = str.end();
        
        while(pos != end) {
            while(pos != end && (*pos == ' ' || isspace(*pos))) ++pos;
            c += (pos != end);
            while(pos != end && (*pos != ' ' || !isspace(*pos))) ++pos;
        }
        
        return c;
    }
    
    /*
     *  Grabs the output from a system command executed within XenC
     *  (not so classy but hey!)
     */
    inline string getStdoutFromCommand(string cmd) {
        try {
            string data;
            FILE *stream;
            int MAX_BUFFER = 65535;
            char buffer[MAX_BUFFER];
            cmd.append(" 2>&1");
            stream = popen(cmd.c_str(), "r");
            
            if (!stream){
                throw XenCEption("Can't open stream with popen()!");
            }
            
            while (!feof(stream)){
                if (fgets(buffer, MAX_BUFFER, stream) != NULL){
                    data.append(buffer);
                }
            }
            
            pclose(stream);
            return data;
        }
        catch (XenCEption &e) {
            throw;
        }
        
        return "";
    }
}

/*
 *  Class to split a string into vector of string,
 *  given a potentially multi-character delimiter
 *  (like "|||" in a phrase table for instance)
 */
class Splitter {
    vector<string> _tokens;
    
    public:
        typedef vector<string>::size_type size_type;

        Splitter() { }
        Splitter(const string& src, const string& delim) { reset (src, delim); }
        string& operator[] (size_type i) { return _tokens.at(i); }
        size_type size() const { return _tokens.size(); }

        void reset(const string& src, const string& delim) {
            vector<string> tokens;
            string::size_type start = 0;
            string::size_type end;
        
        for ( ; ; ) {
            end = src.find(delim, start);
            tokens.push_back(src.substr(start, end - start));

            if (end == string::npos)
                break;

            start = end + delim.size();
        }
        
        _tokens.swap (tokens);
    }
};

#endif
