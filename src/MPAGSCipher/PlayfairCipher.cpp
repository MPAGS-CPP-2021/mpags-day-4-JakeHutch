#include "PlayfairCipher.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>

PlayfairCipher::PlayfairCipher(const std::string& key) : key_{"A"}
{
    PlayfairCipher::setKey(key);
}

void PlayfairCipher::setKey( const std::string& key)
{
    // store the original key
    key_ = key;

    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(key_.begin() , key_.end(), key_.begin(), ::toupper );
    // Remove non-alpha characters
    auto key_iter = std::remove_if(key_.begin(), key_.end(), [] (char key_letter) { 
        return !std::isalpha(key_letter);
        });
    key_.erase(key_iter, key_.end());
    // Change J -> I
    std::transform(key_.begin() , key_.end(), key_.begin(), [] (char key_letter) {
        if (key_letter == 'J') { key_letter = 'I' ;}
        return key_letter ;
    } );
    // Remove duplicated letters
    std::string lettersFound {""};
    auto key_enc = std::remove_if(key_.begin(), key_.end(), [&lettersFound] (char key_letter) { 
        if (lettersFound.find(key_letter) != std::string::npos) {
            return true ;
        } else {
            lettersFound += key_letter;
            return false;
        }
        });
    key_.erase(key_enc, key_.end());

    // Store the coords of each letter
    //using coord = std::pair<int, int>; (this is in .hpp file)
    std::map <coord, char> coordToLetter ;
    std::map <char, coord> letterToCoord ;
    for (std::size_t i{0}; i < key_.size(); ++i) {
        std::pair <coord, char> coordLetter { {i%5 , i/5 } , key_[i]};
        std::pair <char, coord> letterCoord { key_[i] , {i%5 , i/5 } };
        coordToLetter.insert( coordLetter) ;
        letterToCoord.insert( letterCoord) ;
    }

    // Store the playfair cihper key map
    letterToCoord_ = letterToCoord ;
    coordToLetter_ = coordToLetter ;

    // for ( auto p : letterToCoord ){
    //     std::cout << p.first << ": [" << p.second.first << p.second.second << "]" << std::endl;
    // }

}


std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) const
{
    std::string text {inputText};

    // Change J → I
    std::transform(text.begin() , text.end(), text.begin(), [] (char inLetter) {
        if (inLetter== 'J') { inLetter = 'I' ;}
        return inLetter ;
    } );
    //std::cout << "Modified input : " << text << std::endl;

//    If repeated chars in a digraph add an X or Q if XX

    // for (auto iter {text.begin()}; iter != text.end() ; ++iter){
    //     if (*iter == *(iter-1)){
    //         char charToAdd {'X'};
    //         if (*iter == 'X'){
    //             charToAdd='Q';
    //         }
    //         text.insert( iter, 1, charToAdd);
    //         iter++ ;
    //     }        
    // }
    if (cipherMode ==CipherMode::Encrypt){
            
        for (std::size_t i {0}; i < text.size() ; ++i){
            if (text[i] == text[i-1]){
                char charToAdd {'X'};
                if (text[i] == 'X'){
                    charToAdd='Q';
                }
                text.insert( i, 1, charToAdd);
                i++ ;
            }        
        }
    }

    //std::cout << "Modified input : " << text << std::endl;

    // if the size of input is odd, add a trailing Z
    if (text.size()%2 == 1){
        text += 'Z';
    }
    //std::cout << "Modified input : " << text << std::endl;

    // Loop over the input in Digraphs
    coord coord1; 
    coord coord2;
    coord newCoord1;
    coord newCoord2;
    std::string outputText {""};
    for (auto txtIter {text.begin()}; txtIter != text.end() ; txtIter+=2){
        // - Find the coords in the grid for each digraph
        auto iter1 { letterToCoord_.find (*txtIter) };
        auto iter2 { letterToCoord_.find (*(txtIter+1)) };
        coord1 = (*iter1).second ; 
        coord2 = (*iter2).second ; 

        // - Apply the rules to these coords to get 'new' coords
        
        if (coord1.first == coord2.first){ // if they're on the same column:

            if ( cipherMode==CipherMode::Encrypt ){
                newCoord1.first = coord1.first ;
                newCoord2.first = coord2.first ;
                newCoord1.second = (coord1.second +1) % 5;
                newCoord2.second = (coord2.second +1) % 5;

            }else {
                newCoord1.first = coord1.first ;
                newCoord2.first = coord2.first ;
                newCoord1.second = (coord1.second -1 +5) % 5;
                newCoord2.second = (coord2.second -1 +5) % 5;                
            }

        }else if (coord1.second == coord2.second){ // if they're on the same row:

            if ( cipherMode==CipherMode::Encrypt ){
                newCoord1.first = (coord1.first +1) % 5;
                newCoord2.first = (coord2.first +1) % 5;                
                newCoord1.second = coord1.second ;
                newCoord2.second = coord2.second ;

            }else {
                newCoord1.first = (coord1.first -1 +5) % 5;
                newCoord2.first = (coord2.first -1 +5) % 5;                
                newCoord1.second = coord1.second ;
                newCoord2.second = coord2.second ;
            }

        }else { // if they form a rectangle
            // I think this is the same for encrypt and decrypt
            newCoord1.first = coord2.first;  // the column becomes the same column as the other coord
            newCoord1.second = coord1.second; // the row stays the same
            newCoord2.first = coord1.first;
            newCoord2.second = coord2.second;
        }
        // - Find the letter associated with the new coords
        auto iter3 {coordToLetter_.find(newCoord1)};
        auto iter4 {coordToLetter_.find(newCoord2)};

        //std::cout << (*iter3).second <<std::endl;
        //std::cout << (*iter4).second <<std::endl;
        // return the text
        outputText += (*iter3).second;
        outputText += (*iter4).second;
    }

    return outputText;
}