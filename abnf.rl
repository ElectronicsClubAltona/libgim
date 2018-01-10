void foo (void) { }

%%{

machine rfc5234;

###############################################################################
# RFC5234 ABNF core rules

# ; A-Z / a-z
ALPHA =  0x41..0x5A | 0x61..0x7A;
BIT   =  '0' | '1';

# any 7-bit US-ASCII character, excluding NUL
CHAR  = 0x01..0x7F;

# carriage return
CR = 0x0D;

# linefeed
LF = 0x0A;

# Internet standard newline
CRLF = CR LF;

# controls
CTL = 0x00..0x1F | 0x7F;

# 0-9
DIGIT = 0x30..0x39;

# " (Double Quote)
DQUOTE = 0x22;

HEXDIG = DIGIT | 'A'..'F';

# horizontal tab
HTAB = 0x09;

SP = 0x20;

# white space
WSP =  SP | HTAB;

# Use of this linear-white-space rule permits lines containing only white space
# that are no longer legal in mail headers and have caused interoperability
# problems in other contexts.
#
# Do not use when defining mail headers and use with caution in other contexts.
LWSP = (WSP | CRLF WSP)*;

# 8 bits of data
OCTET = any; #0x00..0xFF;

# visible (printing) characters
VCHAR = 0x21..0x7E;

    write data;

}%%

int main () {


    %%write init;
    %%write exec;
}
