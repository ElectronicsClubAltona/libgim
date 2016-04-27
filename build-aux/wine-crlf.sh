#!/bin/sh

case "$1" in
*.exe)
    OUTPUT="$(/usr/bin/env wine $@)"
    STATUS=$?

    echo "$OUTPUT" | sed 's/\r$//'
    exit $STATUS
    ;;

*)
    $@
    exit $?
    ;;
esac
