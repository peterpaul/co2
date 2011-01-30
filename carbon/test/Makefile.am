EXTRA_DIST = success fail generate-testcases.sh run_succes_test.sh run_fail_test.sh
MAINTAINERCLEANFILES = Makefile.in

#SUBDIRS = success fail

targetdir = target

TESTS_ENVIRONMENT = CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

distclean-local:
	-rm -rf $(targetdir)

clean-local:
	-rm -f ${TESTS}

${TESTS}: ./generate-testcases.sh
	./generate-testcases.sh

TESTS=\
success_arithmetic_expressions.sh\
success_array.sh\
success_cast_expression.sh\
success_class_decl_constructor_parameters.sh\
success_class_decl_constructor.sh\
success_class_decl_destructor.sh\
success_class_decl_names.sh\
success_class_decl_object_vars.sh\
success_class_decl_self.sh\
success_class_decl_super.sh\
success_class_decl.sh\
success_class_intergrated.sh\
success_external_definition.sh\
success_function_call.sh\
success_fun_decl.sh\
success_fun_decl_vararg.sh\
success_fun_type.sh\
success_global_scope.sh\
success_import.sh\
success_interface_decl.sh\
success_interface_implementation.sh\
success_List.sh\
success_Object.sh\
success_ray.sh\
success_scope_simple.sh\
success_scope.sh\
success_size_expression.sh\
success_struct_decl.sh\
success_try_statement_finally.sh\
success_try_statement_nested_finally.sh\
success_try_statement_nested_return.sh\
success_try_statement_nested.sh\
success_try_statement_return.sh\
success_try_statement.sh\
success_typedef.sh\
success_va_arg.sh\
success_var_decl.sh\
fail_cast_expression.sh\
fail_class_decl_constructor_arguments.sh\
fail_class_decl_constructor_decl.sh\
fail_class_decl_constructor_name.sh\
fail_class_decl_constructor.sh\
fail_class_decl_redecl_var.sh\
fail_class_decl.sh\
fail_class_decl_type_check.sh\
fail_function_decl_args.sh\
fail_function_decl.sh\
fail_function_decl_vararg.sh\
fail_import.sh\
fail_interface_method_in_two_interfaces.sh\
fail_interface_not_implemented.sh\
fail_scope.sh\
fail_type_compat_int_float_1.sh\
fail_type_compat_int_float_2.sh\
fail_type_compat_int_float_3.sh\
fail_type_compat_int_float_4.sh