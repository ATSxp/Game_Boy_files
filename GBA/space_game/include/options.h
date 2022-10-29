#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include "void.h"
#include "player.h"
#include "texts_en.h"
#include "texts_ptbr.h"

using namespace std;

extern u16 LANGUAGE;
#define LANGUAGE_EN 0
#define LANGUAGE_PTBR 1

INLINE string translTxt( string txt_en, string txt_ptbr ){
    if( LANGUAGE == LANGUAGE_PTBR ){
        return txt_ptbr;
    }
    return txt_en;
}

#endif // OPTIONS_H
