/**
 * ENTER DESCRIPTION HERE
 * @date 8/28/2022
 * @author Javier "Lopea" Sandoval (https://lopea.dev)
 * 
 */

#include "scanner.h"
#include "Log.h"

/**********************************************************************************************************************/
void Scanner::String()
{
    //for better debugging
    int startString = m_line;
    while (m_data[m_current] != '"' && !EoF())
    {
        if(m_data[m_current] == '\n') m_line++;
        Advance();
    }
    if(EoF())
    {
        Log::Error(startString, "Unterminated String");
        return;
    }

    Advance();
    AddToken(TokenType::STRING,
             std::string(&m_data[m_start + 1], m_current - m_start -2));
}

/**********************************************************************************************************************/
void Scanner::Number()
{
    while(!EoF() && isdigit(m_data[m_current])) Advance();

    //TODO: add float support with the f flag
    if(!EoF() && m_data[m_current] == '.' && isdigit(m_data[m_current + 1]))
    {
        Advance();
        while(isdigit(m_data[m_current])) Advance();
        AddToken(TokenType::NUMBER, std::stod(GetString()));
    }
    else
    {
        AddToken(TokenType::NUMBER, std::stoi(GetString()));
    }
}

/**********************************************************************************************************************/
void Scanner::Identifier()
{
    while(!EoF() && isalnum(m_data[m_current])) Advance();
    TokenType type = TokenType::IDENTIFIER;
    auto reservedIt = m_reservedWords.find(GetString());

    if(reservedIt != m_reservedWords.end())
        type = reservedIt->second;

    AddToken(type);
}

/**********************************************************************************************************************/
bool Scanner::EoF() const
{
    return m_data.size() <= m_current;
}

/**********************************************************************************************************************/
std::string Scanner::GetString() const
{
    return {&m_data[m_start], (unsigned)m_current - m_start};
}

/**********************************************************************************************************************/
void Scanner::ScanToken()
{
    char curr = Advance();
    switch (curr)
    {
        case '(' : AddToken(TokenType::LPAREN); break;
        case ')' : AddToken(TokenType::RPAREN); break;
        case '{' : AddToken(TokenType::LBRACE); break;
        case '}' : AddToken(TokenType::RBRACE); break;
        case ',' : AddToken(TokenType::COMMA); break;
        case '.' : AddToken(TokenType::DOT); break;
        case '+' :
            AddToken(Match('+') ? TokenType::PLUS2 : TokenType::PLUS); break;
        case '-' :
            AddToken(Match('-') ? TokenType::MINUS2 : TokenType::MINUS); break;
        case ';' : AddToken(TokenType::SEMICOLON); break;
        case '*' : AddToken(TokenType::STAR); break;
        case '!' :
            AddToken(Match('=') ? TokenType::EXC_EQUAL: TokenType::EXC);
            break;
        case '=':
            AddToken(Match('=') ? TokenType::EQUAL_EQUAL: TokenType::EQUAL);
            break;
        case '<' :
            AddToken(Match('=') ? TokenType::LESS_EQUAL: TokenType::LESS);
            break;
        case '>' :
            AddToken(Match('=') ? TokenType::GREATER_EQUAL: TokenType::GREATER);
            break;
        case '/':
            //comments like this one!
            if(Match('/'))
                while(!EoF() && m_data[m_current] != '\n') Advance();
            else if(Match('*'))
            {
                //move until it finds the end of the multi line comment
                while((m_current != m_data.size() - 2)
                      && m_data[m_current] != '*' && m_data[m_current + 1] != '/')
                {
                    if(m_data[m_current] == '\n') m_line++;
                    Advance();
                }
                //skip both identifiers if necessary
                Advance();
                Advance();
            }
            else
                AddToken(TokenType::SLASH);
        case ' ':
            [[fallthrough]];
        case '\t':
            [[fallthrough]];
        case '\r':
            //ignore whitespace
            break;

            //new line
        case '\n':
            ++m_line;
            break;
            //string
        case '"':
            String();
            break;
        default:
            if(isdigit(curr)) Number();
            else if(isalpha(curr)) Identifier();
            else Log::Error(m_line, "Unexpected character");
            break;
    }
}

/**********************************************************************************************************************/
void Scanner::AddToken(TokenType type)
{
    AddToken(type, nullptr);
}

/**********************************************************************************************************************/
void Scanner::AddEOF()
{
    std::nullptr_t s = nullptr;
    m_tokens.push_back(std::make_shared<TemplateToken<std::nullptr_t>>(TokenType::ENDOFFILE,
            "", s, m_line));
}

/**********************************************************************************************************************/
bool Scanner::Match(char m)
{
    if(EoF()) return false;
    if(m != m_data[m_current]) return false;

    ++m_current;
    return true;
}

/**********************************************************************************************************************/
char Scanner::Advance()
{
    return m_data[m_current++];
}

/**********************************************************************************************************************/
void Scanner::ScanTokens()
{
    while(m_data.size() > m_current)
    {
        m_start = m_current;
        ScanToken();
    }
    AddEOF();
}
