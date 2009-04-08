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
function debug(string) {
	if (debug_level > 0) {
		print string;
	}
}
function rewrite_method(method, splitter, object, macro, method_prefix) {
	debug("rewrite_method(\""method"\", \""splitter"\", \""object"\")");

	split(method, list, splitter);	
	if (array_length(list) != 2) {
		debug("ERROR: Expected list of size 2!");
		return method;
	}

	split(list[1], prefix, object);
	if (array_length(prefix) != 1) {
		debug("WARNING: Expected prefix of size 1!");
		for (offset = 1; offset < array_length(prefix); offset ++) {
			debug("prefix["offset"] = "prefix[offset]);
		}
		# return method;
	}
	
	meth = substr(list[2],1,length(list[2])-1);
	return prefix[1] macro "(" object", " method_prefix meth;
}
function rewrite_method_call(method,parameters, pos, parameter_length, result) {
	debug("rewrite_method_call(\""method"\", \""parameters"\")");
	parameters = unnest(parameters);
	comma = index(parameters, ",");
	if (comma == 0) {
		comma = length(parameters) + 1;
	}
	object = substr(parameters, 1, comma - 1);
	parameters = substr(parameters, comma + 1);
	debug("parameters = "parameters);
	method = rewrite_method(method, "->class->", object, "O_CALL");
	debug("method = "method);
	if (trim(parameters) == "") {
		return method;
	} else {
		return method "," parameters;
	}
}
function rewrite_new_call(method, parameters) {
	debug("rewrite_new_call(\""method"\", \""parameters"\")");
	parameters = unnest(parameters);
	comma = index(parameters, ",");
	if (comma == 0) {
		comma = length(parameters);
	}
	object = substr(parameters, 1, comma - 1);
	parameters = substr(parameters, comma + 1);
	debug("parameters = "parameters);
	method = rewrite_method(method, "->new", object, "O_CALL_CLASS", "new");
	debug("method = "method);
	return method "," parameters;
}
function rewrite_call(method,parameters) {
	debug("rewrite_call(\""method"\", \""parameters"\")");
	if (index(method, "->new") > 0) {
		return rewrite_new_call(method, parameters);
	} else if (index(method, "->class->") > 0) {
		return rewrite_method_call(method, parameters);
	} else {
		return method unnest(parameters);
	}
}
function unnest(line, begin, middle, end) {
	debug("unnest(\""line"\")");
	line_length = length(line);
	if (line_length == 0) {
		return line;
	}
	start_bracket = 0;
	end_bracket = 0;
	nest_level = 0;
	for (i = 1; i <= line_length; i++) {
		char = substr(line, i, 1);
		if (char == "(") {
			if (i < line_length) {
				if  (substr(line, i + 1, 1) != ")") {
					if (start_bracket == 0) {
						start_bracket = i;
					}
					nest_level ++;
				} else {
					i ++;
				}
			}
		} else if (char == ")") {
			if (nest_level > 0) {
				nest_level --;
				if (nest_level == 0) {
					end_bracket = i;
					break;
				}
			}
		}
	}
	if (start_bracket == 0) {
		return line;
	}
	if (end_bracket < start_bracket) {
		end_bracket = line_length + 1;
	}
	begin = substr (line, 1, start_bracket);
	middle = substr (line, start_bracket+1, end_bracket - start_bracket - 1);
	end = substr (line, end_bracket);
	return rewrite_call(begin,middle) unnest(end);
}
BEGIN {
	debug_level = 0;
}
#{
#	print unnest($0);
#}
/.*->class->[a-zA-Z_]*\(.*\)/ {
	print unnest($0);
}
/.*\(\)->new[a-zA-Z_]*\(.*\)/ {
	print unnest($0);
}
! (/.*->class->[a-zA-Z_]*\(.*\)/||/.*\(\)->new[a-zA-Z_]*\(.*\)/) {
	print $0;
}
