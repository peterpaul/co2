EXTRA_DIST = success fail generate-testcases.sh run_succes_test.sh run_fail_test.sh
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
success/arithmetic_expressions.sh\
success/array.sh\
success/cast_expression.sh\
success/class_decl_constructor_parameters.sh\
success/class_decl_constructor.sh\
success/class_decl_destructor.sh\
success/class_decl_names.sh\
success/class_decl_object_vars.sh\
success/class_decl_self.sh\
success/class_decl_super.sh\
success/class_decl.sh\
success/class_intergrated.sh\
success/external_definition.sh\
success/function_call.sh\
success/fun_decl.sh\
success/fun_decl_vararg.sh\
success/fun_type.sh\
success/global_scope.sh\
success/import.sh\
success/interface_decl.sh\
success/interface_implementation.sh\
success/List.sh\
success/Object.sh\
success/ray.sh\
success/scope_simple.sh\
success/scope.sh\
success/size_expression.sh\
success/struct_decl.sh\
success/try_statement_finally.sh\
success/try_statement_nested_finally.sh\
success/try_statement_nested_return.sh\
success/try_statement_nested.sh\
success/try_statement_return.sh\
success/try_statement.sh\
success/typedef.sh\
success/va_arg.sh\
success/var_decl.sh\
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