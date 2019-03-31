/*
 Copyright (C) 2016 Vladimir "allejo" Jimenez

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "bzfsAPI.h"
#include "plugin_utils.h"

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));

    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    return select_randomly(start, end, gen);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }

    return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);

    return elems;
}

class AOLer : public bz_Plugin
{
public:
    virtual const char* Name () {return "AOLer";}
    virtual void Init (const char* config);
    virtual void Event (bz_EventData *eventData);
    virtual void Cleanup (void);

    virtual void InitDictionary();
    virtual std::string aoler(const char* message);

    typedef std::vector<std::string> strVector;

    std::map<std::string, strVector> multiWordTranslation;
    std::map<std::string, std::string> phraseTranslation;
    std::map<std::string, std::string> wordTranslations;
};

BZ_PLUGIN(AOLer)

void AOLer::InitDictionary()
{
    phraseTranslation["what the fuck"] = "wtf";
    phraseTranslation["what the hell"] = "wth";
    phraseTranslation["what the heck"] = "wth";
    phraseTranslation["oh my god"]     = "omg";
    phraseTranslation["be right back"] = "brb";

    wordTranslations["your"]    = "ur";
    wordTranslations["you"]     = "u";
    wordTranslations["what"]    = "wut";
    wordTranslations["are"]     = "r";
    wordTranslations["why"]     = "y";
    wordTranslations["because"] = "b/c";
    wordTranslations["oh"]      = "o";
    wordTranslations["my"]      = "mah";
    wordTranslations["new"]     = "nu";
    wordTranslations["with"]    = "wit";
    wordTranslations["really"]  = "rilly";
    wordTranslations["please"]  = "plz";
    wordTranslations["thanks"]  = "thx";
    wordTranslations["ok"]      = "k";
    wordTranslations["okay"]    = "k";
    wordTranslations["library"] = "liberry";
    wordTranslations["be"]      = "b";
    wordTranslations["schoool"] = "skool";
    wordTranslations["hi"]      = "hai";
    wordTranslations["noob"]    = "newb";
    wordTranslations["hack"]    = "h4x";
    wordTranslations["lag"]     = "h4x";
    wordTranslations["cheat"]   = "chiet";
    wordTranslations["that"]    = "taht";
    wordTranslations["people"]  = "ppl";
    wordTranslations["sorry"]   = "sowwy";

    multiWordTranslation["to"].push_back("two");
    multiWordTranslation["to"].push_back("too");
    multiWordTranslation["to"].push_back("2");

    multiWordTranslation["two"].push_back("to");
    multiWordTranslation["two"].push_back("too");
    multiWordTranslation["two"].push_back("2");

    multiWordTranslation["too"].push_back("to");
    multiWordTranslation["too"].push_back("two");
    multiWordTranslation["too"].push_back("2");

    multiWordTranslation["the"].push_back("teh");
    multiWordTranslation["the"].push_back("da");

    multiWordTranslation["their"].push_back("there");
    multiWordTranslation["their"].push_back("they're");

    multiWordTranslation["there"].push_back("their");
    multiWordTranslation["there"].push_back("they're");

    multiWordTranslation["they're"].push_back("their");
    multiWordTranslation["they're"].push_back("there");
}

void AOLer::Init (const char* commandLine)
{
    InitDictionary();

    // Register our events with Register()
    Register(bz_eRawChatMessageEvent);
}

void AOLer::Cleanup (void)
{
    Flush(); // Clean up all the events
}

void AOLer::Event (bz_EventData *eventData)
{
    switch (eventData->eventType)
    {
        case bz_eRawChatMessageEvent: // This event is called for each chat message the server receives. It is called before any filtering is done.
        {
            bz_ChatEventData_V1* chatData = (bz_ChatEventData_V1*)eventData;

            if (rand() % 100 < 65)
            {
                chatData->message = aoler(chatData->message.c_str());
            }
        }
        break;

        default: break;
    }
}

std::string AOLer::aoler(const char* _message)
{
    int i = 0;
    int wordsToSkip = 0;
    std::string message = bz_tolower(_message);
    std::string output;

    strVector sentence = split(message, ' ');

    for (auto &word : sentence)
    {
        if (wordsToSkip > 0)
        {
            wordsToSkip--;
            continue;
        }

        if (sentence.size() >= 3 && i + 3 <= sentence.size())
        {
            std::string first = word;
            std::string second = sentence.at(i + 1);
            std::string third = sentence.at(i + 2);
            std::string phrase = first + " " + second + " " + third;

            if (phraseTranslation.count(phrase))
            {
                output += phraseTranslation[phrase];
                wordsToSkip = 3;

                continue;
            }
        }

        if (wordTranslations.count(word))
        {
            output += wordTranslations[word];
        }
        else if (multiWordTranslation.count(word))
        {
            output += *select_randomly(multiWordTranslation[word].begin(), multiWordTranslation[word].end());
        }
        else
        {
            int j = 0;
            int lettersToSkip = 0;

            for (char &c : word)
            {
                if (lettersToSkip > 0)
                {
                    lettersToSkip--;
                    continue;
                }

                if ((j + 1 < word.length()) && c == word.at(j + 1))
                {
                    output += c;
                    lettersToSkip = 1;
                }
                else if ((j + 1 < word.length()) && c == 'b' && word.at(j + 1) == 'e') // Replace 'be' with 'b'
                {
                    output += c;
                    lettersToSkip = 1;
                }
                else if ((j + 1 < word.length()) && c == 'c' && word.at(j + 1) == 'k') // Replace 'ck' with 'k'
                {
                    output += 'k';
                    lettersToSkip = 1;
                }
                else if (c == 'e') // Replace e's randomly
                {
                    if      ((rand() * 100) % 3 == 2) { output += '3'; }
                    else if ((rand() * 100) % 3 == 1) { output += 'a'; }
                    else                              { output += 'e'; }
                }
                else if (c == 'i')
                {
                    // Replace '-ing' with 'ng'
                    if ((j + 2 < word.length()) && word.at(j + 1) == 'n' && word.at(j + 2) == 'g')
                    {
                        output += "ng";
                        lettersToSkip = 2;
                    }
                    // Replace 'i_e' with 'ie_'; e.g. 'like' => 'liek'
                    else if ((j + 2 < word.length()) && word.at(j + 2) == 'e')
                    {
                        output += "ie";
                        output += word.at(j + 1);
                        lettersToSkip = 2;
                    }
                    // Replace 'ie' with 'ei'
                    else if ((j + 1 < word.length()) && word.at(j + 1) == 'e')
                    {
                        output += "ei";
                        lettersToSkip = 1;
                    }
                    else
                    {
                        output += c;
                    }
                }
                else if (c == 'a')
                {
                    if ((j + 1 < word.length()) && word.at(j + 1) == 'm')
                    {
                        output += "m";
                        lettersToSkip = 1;
                    }
                    else if ((j + 2 < word.length()) && word.at(j + 1) == 'l' && word.at(j + 2) == 'k')
                    {
                        output += "ok";
                        lettersToSkip = 2;
                    }
                    else if ((j + 1 < word.length()) && word.at(j + 1) == 'i')
                    {
                        output += "a";
                        output += word.at(j + 2);
                        output += "e";
                        lettersToSkip = 2;
                    }
                    else if ((j + 3 < word.length()) && word.at(j + 1) == 't' && word.at(j + 2) == 'e' && word.at(j + 3) == 'r')
                    {
                        output += "8r";
                        lettersToSkip = 3;
                    }
                    else if ((j + 2 < word.length()) && word.at(j + 2) == 'e')
                    {
                        output += "ae";
                        output += word.at(j + 1);
                        lettersToSkip = 2;
                    }
                    else
                    {
                        output += c;
                    }
                }
                else if (c == 'o')
                {
                    if ((j + 1 < word.length()) && word.at(j + 1) == 'o')
                    {
                        output += "u";
                        lettersToSkip = 1;
                    }
                    else
                    {
                        if      (rand() % 3 == 2) { output += "0"; }
                        else if (rand() % 3 == 1) { output += "ooo"; }
                        else                              { output += "o"; }
                    }
                }
                else if (c != '.' && c != '!' && c != '?' && c != '\'' && c != ';' && c != ',' && c != ':' && c != '"' && c != '`' && c != '~')
                {
                    output += c;
                }
            }
        }

        output += " ";
        i++;
    }

    if (rand() % 2) { output += "OMG"; }
    if (rand() % 2) { output += "ROFL"; }
    if (rand() % 2) { output += "LOL"; }

    if (rand() % 100 < 33 || message.find("!") != std::string::npos)
    {
        int random = (rand() % 10) + 3;

        for (int k = 0; k < random; k++)
        {
            output += (rand() % 2) ? "!" : "1";
        }
    }
    else if (rand() % 100 < 33 || message.find("?") != std::string::npos)
    {
        int random = (rand() % 10) + 3;

        for (int k = 0; k < random; k++)
        {
            output += (rand() % 2) ? "?" : "!";
        }
    }

    return bz_toupper(output.c_str());
}
