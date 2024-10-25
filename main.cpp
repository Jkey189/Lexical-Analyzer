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

    while (position_ < input_.length()) {
      char currChar = input_[position_];
      ++currColumn;
      if (currChar == '\n') {
        ++currLine;
        currColumn = 0;
      }

      if (isSpace(currChar)) {
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

      if (isAlphaNumeric(currChar)) {
        std::string word = getWord();

        if (keywords_.find(word) != keywords_.end()) {
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

        if (number.find('.') != std::string::npos) {
          tokens.emplace_back(TokenType::FLOAT_LITERAL, number, currLine, currColumn);
        } else {
          tokens.emplace_back(TokenType::INTEGER_LITERAL, number, currLine, currColumn);
        }
      } else if (currChar == '+' ||
                 currChar == '-' ||
                 currChar == '*' ||
                 currChar == '/') { // + add <, >, <=, >=, ==, !=
        tokens.emplace_back(TokenType::OPERATOR, std::string(1, currChar), currLine, currColumn);
        ++position_;
        ++currColumn;
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
  std::unordered_map<std::string, TokenType> keywords_;

  void initializeKeywords() {
    keywords_["if"] = TokenType::KEYWORD;
    keywords_["else"] = TokenType::KEYWORD;
    keywords_["case"] = TokenType::KEYWORD;
    keywords_["switch"] = TokenType::KEYWORD;
    keywords_["break"] = TokenType::KEYWORD;
    keywords_["continue"] = TokenType::KEYWORD;
    keywords_["const"] = TokenType::KEYWORD;
    keywords_["while"] = TokenType::KEYWORD;
    keywords_["for"] = TokenType::KEYWORD;
    keywords_["return"] = TokenType::KEYWORD;
    keywords_["void"] = TokenType::KEYWORD;
    keywords_["true"] = TokenType::KEYWORD;
    keywords_["false"] = TokenType::KEYWORD;
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
    std::cerr << "Failed to open file " << fileName << std::endl;

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
