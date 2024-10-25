#include "includes/includes.h"


enum class TokenType {
  INTEGER_LITERAL,
  FLOAT_LITERAL,
  STRING_LITERAL,
  LOGICAL_LITERAL,
  INTEGER_TYPE,
  FLOAT_TYPE,
  STRING_TYPE,
  LOGICAL_TYPE,
  KEYWORD,
  IDENTIFIER,
  OPERATOR,
  PUNCTUATOR,
  UNKNOWN
};

struct Token {
  TokenType type;
  std::string value;
  std::pair<int, int> position; // line ans column

  Token(TokenType t, std::string  v, int line, int column) :
  type(t), value(std::move(v)), position({line, column}) {}
};


class LexicalAnalyser {
public:
  explicit LexicalAnalyser(std::string source) : input_(std::move(source)), position_(0) {
    initializeKeywords();
  }

  std::vector<Token> tokenize() { // add LOGICAL and STRINGS
    std::vector<Token> tokens;
    int currLine = 1;
    int currColumn = 0;

    std::pair<char, bool> withNum;

    while (position_ < input_.length()) {
      char currChar = input_[position_];
      ++currColumn;
      if (currChar == '\n') {
        ++currLine;
        currColumn = 0;
      }

      if (isSpace(currChar)) {
        withNum.second = false;
        ++position_;
        ++currColumn;
        continue;
      }

      if (isEnter(currChar)) {
        ++position_;
        ++currLine;
        currColumn = 0;
        continue;
      }

      if (isAlpha(currChar)) {
        std::string word = getWord();

        if (keywords_.find(word)) {
          tokens.emplace_back(TokenType::KEYWORD, word, currLine, currColumn);
        } else if (word == "int") {
          tokens.emplace_back(TokenType::INTEGER_TYPE, word, currLine, currColumn);
        } else if (word == "float") {
          tokens.emplace_back(TokenType::FLOAT_TYPE, word, currLine, currColumn);
        } else if (word == "bool") {
          tokens.emplace_back(TokenType::LOGICAL_TYPE, word, currLine, currColumn);
        } else if (word == "string") {
          tokens.emplace_back(TokenType::STRING_TYPE, word, currLine, currColumn);
        } else {
          tokens.emplace_back(TokenType::IDENTIFIER, word, currLine, currColumn);
        }
      } else if (isDigit(currChar)) {
        std::string number = getNumber();

        if (withNum.second) {
          std::string cntNumber;
          cntNumber += withNum.first;
          cntNumber += number;
          number = cntNumber;
        }

        if (number.find('.') != std::string::npos) {
          tokens.emplace_back(TokenType::FLOAT_LITERAL, number, currLine, currColumn);
        } else {
          tokens.emplace_back(TokenType::INTEGER_LITERAL, number, currLine, currColumn);
        }

        withNum.second = false;
      } else if (currChar == '+' ||
                 currChar == '-' ||
                 currChar == '*' ||
                 currChar == '/') { // + add <, >, <=, >=, ==, !=
        if (currChar == '+' || currChar == '-' && !withNum.second) {
          withNum = {currChar, true};
          ++position_;
          ++currColumn;
        } else {
          tokens.emplace_back(TokenType::OPERATOR, std::string(1, currChar), currLine, currColumn);
          ++position_;
          ++currColumn;
        }
      } else if (currChar == '(' ||
                 currChar == ')' ||
                 currChar == '{' ||
                 currChar == '}' ||
                 currChar == ';') {
        tokens.emplace_back(TokenType::PUNCTUATOR, std::string(1, currChar), currLine, currColumn);
        ++position_;
        ++currColumn;
      } else {
        tokens.emplace_back(TokenType::UNKNOWN, std::string(1, currChar), currLine, currColumn);
        ++position_;
        ++currColumn;
      }
    }

    return tokens;
  }


private:
  std::string input_;
  size_t position_;
  KeywordsTree keywords_;
  /*std::unordered_map<std::string, TokenType> OLDkeywords_;*/

  void initializeKeywords() {
    /*OLDkeywords_["if"] = TokenType::KEYWORD;
    OLDkeywords_["else"] = TokenType::KEYWORD;
    OLDkeywords_["case"] = TokenType::KEYWORD;
    OLDkeywords_["switch"] = TokenType::KEYWORD;
    OLDkeywords_["break"] = TokenType::KEYWORD;
    OLDkeywords_["continue"] = TokenType::KEYWORD;
    OLDkeywords_["const"] = TokenType::KEYWORD;
    OLDkeywords_["while"] = TokenType::KEYWORD;
    OLDkeywords_["for"] = TokenType::KEYWORD;
    OLDkeywords_["return"] = TokenType::KEYWORD;
    OLDkeywords_["void"] = TokenType::KEYWORD;
    OLDkeywords_["true"] = TokenType::KEYWORD;
    OLDkeywords_["false"] = TokenType::KEYWORD;*/

    std::string ifStr = "if";
    std::string elseStr = "else";
    std::string caseStr = "case";
    std::string switchStr = "switch";
    std::string breakStr = "break";
    std::string continueStr = "continue";
    std::string constStr = "const";
    std::string whileStr = "while";
    std::string forStr = "for";
    std::string returnStr = "return";
    std::string voidStr = "void";
    std::string trueStr = "true";
    std::string falseStr = "false";

    keywords_.insert(ifStr);
    keywords_.insert(elseStr);
    keywords_.insert(caseStr);
    keywords_.insert(switchStr);
    keywords_.insert(breakStr);
    keywords_.insert(continueStr);
    keywords_.insert(constStr);
    keywords_.insert(whileStr);
    keywords_.insert(forStr);
    keywords_.insert(returnStr);
    keywords_.insert(voidStr);
    keywords_.insert(trueStr);
    keywords_.insert(falseStr );
  }

