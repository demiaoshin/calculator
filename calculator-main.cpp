#include <iostream>
#include <utility>
#include <optional>
#include <string>
#include <cstdlib>

class Tokeniser
{
public:

    enum class Type
    {
        add,
        subtract,
        multiply,
        divide,
        
        unknown,
    };
    
    struct TokenList
    {
        double lhs;
        double rhs;
        Type type;
    };

    // all the operators stored in an array of strings
    std::string symbols[4] = {"+", "-", "*", "/"};

    [[nodiscard]] std::optional <TokenList> tokenise (const std::string& input) const
    {

        auto type = findType (input);
        if (type == Type::unknown)
            return {};
            
        auto lhs = findLHS (input);
        if (! lhs.has_value ())
            return {};
            
        auto rhs = findRHS (input);
        if (! rhs.has_value ())
            return {};
    
        TokenList tokens{};
        tokens.lhs = *lhs;
        tokens.rhs = *rhs;
        tokens.type = type;
        return tokens;
    }
    
private:

    [[nodiscard]] static bool find (const std::string& input, const std::string& character)
    {
        return input.find (character) != std::string::npos;
    }
    
    [[nodiscard]] static std::optional <double> findAndExtractLHS (const std::string& input, const std::string& character)
    {
        if (auto pos = input.find (character); pos != std::string::npos) {
            const std::string res = input.substr (0, pos-1);
            if (res == "pi")
                return 3.1415;
            return std::stod (res);
        }

        return {};
    }
    
    [[nodiscard]] static std::optional <double> findAndExtractRHS (const std::string& input, const std::string& character)
    {
        if (auto pos = input.find (character); pos != std::string::npos) {
            const std::string res = input.substr (pos + 2);
            if (res == "pi")
                return 3.1415;
            return std::stod (res);
        }
            
        return {};
    }

    [[nodiscard]] static Type findType (const std::string& input)
    {
        if (find (input, "+")) return Type::add;
        if (find (input, "-")) return Type::subtract;
        if (find (input, "*")) return Type::multiply;
        if (find (input, "/")) return Type::divide;
        
        return Type::unknown;
    }
    
    [[nodiscard]] std::optional <double> findLHS (const std::string& input) const
    {
        for (const std::string& symbol : symbols) {
            if (auto result = findAndExtractLHS (input, symbol))
                return result;
        }

        return {};
    }
    
    [[nodiscard]] std::optional <double> findRHS (const std::string& input) const
    {
        for (const std::string& symbol : symbols) {
            if (auto result = findAndExtractRHS (input, symbol))
                return result;
        }
            
        return {};
    }
};

class Calculator
{
public:

    Calculator () = default;
    ~Calculator () = default;
    
    [[nodiscard]] static double calculate (Tokeniser::TokenList tokens)
    {
        switch (tokens.type)
        {
            case Tokeniser::Type::add:
                return tokens.lhs + tokens.rhs;
            case Tokeniser::Type::subtract:
                return tokens.lhs - tokens.rhs;
            case Tokeniser::Type::multiply:
                return tokens.lhs * tokens.rhs;
            case Tokeniser::Type::divide:
                return tokens.lhs / tokens.rhs;
            default:
                break;
        }
        
        return 0;
    }
};

class InputProcessor
{
public:
    
    enum class InputRequest
    {
        quitProcessing,
        continueProcessing,
    };
    

    [[nodiscard]] static InputRequest requestInput ()
    {
        std::cout << "Please enter a calculation (Type Q to quit)" << std::endl;
        
        std::string input;
        std::getline (std::cin, input);
        
        if (input == "Q" || input == "q")
            return InputRequest::quitProcessing;
            
        processInput (input);
    
        return InputRequest::continueProcessing;
    }

private:

    static void processInput (const std::string& input)
    {
        if (auto tokens = Tokeniser ().tokenise (input))
            std::cout << "Answer: " << Calculator::calculate (*tokens) << std::endl;
        else
            std::cout << "There was an error in the input string, please try again..." << std::endl;
    }
};

class ResultChecker
{
public:

    static void check (double value, double expected, double range = 1e-3)
    {
        return assert (std::abs (value - expected) <= range);
    }
};

void test ()
{
    auto result = Tokeniser ().tokenise ("6*9");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 6);
    ResultChecker::check (result->rhs, 9);
    assert (result->type == Tokeniser::Type::multiply);
    
    result = Tokeniser ().tokenise ("6 * 9");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 6);
    ResultChecker::check (result->rhs, 9);
    assert (result->type == Tokeniser::Type::multiply);
    
    result = Tokeniser ().tokenise ("25 * 4");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 25);
    ResultChecker::check (result->rhs, 4);
    assert (result->type == Tokeniser::Type::multiply);

    ResultChecker::check (Calculator::calculate ({ 10, 4, Tokeniser::Type::multiply }), 40);
    ResultChecker::check (Calculator::calculate ({ 25.3, 18.6, Tokeniser::Type::add }), 43.9);
    ResultChecker::check (Calculator::calculate ({ 3, 5.6, Tokeniser::Type::subtract }), 2.6);
    ResultChecker::check (Calculator::calculate ({3.1415, 5, Tokeniser::Type::multiply }), 2.6);
}

void run ()
{
    while (InputProcessor::requestInput () == InputProcessor::InputRequest::continueProcessing);
}

int main (int argc, const char * argv[])
{
    if (argc > 1 && std::string (argv[1]) == "--test")
        test ();
    else
        run ();
    
    return 0;
}
