/**
 * ENTER DESCRIPTION HERE
 * @date 8/28/2022
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#ifndef MYLOX_SCANNER_H
#define MYLOX_SCANNER_H

#include <vector>
#include <string>
#include <cstddef>
#include <unordered_map>
#include <iostream>
#include <any>
#include <memory>


enum class TokenType
{
    //(
    LPAREN,
    //)
    RPAREN,
    //{
    LBRACE,
    //}
    RBRACE,
    //,
    COMMA,
    //.
    DOT,
    //-
    MINUS,
    //--
    MINUS2,
    //+
    PLUS,
    //++,
    PLUS2,
    //;
    SEMICOLON,
    // /
    SLASH,
    //*
    STAR,
    // !
    EXC,
    // !=
    EXC_EQUAL,
    // =
    EQUAL,
    // ==
    EQUAL_EQUAL,
    // <
    LESS,
    // <=
    LESS_EQUAL,
    // >
    GREATER,
    // >=
    GREATER_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NUL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, VIRTUAL, OVERRIDE,

    ENDOFFILE
};


struct Token
{
    TokenType m_type;
    std::string m_lexeme;
    int m_line;

    virtual std::any GetLiteral() = 0;

    Token(TokenType type, std::string lexeme, int line) :
            m_type(type), m_lexeme(std::move(lexeme)), m_line(line)
    {}

    virtual ~Token() = default;

};

template<typename T>
struct TemplateToken : public Token
{
    T m_literal;

    TemplateToken(TokenType type, std::string lexeme, T &literal, int line)
            : Token(type, lexeme, line), m_literal(literal)
    {}

    std::any GetLiteral() override
    { return m_literal; }
};


using TokenPtr = std::shared_ptr<Token>;

class Scanner
{
public:
    explicit Scanner(std::vector<char> &&data) : m_data(data)
    {}

    void ScanTokens();

    [[nodiscard]] std::vector<TokenPtr> Get() const { return m_tokens; }
private:
    /** Movement Functions**/
    char Advance();

    bool Match(char m);

    /** Token Specific **/
    void String();

    void Number();

    void Identifier();

    /** Helper Functions **/
    [[nodiscard]] bool EoF() const;

    [[nodiscard]] std::string GetString() const;

    void ScanToken();

    /** Adding tokens **/
    template<typename LiteralType>
    void AddToken(TokenType type, LiteralType &&literal)
    {
        m_tokens.push_back(std::make_shared<TemplateToken<LiteralType>>(
                type,
                std::string(&m_data[m_start], m_current - m_start),
                literal,
                m_line
        ));
    }

    void AddToken(TokenType type);

    void AddEOF();

    /** variables **/

    unsigned long long m_start = 0, m_current = 0, m_line = 1;

    std::vector<char> m_data;
    std::vector<TokenPtr> m_tokens;

    std::unordered_map<std::string, TokenType> m_reservedWords
            {
                    {"and",      TokenType::AND},
                    {"class",    TokenType::CLASS},
                    {"else",     TokenType::ELSE},
                    {"false",    TokenType::FALSE},
                    {"fn",       TokenType::FUN},
                    {"for",      TokenType::FOR},
                    {"if",       TokenType::IF},
                    {"null",     TokenType::NUL},
                    {"or",       TokenType::OR},
                    {"print",    TokenType::PRINT},
                    {"return",   TokenType::RETURN},
                    {"super",    TokenType::SUPER},
                    {"this",     TokenType::THIS},
                    {"true",     TokenType::TRUE},
                    {"var",      TokenType::VAR},
                    {"while",    TokenType::WHILE},
                    {"virtual",  TokenType::VIRTUAL},
                    {"override", TokenType::OVERRIDE},
            };
};


#endif //MYLOX_SCANNER_H