  static bool isSpace(const char c) {
    return c == ' ';
  }

  static bool isEnter(const char c) {
    return c == '\n';
  }

  static bool isAlpha(const char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
  }

  static bool isDigit(const char c) {
    return c >= '0' && c <= '9';
  }

  static bool isAlphaNumeric(const char c) {
    return isAlpha(c) || isDigit(c);
  }

  std::string getWord() {
    size_t start = position_;
    while (position_ < input_.length() && isAlphaNumeric(input_[position_])) {
      ++position_;
    }

    return input_.substr(start, position_ - start);
  }

  std::string getNumber() {
    size_t start = position_;
    bool hasDecimal = false;

    while (position_ < input_.length() && isDigit(input_[position_]) || input_[position_] == '.') {
      if (input_[position_] == '.') {
        if (hasDecimal) { break; }
        hasDecimal = true;
      }
      ++position_;
    }

    return input_.substr(start, position_ - start);
  }
};


std::string getTokenTypeName(TokenType tokenType) {
  switch (tokenType) {
    case TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case TokenType::LOGICAL_LITERAL:
      return "LOGICAL_LITERAL";
    case TokenType::INTEGER_TYPE:
      return "INTEGER_TYPE";
    case TokenType::FLOAT_TYPE:
      return "FLOAT_TYPE";
    case TokenType::STRING_TYPE:
      return "STRING_TYPE";
    case TokenType::LOGICAL_TYPE:
      return "LOGICAL_TYPE";
    case TokenType::KEYWORD:
      return "KEYWORD";
    case TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case TokenType::OPERATOR:
      return "OPERATOR";
    case TokenType::PUNCTUATOR:
      return "PUNCTUATOR";
    default: // TokenType::UNKNOWN
      return "UNKNOWN";
  }
}

void printTokens(const std::vector<Token>& tokens) {
  for (auto& currToken : tokens) {
    std::cout << "Token value: " << currToken.value << '\n';
    std::cout << "Token type: " << getTokenTypeName(currToken.type) << '\n';
    std::cout << "Token position: line: " << currToken.position.first << '\n';
    std::cout << "Token position: column: " << currToken.position.second << "\n\n";
  }
}


int main() {
  /*std::string sourceCode = "#include <iostream>\n\n"
                           "int main() {\n"
                           "\tstd::cout << 12345;\n"
                           "\treturn 0;\n"
                           "}";
  LexicalAnalyser lexer(sourceCode);

  std::vector<Token> tokens = lexer.tokenize();

  std::cout << "Source code: " << '\n' << sourceCode << "\n\n\n";

  std::cout << "Tokens in this source code: " << "\n\n";
  printTokens(tokens);
  std::cout << std::endl;*/

  std::string fileName = "C:/Work/Lexical-Analyzer/Lexical-Analyzer/source_file.txt";
  std::ifstream sourceFile(fileName);

  if (!sourceFile.is_open()) {
    std::cerr << "Failed to open file " << "\"" << fileName << "\"" << std::endl;

    if (sourceFile.bad()) {
      std::cerr << "Fatal error: bad-bit is set" << std::endl;
    }

    if (sourceFile.fail()) {
      std::cerr << "Error details: " << strerror(errno) << std::endl;
    }

    return 1;
  }

  sourceFile.seekg(0, std::ios::end);

  size_t lengthOfFile = sourceFile.tellg();
  std::string sourceCode( lengthOfFile, ' ');

  sourceFile.seekg(0/*, std::ios::end*/);

  sourceFile.read(&sourceCode[0], lengthOfFile);

//  std::cout << sourceCode << std::endl;

  LexicalAnalyser lexer(sourceCode);

  std::vector<Token> tokens = lexer.tokenize();

  std::cout << "Source code: " << '\n' << sourceCode << "\n\n\n";

  std::cout << "Tokens in this source code: " << "\n\n";
  printTokens(tokens);
  std::cout << std::endl;

  /*std::string fileName = "C:/Work/lexical_analyzer/source_file.txt";
  std::ifstream sourceFile(fileName);

  if (!sourceFile.is_open()) {
    // throw std::some_error;
    std::cerr << "Failed to read from file " << fileName << std::endl;
    return 1;
  }

  std::string sourceCode;
  while (std::getline(sourceFile, sourceCode)) {
    std::cout << sourceCode << std::endl;
  }

  sourceFile.close();*/

  return 0;
}
