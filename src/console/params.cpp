#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/variant.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;

typedef boost::variant<
    std::string,
    float
    > Param;

struct Cmd {
    std::string name;
    std::vector<Param> params;
};

BOOST_FUSION_ADAPT_STRUCT(
    Cmd,
    (std::string, name)
    (std::vector<Param>, params)
    );

template <typename Iterator, typename Skipper = ascii::space_type>
struct cmd_grammar : qi::grammar<Iterator, Cmd(), Skipper>
{
    cmd_grammar() : cmd_grammar::base_type(cmd_)
    {
        using qi::float_;
        using qi::lexeme;
        using qi::alpha;
        using qi::alnum;
        using ascii::char_;

        identifier_ %= lexeme[alpha >> *(alnum | char_('_') | char_('-'))];

        quoted_string_ %= lexeme['"' >> +(char_ - '"') >> '"'];

        param_ %= identifier_ | quoted_string_ | float_;

        cmd_ %= identifier_ >> *param_;
    }

    qi::rule<Iterator, std::string(), Skipper> identifier_, quoted_string_;
    qi::rule<Iterator, Param(), Skipper> param_;
    qi::rule<Iterator, Cmd(), Skipper> cmd_;
};

int main(int argc, char *argv[])
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tAn employee parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef cmd_grammar<iterator_type> cmd_grammar;

    cmd_grammar g; // Our grammar
    std::string str;
    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        Cmd cmd;
        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        bool r = phrase_parse(iter, end, g, space, cmd);

        if (r && iter == end)
        {
            std::cout << boost::fusion::tuple_open('[');
            std::cout << boost::fusion::tuple_close(']');
            std::cout << boost::fusion::tuple_delimiter(", ");

            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";

            std::cout << cmd.name << std::endl;
            for (Param p : cmd.params)
                std::cout << p << std::endl;

            std::cout << "\n-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
