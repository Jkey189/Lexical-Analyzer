#include <utility>

#include "includes/includes.h"


enum class TokenType {
  INTEGER_LITERAL,
  FLOAT_LITERAL,
  NUMBER_LITERAL,
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

enum class State {
  START,
  IN_IDENTIFIER,
  IN_NUMBER,
  IN_OPERATOR,
  ERROR,
  DONE
};

struct Token {
  TokenType type;
  std::string value;

  Token(TokenType t, std::string  v) :
      type(t), value(std::move(v)) {}
};


class LexicalAnalyser {
public:
  explicit LexicalAnalyser(std::string sourceCode) : sourceCode_(std::move(sourceCode)), position_(0), state_(State::START) {}

  std::vector<Token> tokenize() {
    std::vector<Token> tokens;

    while (position_ < sourceCode_.length()) {
      char currChar = peek();
      State nextState = transition(state_, currChar);

      if (4isEnter(currChar)) {
        get();
        continue;
      }

      if (nextState == State::DONE || nextState == State::ERROR) {
        if (state_ == State::IN_IDENTIFIER) {
          TokenType keywordType = keywords_.find(currentToken_) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
          tokens.emplace_back(keywordType, currentToken_);
        } else if (state_ == State::IN_NUMBER) {
          tokens.emplace_back(TokenType::NUMBER_LITERAL, currentToken_);
        } else if (state_ == State::IN_OPERATOR) {
          tokens.emplace_back(TokenType::OPERATOR, currentToken_);
        }

        reset();

        if (nextState == State::ERROR) { get(); } // skip unknown symbol
      } else {
        currentToken_ += get();
        state_ = nextState;
      }
    }

    return tokens;
  }

  static bool isWhitespace(const char ch) {
    return ch == ' ';
  }

  static bool isEnter(const char ch) {
    return ch == '\n';
  }


private:
  std::string sourceCode_;
  size_t position_;
  KeywordsTree keywords_;
  std::string currentToken_;
  State state_;

  char peek() { return position_ < sourceCode_.size() ? sourceCode_[position_] : '\0'; }
  char get() { return position_ < sourceCode_.size() ? sourceCode_[position_++] : '\0'; }
  void reset() {
    currentToken_.clear();
    state_ = State::START;
  }

  static State transition(State currentState, char input) {
    switch (currentState) {
      case State::START:
        if (isspace(input)) return State::START;
        if (isalpha(input) || input == '_') return State::IN_IDENTIFIER;
        if (isdigit(input)) return State::IN_NUMBER;
        if (ispunct(input)) return State::IN_OPERATOR;
        return State::ERROR;
      case State::IN_IDENTIFIER :
        if (isalnum(input) || input == '_') return State::IN_IDENTIFIER;
        return State::DONE;
      case State::IN_NUMBER:
        if (isdigit(input)) return State::IN_NUMBER;
        return State::DONE;
      case State::IN_OPERATOR:
        return State::DONE;
      default:
        return State::ERROR;
    }
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
    /*std::cout << "Token position: line: " << currToken.position.first << '\n';
    std::cout << "Token position: column: " << currToken.position.second << "\n\n";*/
  }
}


int main() {
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

  LexicalAnalyser lexer(sourceCode);

  std::vector<Token> tokens = lexer.tokenize();

  std::cout << "Source code: " << '\n' << sourceCode << "\n\n\n";

  std::cout << "Tokens in this source code: " << "\n\n";
  printTokens(tokens);
  std::cout << std::endl;

  return 0;
}
