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
  bool isHas(std::string&);
  void insert(std::string&);
  std::string findK(int64_t);


private:
  int64_t cnt = 0;
  Word* root = new Word();
};

bool KeywordsTree::isHas(std::string& str) {
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

std::string KeywordsTree::findK(int64_t k) {
  std::string str;
  Word* v = root;

  while (k > 0) {
    if (v->isTerm) {
      --k;
    }
    if (!k) { break; }

    for (const auto& it : v->to) {
      char ch = it.first;
      Word* u = it.second;

      if (k > u->termCnt) {
        k -= u->termCnt;
      } else {
        str += ch;
        v = v->to[ch];

        break;
      }
    }
  }

  return str;
}


#endif //LEXICAL_ANALYZER_TOKENS_H
