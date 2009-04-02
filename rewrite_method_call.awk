#!/usr/bin/awk -f
function ltrim(s) {
	sub(/^[ \t]+/, "", s);
	return s;
}
function rtrim(s) {
	sub(/[ \t]+$/, "", s);
	return s;
}
function trim(s) {
	return rtrim(ltrim(s));
}
function array_length(a) {
	n = 0;
	for (i in a) n++;
	return n;
}
function rewrite_method_call(line, macro, splitter, method_prefix) {
	lvalue = 0;
	rvalue = 0;
	# Determine lvalue and rvalue
	split(line, value, "=");
	value_length = array_length(value);
	if (value_length == 1) {
		rvalue = value[1];
	} else if (value_length == 2) {
		lvalue = value[1];
		rvalue = value[2];
	}

	# Determine object, method and parameters
	split(rvalue, object_list, splitter);
	object = object_list[1];
	method = substr(object_list[2], 1, index(object_list[2], "(") - 1);

	parameters = substr(object_list[2], index(object_list[2], ",")+1);

	result = sprintf("%s, %s%s(%s", trim(object), method_prefix, trim(method), parameters);
	result = sprintf("%s(%s);", macro, result);
	if (lvalue) {
		result = sprintf("%s= %s", lvalue, result);
	} else {
		offset = substr(line, 1, index(line, trim(object))-1);
		result = sprintf("%s%s", offset, result);
	}
	return result;
}
/.*->class->[a-zA-Z_]*\(.*\)/ {
	print rewrite_method_call($0, "O_CALL", "->class->", "");
}
/.*\(\)->new[a-zA-Z_]*\(.*\)/ {
	print rewrite_method_call($0, "O_CALL_CLASS", "->new", "new");
}
! (/.*->class->[a-zA-Z_]*\(.*\)/||/.*\(\)->new[a-zA-Z_]*\(.*\)/) {
	print $0;
}