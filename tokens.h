#ifndef LEXICAL_ANALYZER_TOKENS_H
#define LEXICAL_ANALYZER_TOKENS_H


#include "includes/includes.h"


class KeywordsTree {
  struct Word {
    std::map<char, Word*> to;
    int64_t termCnt = 0;
    bool isTerm = false;
  };


public:
  bool find(std::string&);
  void insert(std::string&);


private:
  int64_t cnt = 0;
  Word* root = new Word();
};

bool KeywordsTree::find(std::string& str) {
  Word* v = root;

  for (auto ch : str) {
    if (!v->to[ch]) {
      return false;
    }
    v = v->to[ch];
  }

  return true;
}

void KeywordsTree::insert(std::string& str) {
  Word* v = root;

  ++v->termCnt;
  for (auto ch : str) {
    if (!v->to[ch]) {
      v->to[ch] = new Word();
    }
    v = v->to[ch];
    ++v->termCnt;
  }
  v->isTerm = true;
}


#endif //LEXICAL_ANALYZER_TOKENS_H
