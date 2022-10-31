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

#define translTxt(text)((LANGUAGE)==(LANGUAGE_PTBR) ? (PT_##text) : (EN_##text) )

#endif // OPTIONS_H
