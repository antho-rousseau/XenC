/**
 *  @file common.h
 *  @brief File containing all common classes/structures/functions of many classes of XenC
 *  @author Anthony Rousseau
 *  @version 1.0.0
 *  @date 27 July 2013
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

#ifndef COMMON_H_
#define COMMON_H_

//!< Common includes
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

/**
 *  @struct _Options common.h "utils/common.h"
 *  @brief XenC options structure
 */
typedef struct _Options {
    std::string sLang;      //!< The source language
	std::string tLang;      //!< The target language
	std::string inSData;    //!< The in-domain source corpus
	std::string outSData;   //!< The out-of-domain source corpus
    std::string inTData;    //!< The in-domain target corpus
    std::string outTData;   //!< The out-of-domain target corpus
    std::string inSStem;    //!< The in-domain source stem corpus
	std::string outSStem;   //!< The out-of-domain source stem corpus
    std::string inTStem;    //!< The in-domain target stem corpus
    std::string outTStem;   //!< The out-of-domain source stem corpus
    std::string iPTable;    //!< The in-domain phrase-table
    std::string oPTable;    //!< The out-of-domain phrase-table
	int mode;               //!< The filtering mode
    bool mean;              //!< Indicates mean computation
    bool sim;               //!< Indicates similarity computation
    bool simOnly;           //!< Indicates similarity computation only
    int vecSize;            //!< The vector size for similarity
	std::string sVocab;     //!< The source language vocabulary
	std::string tVocab;     //!< The target language vocabulary
	std::string inSLM;      //!< The in-domain source language model
	std::string outSLM;     //!< The out-of-domain source language model
	std::string inTLM;      //!< The in-domain target language model
	std::string outTLM;     //!< The out-of-domain target language model
	std::string wFile;      //!< The weight file
	std::string dev;        //!< The development corpus (for evaluation)
	int order;              //!< The order for language models estimation
    int discount;           //!< The discounting method for language models estimation
    int binLM;              //!< The language models output format (0 = ARPA, 1 = binary)
	int sampleSize;         //!< The sample size for the out-of-domain corpus
	bool log;               //!< Indicates if the weights are given in the log domain
	bool rev;               //!< Indicates if a reversed output is requested (descending order)
    bool inv;               //!< Indicates if an inverse output is requested (1 - score)
	bool mono;              //!< Indicates if monolingual data is being filtered or not
    bool stem;              //!< Indicates stem computation
    bool local;             //!< Indicates local scores computation for phrase table filtering
	bool eval;              //!< Indicates evaluation mode
	bool bp;                //!< Indicates best-point evaluation mode
	int step;               //!< The step size for evaluation and best-point
	int pc;                 //!< The current percentage being evaluated
	int inToks;             //!< The number of in-domain tokens
	int outToks;            //!< The number of out-of-domain tokens
    std::string outName;    //!< The output file name
    std::string name;       //!< The program name
    bool version;           //!< The program version
    int threads;            //!< The number of threads
    bool sortOnly;          //!< Indicated outputting only the "sorted" file (not the "scored" one)
} Options, *LPOptions;

/**
 *  @namespace XenCommon
 *  @brief Namespace containing all the common functions of XenC
 */
namespace XenCommon {
    /**
     *  @brief Template converting a value into a string with a precision of 20
     *
     *  @tparam &Value :    the value to convert
     *  @return string containing the converted value
     */
    template<typename T> std::string toString(const T &Value) {
        std::ostringstream oss;
        oss.precision(20);
        oss << Value;
        return oss.str();
    }
    
    /**
     *  @brief Template converting a value into a string with no precision
     *
     *  @tparam &Value :    the value to convert
     *  @return string containing the converted value
     */
    template<typename T> std::string toString0(const T &Value) {
        std::ostringstream oss;
        oss << Value;
        return oss.str();
    }
    
    /**
     *  @brief Template converting a value (generally a string) into an integer
     *
     *  @tparam &Value :    the value to convert
     *  @return string containing the converted value
     */
    template<typename T> int toInt(const T &Value) {
        int ret;
        std::istringstream iss;
        iss.str(Value);
        iss >> ret;
        return ret;
    }
    
