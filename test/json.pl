#!/usr/bin/perl

$COMMAND = "./json-check";

@good   = <json/good/*>;
@bad    = <json/bad/*>;
$success = 1;


sub status_to_str {
	$status = shift @_;
	return $status ? "failed" : "passed";
}


foreach $testfile(@good) {
	$status    = system("$COMMAND $testfile &>/dev/null");
	$success &&= $status == 0;

	printf "%s\t%s\n", status_to_str($status), $testfile;
}


foreach $testfile(@bad) {
	$status    = system("$COMMAND $testfile &>/dev/null");
	$success &&= $status != 0;

	printf "%s\t%s\n", status_to_str(!$status), $testfile;
}

exit !$success
