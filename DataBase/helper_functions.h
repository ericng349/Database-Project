#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <string>
#include <iostream>

/**
 * @brief toLower - converts a sentence to a lower case format
 * @param sentence - the sentence to make lower case
 * @return  - a lower cased version of the sentece
 */

std::string toLower(std::string sentence)
{
    for(int i = 0; i < sentence.size();i++)
        sentence[i] = tolower(sentence[i]);

    return sentence;
}

#endif // HELPER_FUNCTIONS_H
