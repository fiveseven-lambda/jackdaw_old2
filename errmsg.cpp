#include <iostream>
#include "errmsg.h"
#include "io.h"
#include "load.h"

void error_unterminated_comment(const Char &ch){
	std::cerr << ch.pos << " error unterminated comment" << std::endl;
}
void error_unexpected_end_of_file(const Char &ch){
	std::cerr << ch.pos << " unexpected end of file" << std::endl;
}
void error_unknown_command(const String &command){
	std::cerr << command.pos() << " unknown command \"" << command << "\"" << std::endl;
}

void error_duplicate_macro(const String &before, const String &after){
	std::cerr << after.pos() << " macro \"" << after << "\" already defined at " << before.pos() << std::endl;
}
void error_undefined_macro(const String &macro){
	std::cerr << macro.pos() << " undefined macro \"" << macro << "\"" << std::endl;
}
void error_unexpected_character(const Char &ch){
	std::cerr << ch.pos << " unexpected character \"" << ch << "\"" << std::endl;
}