    /**
     *  @brief Template converting a value (generally a string) into an double
     *
     *  @tparam &Value :    the value to convert
     *  @return string containing the converted value
     */
    template<typename T> double toDouble(const T &Value) {
        double ret;
        std::istringstream iss;
        iss.str(Value);
        iss >> ret;
        return ret;
    }
    
    /**
     *  @brief Template flipping a pair key type with value type
     *
     *  @tparam &p :    the map pair<A, B> to flip
     *  @return flipped pair<B, A>
     */
    template<typename A, typename B> std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
        return std::pair<B,A>(p.second, p.first);
    }
    
    /**
     *  @brief Template flipping a multimap with descending order keys with values
     *
     *  @tparam &src :    the multimap to flip
     *  @return flipped multimap with descending order
     */
    template<typename A, typename B> std::multimap<B, A, std::greater<B> > flip_map(const std::map<A, B> &src) {
        std::multimap<B, A, std::greater<B> > dst;
        transform(src.begin(), src.end(), inserter(dst, dst.begin()), flip_pair<A, B>);
        return dst;
    }
    
    /**
     *  @struct XenCEption common.h "utils/common.h"
     *  @brief XenC exception structure
     */
    struct XenCEption : public std::exception
    {
        std::string s;      //!< The exception message
        
        /**
         *  @fn XenCEption (string ss)
         *  @brief Exception constructur
         */
        XenCEption(std::string ss) : s(ss) {}
        
        /**
         *  @fn virtual ~XenCEption () throw ()
         *  @brief Exception desctructor
         */
        virtual ~XenCEption() throw() {}
        
        /**
         *  @fn const char* what () const throw ()
         *  @brief Accessor to the exception message
         *
         *  @return the exception message
         */
        const char* what() const throw() { return s.c_str(); }
    };
    
    /**
     *  @fn inline int wordCount (const std::string &str)
     *  @brief Computes the word count of a string
     *
     *  @param  &str :  the string to count the words
     *  @return the number of words of the string
     */
    inline int wordCount(const std::string &str) {
        int c = 0;
        std::string::const_iterator pos = str.begin();
        std::string::const_iterator end = str.end();
        
        while(pos != end) {
            while(pos != end && (*pos == ' ' || std::isspace(*pos))) ++pos;
            c += (pos != end);
            while(pos != end && (*pos != ' ' || !std::isspace(*pos))) ++pos;
        }
        
        return c;
    }

    /**
     *  @fn inline std::string getStdoutFromCommand (std::string cmd)
     *  @brief Executes a system command and returns the output
     *
     *  @param  cmd :  the command to execute
     *  @return the output of the executed command
     */
    inline std::string getStdoutFromCommand(std::string cmd) {
        try {
            std::string data;
            std::FILE *stream;
            int MAX_BUFFER = 65535;
            char buffer[MAX_BUFFER];
            cmd.append(" 2>&1");
            stream = popen(cmd.c_str(), "r");
            
            if (!stream){
                throw XenCEption("Can't open stream with popen()!");
            }
            
            while (!feof(stream)){
                if (std::fgets(buffer, MAX_BUFFER, stream) != NULL){
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

    /**
     *  @class Splitter
     *  @brief Class defining a splitter
     *
     *  Class to split a string into vector of string,
     *  given a potentially multi-character delimiter
     *  (like "|||" in a phrase table for instance)
     */
    class Splitter {
        std::vector<std::string> _tokens;
        
    public:
        typedef std::vector<std::string>::size_type size_type;
        
        Splitter() { }
        Splitter(const std::string& src, const std::string& delim) { reset (src, delim); }
        std::string& operator[] (size_type i) { return _tokens.at(i); }
        size_type size() const { return _tokens.size(); }
        
        void reset(const std::string& src, const std::string& delim) {
            std::vector<std::string> tokens;
            std::string::size_type start = 0;
            std::string::size_type end;
            
            for ( ; ; ) {
                end = src.find(delim, start);
                tokens.push_back(src.substr(start, end - start));
                
                if (end == std::string::npos)
                    break;
                
                start = end + delim.size();
            }
            
            _tokens.swap (tokens);
        }
    };
}

#endif
