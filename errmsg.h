#ifndef ERRMSG_H
#define ERRMSG_H

class Char;
class String;

void error_unterminated_comment(const Char &);
void error_unexpected_end_of_file(const Char &ch);
void error_unknown_command(const String &command);
void error_duplicate_macro(const String &before, const String &after);
void error_undefined_macro(const String &macro);
void error_unexpected_character(const Char &ch);

#endif
