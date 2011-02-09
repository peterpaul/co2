EXTRA_DIST = pass fail generate-testcases.sh run_pass_test.sh run_fail_test.sh
MAINTAINERCLEANFILES = Makefile.in

targetdir = target

TESTS_ENVIRONMENT = CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LD="$(LD)"

distclean-local:
	-rm -rf $(targetdir)

clean-local:
	-rm -f ${TESTS}

${TESTS}, Makefile.am: ./generate-testcases.sh
	./generate-testcases.sh

TESTS=\
pass/arithmetic_expressions.sh\
pass/array.sh\
pass/cast_expression.sh\
pass/class_decl_constructor_parameters.sh\
pass/class_decl_constructor.sh\
pass/class_decl_constructor_vararg.sh\
pass/class_decl_destructor.sh\
pass/class_decl_get_class.sh\
pass/class_decl_names.sh\
pass/class_decl_object_vars.sh\
pass/class_decl_self.sh\
pass/class_decl_super.sh\
pass/class_decl.sh\
pass/class_intergrated.sh\
pass/dependencies.sh\
pass/external_definition.sh\
pass/function_call.sh\
pass/fun_decl.sh\
pass/fun_decl_vararg.sh\
pass/fun_type.sh\
pass/global_scope.sh\
pass/import.sh\
pass/interface_decl.sh\
pass/interface_implementation.sh\
pass/interface_override.sh\
pass/List.sh\
pass/loops.sh\
pass/Object.sh\
pass/ray.sh\
pass/scope_simple.sh\
pass/scope.sh\
pass/size_expression.sh\
pass/struct_decl.sh\
pass/try_statement_finally.sh\
pass/try_statement_nested_finally.sh\
pass/try_statement_nested_return.sh\
pass/try_statement_nested.sh\
pass/try_statement_return.sh\
pass/try_statement.sh\
pass/typedef.sh\
pass/va_arg.sh\
pass/var_decl.sh\
fail/cast_expression.sh\
fail/class_decl_constructor_arguments.sh\
fail/class_decl_constructor_decl.sh\
fail/class_decl_constructor_name.sh\
fail/class_decl_constructor.sh\
fail/class_decl_redecl_var.sh\
fail/class_decl.sh\
fail/class_decl_type_check.sh\
fail/function_decl_args.sh\
fail/function_decl.sh\
fail/function_decl_vararg.sh\
fail/import.sh\
fail/scope.sh\
fail/type_compat_int_float_1.sh\
fail/type_compat_int_float_2.sh\
fail/type_compat_int_float_3.sh\
fail/type_compat_int_float_4.sh