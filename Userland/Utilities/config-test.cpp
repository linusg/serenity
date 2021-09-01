#include <AK/Format.h>
#include <AK/StringView.h>
#include <LibConfig/Language/Lexer.h>
#include <LibConfig/Language/Parser.h>

int main(int, char**)
{
    StringView source =
        R"~~~(
[startup]
command : path = "/bin/Shell"

[window]
# This is a comment
# This is another comment
opacity : i32 (0..255) = 255
bell : enum (Visible, Audible, None) = Visible
color_scheme : string = "Default"
#reached the end
)~~~";

    auto lexer = Config::Lexer { source };
    auto parser = Config::Parser { lexer };
    auto ast_or_error = parser.parse();
    if (ast_or_error.is_error()) {
        auto error = ast_or_error.release_error();
        auto [line, column, _] = error.token.start_position();
        dbgln("ERROR: {} @ {}:{}", error.message, line, column);
    }
    auto ast = ast_or_error.release_value();
    ast->dump();
}
