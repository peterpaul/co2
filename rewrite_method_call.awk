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
function rewrite_method(method, splitter, object, macro, method_prefix) {
	# print "rewrite_method(\""method"\", \""splitter"\", \""object"\")";

	split(method, list, splitter);	
	if (array_length(list) != 2) {
		# print "ERROR: Expected list of size 2!";
		return method;
	}

	split(list[1], prefix, object);
	if (array_length(prefix) != 1) {
		# print "WARNING: Expected prefix of size 1!";
		for (offset = 1; offset < array_length(prefix); offset ++) {
			# print "prefix["offset"] = "prefix[offset];
		}
		# return method;
	}
	
	meth = substr(list[2],1,length(list[2])-1);
	return prefix[1] macro "(" object", " method_prefix meth;
}
function rewrite_method_call(method,parameters, pos, parameter_length, result) {
	# print "rewrite_method_call(\""method"\", \""parameters"\")";
	parameters = unnest(parameters);
	comma = index(parameters, ",");
	object = substr(parameters, 1, comma - 1);
	parameters = substr(parameters, comma + 1);
	# print "parameters = "parameters;
	method = rewrite_method(method, "->class->", object, "O_CALL");
	# print "method = "method;
	return method "," parameters;
}
function rewrite_new_call(method, parameters) {
	# print "rewrite_new_call(\""method"\", \""parameters"\")";
	parameters = unnest(parameters);
	comma = index(parameters, ",");
	object = substr(parameters, 1, comma - 1);
	parameters = substr(parameters, comma + 1);
	# print "parameters = "parameters;
	method = rewrite_method(method, "->new", object, "O_CALL_CLASS", "new");
	# print "method = "method;
	return method "," parameters;
}
function rewrite_call(method,parameters) {
	# print "rewrite_call(\""method"\", \""parameters"\")";
	if (index(method, "->class->") > 0) {
		return rewrite_method_call(method, parameters);
	} if (index(method, "->new") > 0) {
		return rewrite_new_call(method, parameters);
	} else {
		return method unnest(parameters);
	}
}
function unnest(line, begin, middle, end) {
	# print "unnest(\""line"\")";
	line_length = length(line);
	start_bracket = index(line, "(");
	end_bracket = 0;
	if (start_bracket == 0) {
		return line;
	}
	nest_level = 1;
	for (i = start_bracket + 1; i <= line_length && end_bracket < start_bracket; i++) {
		char = substr(line, i, 1);
		if (char == "(") {
			nest_level ++;
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
	begin = substr (line, 1, start_bracket);
	middle = substr (line, start_bracket+1, end_bracket - start_bracket - 1);
	end = substr (line, end_bracket);
	return rewrite_call(begin,middle) unnest(end);
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
(/.*->class->[a-zA-Z_]*\(.*\)/||/.*\(\)->new[a-zA-Z_]*\(.*\)/) {
	print $0;
}

