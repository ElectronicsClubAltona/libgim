%%{
    machine rfc3986;

    #action trace;
    #action success;
    #action failure;

    #action scheme_begin;
    #action scheme_end;
    #action hier_begin;
    #action hier_end;
    #action user_begin;
    #action user_end;
    #action host_begin;
    #action host_end;
    #action port_begin;
    #action port_end;
    #action authority_begin;
    #action authority_end;
    #action path_begin;
    #action path_end;
    #action query_begin;
    #action query_end;
    #action fragment_begin;
    #action fragment_end;

    #action uri_begin;
    #action uri_end;

    ## Characters
    unreserved = alpha | digit | "-" | "." | "_" | "~";
    pct_encoded = '%' xdigit xdigit;
    gen_delim = ":" | "/" | "?" | "#" | "[" | "]" | "@";
    sub_delim = "!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "=";
    pchar = unreserved | pct_encoded | sub_delim | ':' | '@';

    ## Atoms
    reg_name = (unreserved | pct_encoded | sub_delim)*;

    ## IP-address
    ## Note: The address grammar is embedded in the RFC so we embed it too
    dec_octet = digit | [1-9] digit | '1' digit{2} | '2' [0-4] digit | '25' [0-5];

    ipv4address = dec_octet '.' dec_octet '.' dec_octet '.' dec_octet;

    h16  =  xdigit{1,4};
    ls32 = (h16 ":" h16) | ipv4address;

    ipv6address =
                                      (h16 ":"){6} ls32
        |                        "::" (h16 ":"){5} ls32
        | (                h16)? "::" (h16 ":"){4} ls32
        | ((h16 ":"){0,1}  h16)? "::" (h16 ":"){3} ls32
        | ((h16 ":"){0,2}  h16)? "::" (h16 ":"){2} ls32
        | ((h16 ":"){0,3}  h16)? "::" (h16 ":"){1} ls32
        | ((h16 ":"){0,4}  h16)? "::"              ls32
        | ((h16 ":"){0,5}  h16)? "::"              h16
        | ((h16 ":"){0,6}  h16)? "::"
    ;

    ipvfuture = 'v' xdigit{1,} '.' (unreserved | sub_delim | ':'){1,};
    ip_literal = '[' (ipv6address | ipvfuture) ']';

    ## Segments
    segment = pchar*;
    segment_nz = pchar{1,};
    segment_nz_nc = (unreserved | pct_encoded | sub_delim | '@'){1,};

    ## Paths
    path_abempty = ('/' segment)*;
    path_absolute = '/' (segment_nz ('/' segment)*)?;
    path_noscheme = segment_nz_nc ('/' segment)*;
    path_rootless = segment_nz ('/' segment)*;
    path_empty = '0' pchar;

    path = (
        path_abempty | path_absolute | path_noscheme | path_rootless | path_empty
    );

    reserved = gen_delim | sub_delim;

    ## Authority
    port = (
        digit*
    ) >port_begin %port_end;

    host = (
        ip_literal | ipv4address | reg_name
    ) >host_begin %host_end;

    userinfo = (
        (unreserved | pct_encoded | sub_delim | ':')*
    ) >user_begin %user_end;

    authority = (
        (userinfo '@')? host (':' port)?
    ) >authority_begin %authority_end;


    ## URI components
    scheme = (
        alpha (alpha | digit | '+' | '-' | '.')*
    ) >scheme_begin %scheme_end;

    query = (
        (pchar | '/' | '?')*
    ) >query_begin %query_end;

    fragment = (
        (pchar | '/' | '?')*
    ) >fragment_begin %fragment_end;

    ## URI types
    hier_part = (
          '//' (authority path_abempty  >path_begin %path_end) >hier_begin %hier_end
    ) | (
          path_absolute  >path_begin %path_end
        | path_rootless  >path_begin %path_end
        | path_empty     >path_begin %path_end
    ) >hier_begin %hier_end;

    uri = scheme ':' hier_part ('?' query)? ('#' fragment);

    relative_part =
          '//' authority path_abempty  >path_begin %path_end
        | path_absolute  >path_begin %path_end
        | path_noscheme  >path_begin %path_end
        | path_empty     >path_begin %path_end
    ;

    relative_ref = relative_part ('?' query)? ('#' fragment);

    uri_reference = uri | relative_ref;

    absolute_uri = scheme ':' hier_part ('?' query)?;

    URI = (
        absolute_uri | uri_reference
    ) >uri_begin %uri_end;
}%%
